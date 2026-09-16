#include "chess/exceptions/invalidMove.hpp"


InvalidMoveException::InvalidMoveException(const std::string& message) : ChessException(message) {}

const char *InvalidMoveException::what() const noexcept
{
	return ChessException::what();
}
