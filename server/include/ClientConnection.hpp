#pragma once

#include "network/Protocol.hpp"
#include <cstdint>
#include <stdexcept>
#include <string>

class ClientConnection
{
public:
	class ConnectionClosed : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	ClientConnection(std::string serverIp, uint16_t serverPort);
	explicit ClientConnection(int connectedSocket);
	~ClientConnection();

	void connectToServer();
	void disconnect() noexcept;

	ClientConnection(const ClientConnection &) = delete;
	ClientConnection &operator=(const ClientConnection &) = delete;
	void sendLine(const Message &message);
	Message receiveLine();

private:
	std::string serverIp_;
	uint16_t serverPort_ = 0;
	int socketFd_ = -1;
};
