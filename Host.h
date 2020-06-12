#pragma once
#include <Windows.h>
#include <SFML/Network.hpp>

class Host
{
public:
	Host(const sf::IpAddress address, const UINT16 port);
private:
	VOID OnSocketHandle(sf::TcpSocket& socket);
private:
	friend static DWORD WINAPI Thread(LPVOID param);
private:
	DWORD dwThreadID;
	sf::IpAddress address;
	UINT16 port;
public:
	const CHAR* Address() const;
	const UINT16 Port() const;
};
