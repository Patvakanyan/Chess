#pragma once

#include "ClientConnection.hpp"

#include <cstdint>
#include <netinet/in.h>
#include <optional>

class ChessServer
{
private:
	sockaddr_in serverAddress{};

	int port;
	int serverSocket{-1};
	std::optional<ClientConnection> whiteClient;
	std::optional<ClientConnection> blackClient;

	void setupServer();
	void waitForClients();
	void relayMessages();

public:
	explicit ChessServer(uint16_t port);
	~ChessServer();

	void startServer();
	void stopServer() noexcept;
};
