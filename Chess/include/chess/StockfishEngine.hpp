#pragma once

#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <fstream>
#include <sstream>
#include "Move.hpp"

#ifndef CHESS_STOCKFISH_PATH
#define CHESS_STOCKFISH_PATH "stockfish"
#endif

class StockfishEngine
{
private:
	const char *stockfishPath_ = CHESS_STOCKFISH_PATH;
	pid_t processId_;
	int pipeToStockfish_[2] = {-1, -1};	  // Pipe for sending commands to Stockfish
	int pipeFromStockfish_[2] = {-1, -1}; // Pipe for receiving output from Stockfish

	static void closePipe(int pipe[2]) noexcept;

public:
	StockfishEngine();
	int startEngine();
	int stopEngine();

	const char *getReadPipe() const;
	void workWithChildProcess();
	std::string sendCommand(const std::string &command, const std::string &target);

	Move getBestMove(const std::string &fen, int depth = 10);
	~StockfishEngine();
};
