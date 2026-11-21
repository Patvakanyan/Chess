#include "include/ChessServer.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
	try
	{
		ChessServer server(5555);
		server.startServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
