#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;
using namespace sf;

bool isConnected(TcpSocket& socket)
{
    return socket.getRemoteAddress() != IpAddress::None;
}

int main(int argc, char* argv[])
{
    TcpListener NTListener;
    TcpSocket NTMaster;
    TcpSocket NTClient;
    Packet NTRPacket;

    unsigned char cc = '-';

    bool assignedTurn = false;

    bool Connected = false;
    bool isYourTurn = false;

    for (signed i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--server") == 0)
        {
            if (NTListener.listen(2000) != Socket::Done)
            {
                cerr << "Error binding to a port." << endl;
                return EXIT_FAILURE;
            }

            while (true)
            {
                if (isConnected(NTMaster) == false)
                {
                    if (NTListener.accept(NTMaster) == Socket::Done)
                    {
                        cout << "Master socket connected." << endl;

                        Packet data;
                        data << true;
                        NTMaster.send(data);
                    }
                    else
                    {
                        cerr << "Error accepting master socket." << endl;
                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    if (NTMaster.receive(NTRPacket) == Socket::Done)
                    {
                        cout << "Received data from a master socket." << endl;
                        cout << "Redirecting data to a client socket." << endl;
                        NTClient.send(NTRPacket);
                    }
                }

                if (isConnected(NTClient) == false)
                {
                    if (NTListener.accept(NTClient) == Socket::Done)
                    {
                        cout << "Client socket connected." << endl;

                        Packet data;
                        data << false;
                        NTClient.send(data);
                    }
                    else
                    {
                        cerr << "Error accepting client socket." << endl;
                        return EXIT_FAILURE;
                    }
                }
                else
                {               
                    if (NTClient.receive(NTRPacket) == Socket::Done)
                    {
                        cout << "Received data from a client socket." << endl;
                        cout << "Redirecting data to a master socket." << endl;
                        NTMaster.send(NTRPacket);
                    }
                }
            }

            return EXIT_SUCCESS;
        }

        if (strcmp(argv[i], "--client") == 0 && i + 2 <= argc)
        {
            if (NTClient.connect(IpAddress(argv[i + 1]), 2000) != Socket::Done)
            {
                cerr << "Could not connect to server!" << endl;
                return EXIT_FAILURE;
            }

            Connected = true;
        }
    }

    sf::RenderWindow window(sf::VideoMode(450, 450), "Tic Tac Toe");
    std::pair<sf::RectangleShape, bool> tiles[3][3];

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

    while (window.isOpen() == true)
    {
        if (!assignedTurn)
        {
            if (NTClient.receive(NTRPacket) == Socket::Done)
            {
                if (NTRPacket >> isYourTurn)
                {
                    if (isYourTurn)
                    {
                        cout << "Your turn is now." << endl;
                        cc = 'X';
                    }
                    else
                    {
                        cout << "You have to wait for your turn." << endl;
                        cc = 'O';
                    }
                    assignedTurn = true;
                }
            }
        }
        else
        {
            if (!isYourTurn)
            {
                if (NTClient.receive(NTRPacket) == Socket::Done)
                {
                    cout << "Received a packet containing " << NTRPacket.getDataSize() << " bytes. " << endl;

                    sf::Uint32 x, y;
                    if (NTRPacket >> x >> y)
                    {
                        gametable[x][y] = (cc == 'X' ? 'O' : 'X');
                        cout << "Received an update to board." << endl;
                        isYourTurn = true;
                    }
                }
            }
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    for (sf::Uint32 i = 0; i < 3u; i++)
                    {
                        for (sf::Uint32 j = 0; j < 3u; j++)
                        {
                            if (!isYourTurn) continue;

                            sf::Vector2i mpos = sf::Mouse::getPosition(window);
                            sf::Vector2f worldPos = window.mapPixelToCoords(mpos);

                            if (tiles[i][j].first.getGlobalBounds().contains(worldPos.y, worldPos.x))
                            {
                                if (gametable[i][j] != 'X' && gametable[i][j] != 'O')
                                {
                                    gametable[i][j] = !Connected ? 'X' : cc;

                                    if (isYourTurn)
                                    {
                                        Packet data;
                                        data << i << j;

                                        NTClient.send(data);
                                        isYourTurn = false;
                                    }
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

        if (!Connected && !isYourTurn)
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

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (gametable[i][j] == '-') hasMovesLeft = true;

                if (i == 0)
                {
                    if (gametable[0][j] == 'O' && gametable[1][j] == 'O' && gametable[2][j] == 'O')
                    {
                        std::cout << "Player lost." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                gametable[i][j] = '-';
                            }
                        }
                    }

                    if (gametable[0][j] == 'X' && gametable[1][j] == 'X' && gametable[2][j] == 'X')
                    {
                        std::cout << "Player win." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                gametable[i][j] = '-';
                            }
                        }
                    }
                }

                if (gametable[0][2] == 'O' && gametable[1][1] == 'O' && gametable[2][0] == 'O')
                {
                    std::cout << "Player lost." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            gametable[i][j] = '-';
                        }
                    }
                }

                if (gametable[0][0] == 'O' && gametable[1][1] == 'O' && gametable[2][2] == 'O')
                {
                    std::cout << "Player lost." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            gametable[i][j] = '-';
                        }
                    }
                }

                if (gametable[0][2] == 'X' && gametable[1][1] == 'X' && gametable[2][0] == 'X')
                {
                    std::cout << "Player win." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            gametable[i][j] = '-';
                        }
                    }
                }

                if (gametable[0][0] == 'X' && gametable[1][1] == 'X' && gametable[2][2] == 'X')
                {
                    std::cout << "Player win." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            gametable[i][j] = '-';
                        }
                    }
                }

                if (j == 0)
                {
                    if (gametable[i][0] == 'O' && gametable[i][1] == 'O' && gametable[i][2] == 'O')
                    {
                        std::cout << "Player lost." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                gametable[i][j] = '-';
                            }
                        }
                    }

                    if (gametable[i][0] == 'X' && gametable[i][1] == 'X' && gametable[i][2] == 'X')
                    {
                        std::cout << "Player win." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                gametable[i][j] = '-';
                            }
                        }
                    }
                }
            }
        }

        if (!hasMovesLeft)
        {
            std::cout << "It's a tie." << std::endl;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    gametable[i][j] = '-';
                }
            }
        }

        window.display();
    }

    NTClient.disconnect();
    NTMaster.disconnect();
    NTListener.close();
    return EXIT_SUCCESS;
}
