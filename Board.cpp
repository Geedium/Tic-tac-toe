#include "Board.h"

Board::Board()
	: x(NULL), y(NULL), z('-')
{
	data = new BYTE[9];
}

Board::~Board()
{
	delete[] data;
}

VOID Board::Update()
{
	data[x, y] = z;
}

VOID Board::Render(sf::RenderWindow& window)
{
	static FLOAT p[3] = { 20.0F, 172.5F, 322.5 };

	for (BYTE i = NULL; i < 3; i++)
	{
		for (BYTE j = NULL; j < 3; j++)
		{
			BYTE n = data[j * 3 + i];

			if (n == 'X')
			{
				sf::RectangleShape shape(sf::Vector2f(150.0F, 5.0F));
				shape.setFillColor(sf::Color::Black);
				shape.setPosition(p[i], p[j]);
				shape.rotate(45.0F);
				window.draw(shape);
				shape.rotate(-90.0F);
				shape.setPosition(p[i], p[j] + 105.0F);
				window.draw(shape);
			}
			else if(n == 'O')
			{
				sf::CircleShape shape(60.0F, 100U);
				shape.setOutlineColor(sf::Color::Black);
				shape.setFillColor(sf::Color::Transparent);
				shape.setPosition(p[i] - 5.0F, p[j] - 5.0F);
				shape.setOutlineThickness(5.0F);
				window.draw(shape);
			}
		}
	}
}
