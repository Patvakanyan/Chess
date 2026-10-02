#pragma once

#include <exception>
#include <string>

class ChessException : public std::exception
{
public:
	explicit ChessException(const std::string& message) ;

	virtual ~ChessException() noexcept = default;

	const char* what() const noexcept override;

private:
	std::string message_;
};
