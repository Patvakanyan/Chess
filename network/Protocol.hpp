#pragma once

#include <cstddef>
#include <string>

enum class MessageType
{
	Color,
	Role,
	Move,
	State,
	Error,
	GameOver,
	UNKNOWN
};

struct Message
{
	MessageType type;
	std::string data;
};

namespace Protocol
{
	// Maximum bytes before the newline, shared by client and server connections.
	inline constexpr std::size_t maxLineSize = 1022;

	std::string toString(MessageType type);

	std::string serialize(const Message &message);

	Message parse(const std::string &message);
}
