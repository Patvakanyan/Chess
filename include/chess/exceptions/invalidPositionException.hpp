#pragma once

#include "chess/exceptions/chessException.hpp"

class InvalidPositionException : public ChessException
{
public:
	explicit InvalidPositionException(const std::string& message);
	const char *what() const noexcept override;
};
