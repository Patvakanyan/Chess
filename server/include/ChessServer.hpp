#pragma once

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

class ChessServer
{
private:
	sockaddr_in serverAddress{};

	int port;
	int serverSocket{-1};
	int whiteSocket{-1};
	int blackSocket{-1};

	void setupServer();
	void waitForClients();
	void relayMessages();

public:
	explicit ChessServer(uint16_t port);
	~ChessServer();

	void startServer();
	void stopServer();
	ssize_t sendMessage(int clientSocket, const char *message, size_t messageSize);
	ssize_t receiveLine(int clientSocket, char *buffer, size_t bufferSize);
};
