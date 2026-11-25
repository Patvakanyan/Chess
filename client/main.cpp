#include "ClientConnection.hpp"

#include <charconv>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace
{
	uint16_t parsePort(const std::string &text)
	{
		unsigned int port = 0;
		const auto result = std::from_chars(text.data(), text.data() + text.size(), port);
		if (result.ec != std::errc{} || result.ptr != text.data() + text.size() ||
			port == 0 || port > std::numeric_limits<uint16_t>::max())
			throw std::invalid_argument("Port must be a number between 1 and 65535");
		return static_cast<uint16_t>(port);
	}

	bool sendMove(ClientConnection &connection)
	{
		while (true)
		{
			std::cout << "Your move (e.g. e2e4), or quit: " << std::flush;
			std::string move;
			if (!std::getline(std::cin, move) || move == "quit")
			{
				connection.sendLine({MessageType::GameOver, "resign"});
				std::cout << "You resigned.\n";
				return false;
			}
			if (move.empty())
				continue;
			try
			{
				connection.sendLine({MessageType::Move, move});
				return true;
			}
			catch (const std::invalid_argument &error)
			{
				std::cerr << "Invalid move message: " << error.what() << '\n';
			}
			catch (const std::length_error &error)
			{
				std::cerr << "Invalid move message: " << error.what() << '\n';
			}
		}
	}
}

int main(int argc, char **argv)
{
	if (argc > 3 || (argc == 2 && std::string(argv[1]) == "--help"))
	{
		std::cout << "Usage: " << argv[0] << " [server-ip] [port]\n";
		return argc > 3 ? 1 : 0;
	}

	try
	{
		const std::string serverIp = argc > 1 ? argv[1] : "127.0.0.1";
		const uint16_t serverPort = argc > 2 ? parsePort(argv[2]) : 5555;
		ClientConnection connection(serverIp, serverPort);
		connection.connectToServer();
		std::cout << "Connected to " << serverIp << ':' << serverPort << std::endl;

		while (true)
		{
			const Message message = connection.receiveLine();
			std::cout << Protocol::toString(message.type) << ": " << message.data << std::endl;
			if (message.type == MessageType::GameOver)
				return 0;
			if (message.type == MessageType::Error || message.type == MessageType::UNKNOWN)
				return 1;

			const bool ourTurn = message.type == MessageType::Move ||
				(message.type == MessageType::Role && message.data == "COLOR WHITE");
			if (ourTurn && !sendMove(connection))
				return 0;
		}
	}
	catch (const ClientConnection::ConnectionClosed &)
	{
		std::cout << "Server disconnected.\n";
	}
	catch (const std::exception &error)
	{
		std::cerr << "Error: " << error.what() << '\n';
		return 1;
	}
	return 0;
}
