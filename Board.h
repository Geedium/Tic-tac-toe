#pragma once
#include <Windows.h>
#include <SFML/Graphics.hpp>

class Board
{
public:
	Board();
	~Board();
public:
	VOID Update();
	VOID Render(sf::RenderWindow& window);
private:
	BYTE x;
	BYTE y;
	BYTE z;
public:
	BYTE* data;
};
