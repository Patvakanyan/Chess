#pragma once

#include <cstdint>
#include <string>

class ChessClient
{
public:
	ChessClient(std::string serverIp, uint16_t serverPort);
	~ChessClient();

	void connectToServer();
	void disconnect();

	void sendLine(const std::string &message);
	std::string receiveLine();

private:
	std::string serverIp_;
	uint16_t serverPort_;
	int socketFd_ = -1;
};
