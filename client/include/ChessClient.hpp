#pragma once

#include <cstdint>
#include <string>

class ChessClient
{
public:
	ChessClient(std::string serverIp, std::uint16_t serverPort);
	~ChessClient();

	ChessClient(const ChessClient &) = delete;
	ChessClient &operator=(const ChessClient &) = delete;

	ChessClient(ChessClient &&) = delete;
	ChessClient &operator=(ChessClient &&) = delete;

	void connectToServer();
	void disconnect();

	void sendLine(const std::string &message);
	std::string receiveLine();

	bool isConnected() const noexcept;

private:
	std::string serverIp_;
	std::uint16_t serverPort_;
	int socketFd_ = -1;
};
