#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(450, 450), "Tic Tac Toe");
    std::pair<sf::RectangleShape, bool> tiles[3][3];
    
    resetGame:
    char gametable[3][3]{
        '-', '-', '-',
        '-', '-', '-',
        '-', '-', '-'
    };

    float gamepos[3] = {
        15.0f,
        15.0f * 11.5f,
        15.0f * 21.5f
    };

    bool isYourTurn = false;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            sf::Vector2f size(150.0f, 150.0f);
            if (!tiles[j][i].second)
            {
                tiles[j][i] = std::make_pair(sf::RectangleShape(size), true);

                tiles[j][i].first.setPosition(i * 150.0f, j * 150.0f);

                tiles[j][i].first.setOutlineColor(sf::Color::Black);
                tiles[j][i].first.setOutlineThickness(3);

                tiles[j][i].second = true;
            }
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            if (!isYourTurn) continue;

                            sf::Vector2i mpos = sf::Mouse::getPosition(window);
                            sf::Vector2f worldPos = window.mapPixelToCoords(mpos);

                            if (tiles[i][j].first.getGlobalBounds().contains(worldPos.y, worldPos.x))
                            {
                                if (gametable[i][j] != 'X' && gametable[i][j] != 'O')
                                {
                                    gametable[i][j] = 'X';
                                    std::cout << i << ',' << j << std::endl;
                                    isYourTurn = false;
                                }
                            }
                        }
                    }
                }
            }
        }

        window.clear();
        
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                window.draw(tiles[i][j].first);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (gametable[i][j] == 'X')
                {
                    sf::RectangleShape shape(sf::Vector2f(150.0F, 5.0F));
                    shape.setFillColor(sf::Color::Black);
                    shape.setPosition(gamepos[i], gamepos[j]);
                    shape.rotate(45.0F);
                    window.draw(shape);
                    shape.rotate(-90.0F);
                    shape.setPosition(gamepos[i], gamepos[j] + 105.0F);
                    window.draw(shape);
                }
                if (gametable[i][j] == 'O')
                {
                    sf::CircleShape shape(60.0F, 100U);
                    shape.setOutlineColor(sf::Color::Black);
                    shape.setFillColor(sf::Color::Transparent);
                    shape.setPosition(gamepos[i] - 5.0F, gamepos[j] - 5.0F);
                    shape.setOutlineThickness(5.0F);
                    window.draw(shape);
                }
            }
        }

        if (!isYourTurn)
        {
            int nx = rand() % 3;
            int ny = rand() % 3;

            if (gametable[nx][ny] == '-')
            {
                gametable[nx][ny] = 'O';

                if (gametable[nx - 1][ny] == 'O' && gametable[nx + 1][ny] == 'O')
                {
                    std::cout << "NPC wins." << std::endl;
                }

                isYourTurn = true;
            }
        }

        bool hasMovesLeft = false;

        for (int i = 0; i < 3; i++) // Processing row.
        {
            for (int j = 0; j < 3; j++) // Processing column.
            {
                if (gametable[i][j] == '-') hasMovesLeft = true;

                if (i == 0)
                {
                    if (gametable[0][j] == 'O' && gametable[1][j] == 'O' && gametable[2][j] == 'O')
                    {
                        std::cout << "NPC wins." << std::endl;
                        goto resetGame;
                    }

                    if (gametable[0][j] == 'X' && gametable[1][j] == 'X' && gametable[2][j] == 'X')
                    {
                        std::cout << "Player wins." << std::endl;
                        goto resetGame;
                    }
                }

                // DIAG. CHECK
                if (gametable[0][2] == 'O' && gametable[1][1] == 'O' && gametable[2][0] == 'O')
                {
                    std::cout << "NPC wins." << std::endl;
                    goto resetGame;
                }

                if (gametable[0][0] == 'O' && gametable[1][1] == 'O' && gametable[2][2] == 'O')
                {
                    std::cout << "NPC wins." << std::endl;
                    goto resetGame;
                }

                if (gametable[0][2] == 'X' && gametable[1][1] == 'X' && gametable[2][0] == 'X')
                {
                    std::cout << "Player wins." << std::endl;
                    goto resetGame;
                }

                if (gametable[0][0] == 'X' && gametable[1][1] == 'X' && gametable[2][2] == 'X')
                {
                    std::cout << "Player wins." << std::endl;
                    goto resetGame;
                }

                if (j == 0)
                {
                    if (gametable[i][0] == 'O' && gametable[i][1] == 'O' && gametable[i][2] == 'O')
                    {
                        std::cout << "NPC wins." << std::endl;
                        goto resetGame;
                    }

                    if (gametable[i][0] == 'X' && gametable[i][1] == 'X' && gametable[i][2] == 'X')
                    {
                        std::cout << "Player wins." << std::endl;
                        goto resetGame;
                    }
                }
            }
        }

        if (!hasMovesLeft)
        {
            std::cout << "It's a tie." << std::endl;
            goto resetGame;
        }

        window.display();
    }

    return 0;
}
