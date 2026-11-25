#include "include/ClientConnection.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <stdexcept>
#include <system_error>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

ClientConnection::ClientConnection(std::string serverIp, uint16_t serverPort)
	: serverIp_(std::move(serverIp)), serverPort_(serverPort)
{
}

ClientConnection::ClientConnection(int connectedSocket)
	: socketFd_(connectedSocket)
{
	if (socketFd_ < 0)
		throw std::invalid_argument("Invalid connected socket");
}

void ClientConnection::connectToServer()
{
	if (socketFd_ >= 0)
		throw std::logic_error("Client is already connected");
	const int newSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (newSocket < 0)
		throw std::system_error(errno, std::generic_category(), "Failed to create socket");
	sockaddr_in serverAddress{};
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = ::htons(serverPort_);

	const int conversionResult = ::inet_pton(AF_INET, serverIp_.c_str(), &serverAddress.sin_addr);

	if (conversionResult != 1)
	{
		::close(newSocket);
		throw std::invalid_argument("Invalid server IP address: " + serverIp_);
	}
	const int connectionResult = ::connect(
		newSocket, reinterpret_cast<const sockaddr *>(&serverAddress), sizeof(serverAddress));
	if (connectionResult < 0)
	{
		const int errorCode = errno;
		::close(newSocket);
		throw std::system_error(errorCode, std::generic_category(), "Failed to connect to server");
	}

	socketFd_ = newSocket;
}

void ClientConnection::disconnect() noexcept
{
	if (socketFd_ >= 0)
	{
		::close(socketFd_);
		socketFd_ = -1;
	}
}

void ClientConnection::sendLine(const Message &message)
{
	if (socketFd_ < 0)
		throw std::logic_error("Client is not connected");
	const std::string messageWithNewline = Protocol::serialize(message);
	size_t totalSent = 0;
	while (totalSent < messageWithNewline.size())
	{
		const ssize_t bytesSent = ::send(socketFd_, messageWithNewline.data() + totalSent,
			messageWithNewline.size() - totalSent, MSG_NOSIGNAL);
		if (bytesSent < 0)
		{
			if (errno == EINTR)
				continue;
			const int errorCode = errno;
			disconnect();
			throw std::system_error(errorCode, std::generic_category(), "Failed to send message");
		}
		if (bytesSent == 0)
		{
			disconnect();
			throw ConnectionClosed("Connection closed while sending");
		}
		totalSent += static_cast<size_t>(bytesSent);
	}
}

Message ClientConnection::receiveLine()
{
	if (socketFd_ < 0)
		throw std::logic_error("Client is not connected");
	std::string receivedMessage;
	char character = '\0';
	while (true)
	{
		const ssize_t bytesRead = ::recv(socketFd_, &character, 1, 0);
		if (bytesRead < 0)
		{
			if (errno == EINTR)
				continue;
			const int errorCode = errno;
			disconnect();
			throw std::system_error(errorCode, std::generic_category(), "Failed to receive message");
		}
		if (bytesRead == 0)
		{
			disconnect();
			if (!receivedMessage.empty())
				throw std::runtime_error("Connection closed before newline");
			throw ConnectionClosed("Connection closed while receiving");
		}
		if (character == '\n')
			break;
		if (receivedMessage.size() >= Protocol::maxLineSize)
		{
			disconnect();
			throw std::runtime_error("Received line is too long");
		}
		receivedMessage += character;
	}
	return Protocol::parse(receivedMessage);
}

ClientConnection::~ClientConnection()
{
	disconnect();
}
