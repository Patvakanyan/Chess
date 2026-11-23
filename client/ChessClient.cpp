#include "ChessClient.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>
#include <utility>

ChessClient::ChessClient(std::string serverIp, uint16_t serverPort)
	: serverIp_(std::move(serverIp)), serverPort_(serverPort)
{
}

void ChessClient::connectToServer()
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

void ChessClient::sendLine(const std::string &message)
{
	if (socketFd_ < 0)
	{
		throw std::logic_error("Client is not connected");
	}
	std::string messageWithNewline = message;
	if (messageWithNewline.empty() || messageWithNewline.back() != '\n')
	{
		messageWithNewline += '\n';
	}
	size_t totalSent = 0;
	while (totalSent < messageWithNewline.size())
	{
		ssize_t bytesSent = ::send(socketFd_, messageWithNewline.data() + totalSent,
								   messageWithNewline.size() - totalSent, MSG_NOSIGNAL);

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
}

std::string ChessClient::receiveLine()
{
	if (socketFd_ < 0)
		throw std::logic_error("Client is not connected");
	constexpr std::size_t maxLineSize = 4096;

	std::string line;
	char character = '\0';

	while (true)
	{
		const ssize_t bytesRead = ::recv(socketFd_, &character, 1, 0);

		if (bytesRead < 0)
		{
			if (errno == EINTR)
				continue;
			const int errorCode = errno;
			throw std::system_error(
				errorCode, std::generic_category(), "Failed to receive message");
		}
		if (bytesRead == 0)
		{
			disconnect();
			throw std::runtime_error("Server closed the connection");
		}
		if (character == '\n')
			return line;
		if (line.size() >= maxLineSize)
			throw std::runtime_error("Received line is too long");
		line.push_back(character);
	}
}

bool ChessClient::isConnected() const noexcept
{
	return socketFd_ >= 0;
}

void ChessClient::disconnect()
{
	if (socketFd_ >= 0)
	{
		::close(socketFd_);
		socketFd_ = -1;
	}
}

ChessClient::~ChessClient()
{
	disconnect();
}
