#include "chess/exceptions/chessException.hpp"


ChessException::ChessException(const std::string& message) : message_(message) {}

const char *ChessException::what() const noexcept
{
	return message_.c_str();
}
