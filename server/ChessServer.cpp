#include "include/ChessServer.hpp"

#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

ChessServer::ChessServer(uint16_t serverPort)
	: serverAddress{}, port(serverPort), serverSocket(-1)
{
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = ::htons(port);
	serverAddress.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);
}

void ChessServer::startServer()
{
	try
	{
		setupServer();
		waitForClients();
		relayMessages();
	}
	catch (...)
	{
		stopServer();
		throw;
	}
	stopServer();
}

void ChessServer::setupServer()
{
	serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to create server socket");
	int reuseAddress = 1;
	if (::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to set SO_REUSEADDR");
	if (::bind(serverSocket, reinterpret_cast<const sockaddr *>(&serverAddress),
			   sizeof(serverAddress)) < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to bind server socket");
	if (::listen(serverSocket, 2) < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to listen");

	std::cout << "Server listening on 127.0.0.1:" << port << '\n';
}

void ChessServer::waitForClients()
{
	const auto acceptClient = [this](const char *errorMessage)
	{
		int clientSocket;
		do
		{
			clientSocket = ::accept(serverSocket, nullptr, nullptr);
		} while (clientSocket < 0 && errno == EINTR);
		if (clientSocket < 0)
			throw std::system_error(errno, std::generic_category(), errorMessage);
		return clientSocket;
	};

	whiteClient.emplace(acceptClient("Failed to accept White"));
	std::cout << "White connected\n";
	whiteClient->sendLine({MessageType::State, "WAITING FOR OPPONENT"});

	blackClient.emplace(acceptClient("Failed to accept Black"));
	std::cout << "Black connected\n";
	whiteClient->sendLine({MessageType::Role, "COLOR WHITE"});
	blackClient->sendLine({MessageType::Role, "COLOR BLACK"});
}

void ChessServer::relayMessages()
{
	const auto relayMessage = [](ClientConnection &sender, ClientConnection &receiver,
								const char *playerName)
	{
		Message message{};
		try
		{
			message = sender.receiveLine();
		}
		catch (const ClientConnection::ConnectionClosed &)
		{
			std::cout << playerName << " disconnected\n";
			return false;
		}
		std::cout << "Received from " << playerName << ": "
			<< Protocol::toString(message.type) << ':' << message.data << '\n';
		receiver.sendLine(message);
		if (message.type == MessageType::GameOver)
		{
			std::cout << "Game over message received from " << playerName << ". Ending relay.\n";
			return false;
		}
		return true;
	};

	while (relayMessage(*whiteClient, *blackClient, "White") &&
		   relayMessage(*blackClient, *whiteClient, "Black"))
	{
	}
}

void ChessServer::stopServer() noexcept
{
	whiteClient.reset();
	blackClient.reset();
	if (serverSocket >= 0)
	{
		::close(serverSocket);
		serverSocket = -1;
	}
}

ChessServer::~ChessServer()
{
	stopServer();
}
