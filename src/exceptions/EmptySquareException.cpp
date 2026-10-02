#include "chess/exceptions/EmptySquareException.hpp"

EmptySquareException::EmptySquareException(const std::string& message) : ChessException(message) {}

const char* EmptySquareException::what() const noexcept
{
	return ChessException::what();
}
