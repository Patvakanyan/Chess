#pragma once

#include "chess/exceptions/ChessException.hpp"

class InvalidMoveException : public ChessException
{
public:
	explicit InvalidMoveException(const std::string& message);
	const char *what() const noexcept override;
};
