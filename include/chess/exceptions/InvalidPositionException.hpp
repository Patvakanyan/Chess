#pragma once

#include "chess/exceptions/ChessException.hpp"

class InvalidPositionException : public ChessException
{
public:
	explicit InvalidPositionException(const std::string& message);
	const char *what() const noexcept override;
};
