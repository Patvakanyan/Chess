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
		_exit(EXIT_FAILURE);
	}
	close(pipeToStockfish_[0]);
	close(pipeFromStockfish_[1]);
	char *const args[] = {const_cast<char *>(stockfishPath_), nullptr};
	execvp(stockfishPath_, args);
	perror("Failed to execute Stockfish engine");
	_exit(EXIT_FAILURE);
}

std::string StockfishEngine::sendCommand(const std::string &command, const std::string &target)
{
	if (processId_ <= 0)
		throw std::runtime_error("Stockfish is not running.");

	const std::string line = command + "\n";
	if (write(pipeToStockfish_[1], line.c_str(), line.size()) != static_cast<ssize_t>(line.size()))
		throw std::runtime_error("Failed to write to Stockfish.");

	if (target.empty())
		return {};

	std::string response;
	char buffer[1024];
	while (true)
	{
		size_t pos = response.find(target);
		if (pos != std::string::npos && response.find('\n', pos) != std::string::npos)
			return response;
		ssize_t n = read(pipeFromStockfish_[0], buffer, sizeof(buffer));
		if (n <= 0)
			throw std::runtime_error("Stockfish closed unexpectedly.");
		response.append(buffer, n);
	}
}

std::string StockfishEngine::getBestMove(const std::string &fen, int depth)
{
	sendCommand("position fen " + fen, "");
	std::string response = sendCommand("go depth " + std::to_string(depth), "bestmove");

	std::istringstream in(response.substr(response.find("bestmove")));
	std::string word, move;
	in >> word >> move;
	if (move.empty() || move == "(none)")
		return "";
	return move;
}
int StockfishEngine::startEngine()
{
	if (processId_ > 0)
		return 1;

	if (pipeToStockfish_[0] == -1 || pipeToStockfish_[1] == -1)
	{
		closePipe(pipeToStockfish_);
		if (pipe(pipeToStockfish_) == -1)
			throw std::runtime_error("Failed to create pipes for Stockfish engine.");
	}
	if (pipeFromStockfish_[0] == -1 || pipeFromStockfish_[1] == -1)
	{
		closePipe(pipeFromStockfish_);
		if (pipe(pipeFromStockfish_) == -1)
		{
			closePipe(pipeToStockfish_);
			throw std::runtime_error("Failed to create pipes for Stockfish engine.");
		}
	}

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
		sendCommand("uci", "uciok");
		sendCommand("isready", "readyok");
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
