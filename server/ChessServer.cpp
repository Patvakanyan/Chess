#include "include/ChessServer.hpp"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

ChessServer::ChessServer(uint16_t serverPort)
	: serverAddress{}, port(serverPort), serverSocket(-1), whiteSocket(-1), blackSocket(-1)
{
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

void ChessServer::startServer()
{
	setupServer();
	waitForClients();
	relayMessages();
	stopServer();
}

void ChessServer::setupServer()
{
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket < 0)
	{
		throw std::system_error(errno, std::generic_category(), "Failed to create server socket");
	}

	int reuseAddress = 1;

	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to set SO_REUSEADDR");
	if (bind(serverSocket, reinterpret_cast<const sockaddr *>(&serverAddress),
			 sizeof(serverAddress)) < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to bind server socket");
	if (listen(serverSocket, 2) < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to listen");

	std::cout << "Server listening on 127.0.0.1:" << port << '\n';
}

void ChessServer::waitForClients()
{
	whiteSocket = accept(serverSocket, nullptr, nullptr);

	if (whiteSocket < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to accept White");
	std::cout << "White connected\n";
	const char waitingMessage[] = "WAITING FOR OPPONENT\n";

	sendMessage(whiteSocket, waitingMessage, sizeof(waitingMessage) - 1);

	blackSocket = accept(serverSocket, nullptr, nullptr);

	if (blackSocket < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to accept Black");

	std::cout << "Black connected\n";
	const char whiteRole[] = "COLOR WHITE\n";
	const char blackRole[] = "COLOR BLACK\n";
	sendMessage(whiteSocket, whiteRole, sizeof(whiteRole) - 1);
	sendMessage(blackSocket, blackRole, sizeof(blackRole) - 1);
}

void ChessServer::relayMessages()
{
	char whiteBuffer[1024];
	char blackBuffer[1024];

	while (true)
	{
		ssize_t whiteBytes = receiveLine(whiteSocket, whiteBuffer, sizeof(whiteBuffer));
		if (whiteBytes == 0)
		{
			std::cout << "White disconnected\n";
			break;
		}

		std::cout << "Received from White: " << whiteBuffer;
		sendMessage(blackSocket, whiteBuffer, static_cast<size_t>(whiteBytes));
		ssize_t blackBytes = receiveLine(blackSocket, blackBuffer, sizeof(blackBuffer));
		if (blackBytes == 0)
		{
			std::cout << "Black disconnected\n";
			break;
		}
		std::cout << "Received from Black: " << blackBuffer;
		sendMessage(whiteSocket, blackBuffer, static_cast<size_t>(blackBytes));
	}
}

ssize_t ChessServer::receiveLine(int clientSocket, char *buffer, size_t bufferSize)
{
	if (buffer == nullptr || bufferSize < 2)
	{
		throw std::invalid_argument("Invalid receive buffer");
	}

	size_t totalRead = 0;

	while (true)
	{
		char character = '\0';

		ssize_t bytesRead = recv(clientSocket, &character, 1, 0);

		if (bytesRead < 0)
		{
			if (errno == EINTR)
				continue;
			throw std::system_error(errno, std::generic_category(), "Failed to receive message");
		}
		if (bytesRead == 0)
		{
			buffer[0] = '\0';
			if (totalRead == 0)
				return 0;
			throw std::runtime_error("Connection closed before newline");
		}

		if (character == '\n')
		{
			if (totalRead >= bufferSize - 1)
				throw std::runtime_error("Received line is too long");
			buffer[totalRead] = character;
			++totalRead;
			break;
		}

		if (totalRead >= bufferSize - 2)
			throw std::runtime_error("Received line is too long");
		buffer[totalRead] = character;
		++totalRead;
	}
	buffer[totalRead] = '\0';
	return static_cast<ssize_t>(totalRead);
}

ssize_t ChessServer::sendMessage(int clientSocket, const char *message, size_t messageSize)
{
	if (message == nullptr || messageSize == 0)
		throw std::invalid_argument("Invalid message");
	size_t totalSent = 0;
	while (totalSent < messageSize)
	{
		ssize_t bytesSent =
			send(clientSocket, message + totalSent, messageSize - totalSent, MSG_NOSIGNAL);

		if (bytesSent < 0)
		{
			if (errno == EINTR)
				continue;
			throw std::system_error(errno, std::generic_category(), "Failed to send message");
		}
		if (bytesSent == 0)
			throw std::runtime_error("Connection closed while sending");
		totalSent += static_cast<size_t>(bytesSent);
	}
	return static_cast<ssize_t>(totalSent);
}

void ChessServer::stopServer()
{
	if (whiteSocket >= 0)
	{
		close(whiteSocket);
		whiteSocket = -1;
	}
	if (blackSocket >= 0)
	{
		close(blackSocket);
		blackSocket = -1;
	}

	if (serverSocket >= 0)
	{
		close(serverSocket);
		serverSocket = -1;
	}
}

ChessServer::~ChessServer()
{
	stopServer();
}
