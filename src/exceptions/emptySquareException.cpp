#include "chess/exceptions/emptySquareException.hpp"

EmptySquareException::EmptySquareException(const std::string& message) : ChessException(message) {}

const char* EmptySquareException::what() const noexcept
{
	return ChessException::what();
}
