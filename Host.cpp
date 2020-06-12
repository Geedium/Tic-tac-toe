#include "Host.h"

Host::Host(const sf::IpAddress address, const UINT16 port)
	: address(address), port(port), dwThreadID(NULL)
{
	CreateThread(NULL, NULL, Thread, this, NULL, &dwThreadID);
}

VOID Host::OnSocketHandle(sf::TcpSocket& socket)
{
	sf::Packet packet;
	sf::Socket::Status s = socket.receive(packet);

	if (s == sf::Socket::Status::Done)
	{
		int x = 0;
		int y = 0;
		packet >> x >> y;
		socket.send(packet);
	}
}

static DWORD WINAPI Thread(LPVOID param)
{
	Host* thread_object = static_cast<Host*>(param);

	if (!thread_object)
	{
		return EXCEPTION_INVALID_HANDLE;
	}

	sf::IpAddress thread_address(thread_object->address);

	sf::TcpListener thread_listener;
	thread_listener.listen(thread_object->port, thread_address);

	sf::TcpSocket thread_socket;
	thread_listener.accept(thread_socket);

	while (TRUE)
	{
		thread_object->OnSocketHandle(thread_socket);
	}

	thread_listener.close();
	return EXIT_SUCCESS;
}

const char* Host::Address() const
{
	return address.toString().c_str();
}

const unsigned short Host::Port() const
{
	return port;
}
