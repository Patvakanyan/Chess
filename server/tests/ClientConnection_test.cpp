#include "ClientConnection.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>

namespace
{
	void require(bool condition, const char *message)
	{
		if (!condition)
			throw std::runtime_error(message);
	}

	template <typename Exception, typename Action>
	void expectThrow(Action action)
	{
		try
		{
			action();
		}
		catch (const Exception &)
		{
			return;
		}
		throw std::runtime_error("Expected an exception");
	}

	class Socket
	{
	public:
		explicit Socket(int fd) : fd_(fd)
		{
			if (fd_ < 0)
				throw std::system_error(errno, std::generic_category(), "Test socket failed");
		}
		~Socket() { close(); }
		Socket(const Socket &) = delete;
		Socket &operator=(const Socket &) = delete;
		int get() const { return fd_; }
		int release()
		{
			const int fd = fd_;
			fd_ = -1;
			return fd;
		}
		void close()
		{
			if (fd_ >= 0)
				::close(fd_);
			fd_ = -1;
		}

	private:
		int fd_;
	};

	class LocalServer
	{
	public:
		LocalServer() : listener_(::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
		{
			sockaddr_in address{};
			address.sin_family = AF_INET;
			address.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);
			require(::bind(listener_.get(), reinterpret_cast<sockaddr *>(&address),
				sizeof(address)) == 0, "Test bind failed");
			require(::listen(listener_.get(), 2) == 0, "Test listen failed");
			socklen_t length = sizeof(address);
			require(::getsockname(listener_.get(), reinterpret_cast<sockaddr *>(&address),
				&length) == 0, "Test getsockname failed");
			port_ = ::ntohs(address.sin_port);
		}
		uint16_t port() const { return port_; }
		Socket accept() { return Socket(::accept(listener_.get(), nullptr, nullptr)); }

	private:
		Socket listener_;
		uint16_t port_ = 0;
	};

	void sendAll(int fd, const std::string &data)
	{
		std::size_t sent = 0;
		while (sent < data.size())
		{
			const ssize_t count = ::send(fd, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
			if (count < 0 && errno == EINTR)
				continue;
			require(count > 0, "Test send failed");
			sent += static_cast<std::size_t>(count);
		}
	}

	std::string readBytes(int fd, std::size_t size)
	{
		std::string data(size, '\0');
		std::size_t received = 0;
		while (received < size)
		{
			const ssize_t count = ::recv(fd, data.data() + received, size - received, 0);
			if (count < 0 && errno == EINTR)
				continue;
			require(count > 0, "Test receive failed");
			received += static_cast<std::size_t>(count);
		}
		return data;
	}

	void checkReconnect(ClientConnection &connection, LocalServer &server)
	{
		connection.connectToServer();
		auto peer = server.accept();
		sendAll(peer.get(), "STATE:ready\n");
		const Message message = connection.receiveLine();
		require(message.type == MessageType::State && message.data == "ready",
			"Reconnected socket did not receive a fresh frame");
	}

	void normalFrames()
	{
		LocalServer server;
		ClientConnection connection("127.0.0.1", server.port());
		connection.connectToServer();
		auto peer = server.accept();
		sendAll(peer.get(), "MO");
		sendAll(peer.get(), "VE:e2e4\nSTATE:ready:now\n");
		const Message first = connection.receiveLine();
		const Message second = connection.receiveLine();
		require(first.type == MessageType::Move && first.data == "e2e4", "Incorrect first frame");
		require(second.type == MessageType::State && second.data == "ready:now",
			"Incorrect second frame");
		connection.sendLine(first);
		require(readBytes(peer.get(), 10) == "MOVE:e2e4\n", "Incorrect outgoing frame");
	}

	void acceptedConnectionOwnsSocket()
	{
		LocalServer server;
		ClientConnection client("127.0.0.1", server.port());
		client.connectToServer();
		auto peer = server.accept();
		{
			ClientConnection accepted(peer.release());
			client.sendLine({MessageType::Move, "e2e4"});
			const Message message = accepted.receiveLine();
			require(message.type == MessageType::Move && message.data == "e2e4",
				"Accepted connection did not receive the move");
			accepted.sendLine({MessageType::State, "ready"});
			const Message response = client.receiveLine();
			require(response.type == MessageType::State && response.data == "ready",
				"Accepted connection did not send the response");
		}
		expectThrow<ClientConnection::ConnectionClosed>([&]() { client.receiveLine(); });
	}

	void invalidSendsLeaveStreamIntact()
	{
		LocalServer server;
		ClientConnection connection("127.0.0.1", server.port());
		connection.connectToServer();
		auto peer = server.accept();
		for (const char *data : {"e2e4\nGAME_OVER:resign", "e2e4\rGAME_OVER:resign"})
		{
			expectThrow<std::invalid_argument>([&]() {
				connection.sendLine({MessageType::Move, data});
			});
		}
		expectThrow<std::length_error>([&]() {
			connection.sendLine({MessageType::Move, std::string(Protocol::maxLineSize - 4, 'x')});
		});
		connection.sendLine({MessageType::Move, "e2e4"});
		require(readBytes(peer.get(), 10) == "MOVE:e2e4\n", "Invalid send wrote bytes to the socket");
	}

	void maximumLengthFrames()
	{
		LocalServer server;
		ClientConnection connection("127.0.0.1", server.port());
		connection.connectToServer();
		auto peer = server.accept();
		const Message message{MessageType::Move, std::string(Protocol::maxLineSize - 5, 'x')};
		const std::string frame = Protocol::serialize(message);
		require(frame.size() == Protocol::maxLineSize + 1, "Incorrect frame size limit");
		connection.sendLine(message);
		require(readBytes(peer.get(), frame.size()) == frame, "Maximum-size send failed");
		sendAll(peer.get(), frame);
		require(connection.receiveLine().data == message.data, "Maximum-size receive failed");
	}

	void oversizedReceiveDisconnects()
	{
		LocalServer server;
		ClientConnection connection("127.0.0.1", server.port());
		connection.connectToServer();
		auto peer = server.accept();
		// There is deliberately no newline: the limit must apply before a full line arrives.
		sendAll(peer.get(), std::string(Protocol::maxLineSize + 1, 'x'));
		expectThrow<std::runtime_error>([&]() { connection.receiveLine(); });
		expectThrow<std::logic_error>([&]() { connection.sendLine({MessageType::Move, "e2e4"}); });
		checkReconnect(connection, server);
	}

	void eofDisconnects()
	{
		for (const char *prefix : {"", "MOVE:e2"})
		{
			LocalServer server;
			ClientConnection connection("127.0.0.1", server.port());
			connection.connectToServer();
			auto peer = server.accept();
			sendAll(peer.get(), prefix);
			peer.close();
			expectThrow<std::runtime_error>([&]() { connection.receiveLine(); });
			checkReconnect(connection, server);
		}
	}

	void resetDisconnects(bool duringSend)
	{
		LocalServer server;
		ClientConnection connection("127.0.0.1", server.port());
		connection.connectToServer();
		auto peer = server.accept();
		const linger reset{1, 0};
		require(::setsockopt(peer.get(), SOL_SOCKET, SO_LINGER, &reset, sizeof(reset)) == 0,
			"Could not configure test reset");
		peer.close();
		try
		{
			if (duringSend)
			{
				for (int attempt = 0; attempt < 10; ++attempt)
					connection.sendLine({MessageType::Move, "e2e4"});
			}
			else
				connection.receiveLine();
			throw std::runtime_error("Expected reset to fail socket I/O");
		}
		catch (const std::system_error &error)
		{
			require(error.code().value() == ECONNRESET || error.code().value() == EPIPE,
				"Socket error code was not preserved");
		}
		checkReconnect(connection, server);
	}
}

int main()
{
	int failures = 0;
	const auto test = [&](const char *name, auto run) {
		try
		{
			run();
			std::cout << "PASS: " << name << '\n';
		}
		catch (const std::exception &error)
		{
			++failures;
			std::cerr << "FAIL: " << name << ": " << error.what() << '\n';
		}
	};
	test("normal frames", normalFrames);
	test("accepted connection owns and closes its socket", acceptedConnectionOwnsSocket);
	test("invalid accepted socket is rejected", []() {
		expectThrow<std::invalid_argument>([]() { ClientConnection invalid(-1); });
	});
	test("invalid sends leave stream intact", invalidSendsLeaveStreamIntact);
	test("maximum-length frames", maximumLengthFrames);
	test("oversized receive disconnects", oversizedReceiveDisconnects);
	test("EOF disconnects, including a partial frame", eofDisconnects);
	test("receive error disconnects", []() { resetDisconnects(false); });
	test("send error disconnects", []() { resetDisconnects(true); });
	return failures == 0 ? 0 : 1;
}
