#include "../include/chess/StockfishEngine.hpp"

StockfishEngine::StockfishEngine() : processId_(-1)
{
	if (pipe(pipeToStockfish_) == -1)
		throw std::runtime_error("Failed to create pipes for Stockfish engine.");
	if (pipe(pipeFromStockfish_) == -1)
	{
		closePipe(pipeToStockfish_);
		throw std::runtime_error("Failed to create pipes for Stockfish engine.");
	}
}

void StockfishEngine::closePipe(int pipe[2]) noexcept
{
	for (int index = 0; index < 2; ++index)
	{
		int &descriptor = pipe[index];
		if (descriptor != -1)
		{
			close(descriptor);
			descriptor = -1;
		}
	}
}

void StockfishEngine::workWithChildProcess()
{
	close(pipeToStockfish_[1]);
	close(pipeFromStockfish_[0]);
	if (dup2(pipeToStockfish_[0], STDIN_FILENO) == -1 || dup2(pipeFromStockfish_[1], STDOUT_FILENO) == -1)
	{
		std::cerr << "Failed to redirect pipes for Stockfish engine." << std::endl;
		exit(EXIT_FAILURE);
	}
	close(pipeToStockfish_[0]);
	close(pipeFromStockfish_[1]);
	char *const args[] = {const_cast<char *>(stockfishPath_), nullptr};
	execvp(stockfishPath_, args);
	perror("Failed to execute Stockfish engine");
	exit(EXIT_FAILURE);
}

std::string StockfishEngine::sendCommand(const std::string &command, std::string &target)
{
	std::string response;
	if (processId_ > 0)
	{
		write(pipeToStockfish_[1], command.c_str(), command.size());
		write(pipeToStockfish_[1], "\n", 1);
		char buffer[1024];
		while (true)
		{
			ssize_t bytesRead = read(pipeFromStockfish_[0], buffer, sizeof(buffer) - 1);
			if (bytesRead <= 0)
				break;
			buffer[bytesRead] = '\0';
			response = buffer;
			if (response.find(target) != std::string::npos)
				break;
		}
	}
	return response;
}

std::string StockfishEngine::getBestMove(const std::string &fen, int depth)
{
	std::string command = "position fen " + fen + "\ngo depth " + std::to_string(depth);
	std::string target = "bestmove";
	std::string bestMoveResponse = sendCommand(command, target);
	size_t bestMovePos = bestMoveResponse.find(target);

	if (bestMovePos != std::string::npos)
	{
		size_t moveStart = bestMovePos + target.length() + 1;
		size_t moveEnd = bestMoveResponse.find(' ', moveStart);
		if (moveEnd == std::string::npos)
			moveEnd = bestMoveResponse.length();
		return bestMoveResponse.substr(moveStart, moveEnd - moveStart);
	}
	else
	{
		throw std::runtime_error("Failed to get the best move from Stockfish engine.");
	}
}
int StockfishEngine::startEngine()
{
	processId_ = fork();
	if (processId_ < 0)
	{
		closePipe(pipeToStockfish_);
		closePipe(pipeFromStockfish_);
		throw std::runtime_error("Failed to fork process for Stockfish engine.");
	}
	if (processId_ == 0)
		workWithChildProcess();
	else
	{
		close(pipeToStockfish_[0]);
		close(pipeFromStockfish_[1]);
		pipeToStockfish_[0] = -1;
		pipeFromStockfish_[1] = -1;
	}
	return 1;
}

int StockfishEngine::stopEngine()
{
	if (processId_ <= 0)
		return 0;

	int status;
	const char command[] = "quit\n";
	write(pipeToStockfish_[1], command, sizeof(command) - 1);
	closePipe(pipeToStockfish_);
	closePipe(pipeFromStockfish_);
	waitpid(processId_, &status, 0);
	processId_ = -1;
	return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

StockfishEngine::~StockfishEngine()
{
	stopEngine();
}
/*
int main()
{
	StockfishEngine engine;
	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	engine.startEngine();
	std::string bestMove = engine.getBestMove(fen, 10);
	engine.stopEngine();
	return 0;
}
**/
