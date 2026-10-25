#include "chess/exceptions/InvalidPositionException.hpp"

InvalidPositionException::InvalidPositionException(const std::string &message) : ChessException(message) {}

const char *InvalidPositionException::what() const noexcept
{
	return ChessException::what();
}
