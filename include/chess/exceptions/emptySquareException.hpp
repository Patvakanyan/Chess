#pragma once

#include "chess/exceptions/chessException.hpp"

class EmptySquareException : public ChessException
{
public:
	explicit EmptySquareException(const std::string& message);
	const char* what() const noexcept override;
};
