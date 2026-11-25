#include "Protocol.hpp"

#include <stdexcept>

std::string Protocol::toString(MessageType type)
{
	switch (type)
	{
	case MessageType::Color:
		return "COLOR";
	case MessageType::Role:
		return "ROLE";
	case MessageType::Move:
		return "MOVE";
	case MessageType::State:
		return "STATE";
	case MessageType::Error:
		return "ERROR";
	case MessageType::GameOver:
		return "GAME_OVER";
	case MessageType::UNKNOWN:
		return "UNKNOWN";
	}
	return "UNKNOWN";
}

std::string Protocol::serialize(const Message &message)
{
	if (message.data.find_first_of("\r\n") != std::string::npos)
		throw std::invalid_argument("Message data must not contain line breaks");
	const std::string typeName = toString(message.type);
	if (message.data.size() > maxLineSize - typeName.size() - 1)
		throw std::length_error("Message is too long");
	return typeName + ":" + message.data + "\n";
}

Message Protocol::parse(const std::string &message)
{
	if (message.empty())
		return {MessageType::UNKNOWN, "Empty message"};

	size_t colonPos = message.find(':');
	if (colonPos == std::string::npos)
		return {MessageType::Error, "Invalid message format"};

	const std::string typeName = message.substr(0, colonPos);
	MessageType type;
	if (typeName == "COLOR")
		type = MessageType::Color;
	else if (typeName == "ROLE")
		type = MessageType::Role;
	else if (typeName == "MOVE")
		type = MessageType::Move;
	else if (typeName == "STATE")
		type = MessageType::State;
	else if (typeName == "ERROR")
		type = MessageType::Error;
	else if (typeName == "GAME_OVER")
		type = MessageType::GameOver;
	else if (typeName == "UNKNOWN")
		type = MessageType::UNKNOWN;
	else
		return {MessageType::UNKNOWN, "Unknown message type"};

	std::string data = message.substr(colonPos + 1);
	if (!data.empty() && data.back() == '\n')
		data.pop_back();

	return {type, data};
}
