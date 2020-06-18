#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <bitset>

using namespace std;
using namespace sf;

constexpr Uint16 BOARD_COLS = 3U;
constexpr Uint16 BOARD_ROWS = 3U;
constexpr Uint16 BOARD = BOARD_COLS * BOARD_ROWS;

constexpr Uint16 SCREEN_WIDTH = 450U;
constexpr Uint16 SCREEN_HEIGHT = 450U;

constexpr Uint16 NETWORK_ENABLED = 2U;
constexpr Uint16 NETWORK_PORT = 2000U;

bool isConnected(TcpSocket& socket)
{
    return socket.getRemoteAddress() != IpAddress::None;
}

int main(int argc, char* argv[])
{
    bitset<8U> OQudVjuuif;

    TcpListener NTListener;
    TcpSocket NTMaster;
    TcpSocket NTClient;
    Packet NTRPacket;

    unsigned char cc = '-';

    bool assignedTurn = false;

    bool isYourTurn = false;

    for (signed i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--server") == 0)
        {
            if (NTListener.listen(NETWORK_PORT) != Socket::Done)
            {
                cerr << "Error binding to a port!" << endl;
                return EXIT_FAILURE;
            }

            while (true)
            {
                if (isConnected(NTMaster) == false)
                {
                    if (NTListener.accept(NTMaster) == Socket::Done)
                    {
                        Packet data;
                        data << true;
                        NTMaster.send(data);
                    }
                    else
                    {
                        cerr << "Error accepting master socket!" << endl;
                        return EXIT_FAILURE;
                    }
                }
                else
                {
                    if (NTMaster.receive(NTRPacket) == Socket::Done)
                    {
                        cout << "Received data from a master socket." << endl;
                        cout << "Redirecting data back to a client socket." << endl;
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
            if (NTClient.connect(IpAddress(argv[i + 1]), NETWORK_PORT) != Socket::Done)
            {
                cerr << "Could not connect to server!" << endl;
                return EXIT_FAILURE;
            }

            OQudVjuuif.set(NETWORK_ENABLED, true);
        }
    }

    RenderWindow fWbaSuIYYf(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Tic-tac-toe");

    static Vector2f LdmxiSsUGp = Vector2f(SCREEN_WIDTH / BOARD_COLS, SCREEN_HEIGHT / BOARD_ROWS);

    static RectangleShape PBgBBDIgQo[BOARD_COLS][BOARD_ROWS];

    static Uint8 QHOtohjnnl[BOARD_COLS][BOARD_ROWS]
    {
        '-', '-', '-',
        '-', '-', '-',
        '-', '-', '-'
    };

    static float XlBwVFADsC[BOARD_COLS & BOARD_ROWS] =
    {
        15.0F,
        172.5F,
        322.5F
    };

    for (signed i = 0; i < 3; i++)
    {
        for (signed j = 0; j < 3; j++)
        {
            PBgBBDIgQo[j][i] = RectangleShape(LdmxiSsUGp);
            PBgBBDIgQo[j][i].setPosition(i * LdmxiSsUGp.x, j * LdmxiSsUGp.y);
            PBgBBDIgQo[j][i].setOutlineColor(Color::Black);
            PBgBBDIgQo[j][i].setOutlineThickness(3);
        }
    }

    while (fWbaSuIYYf.isOpen() == true)
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
                        QHOtohjnnl[x][y] = (cc == 'X' ? 'O' : 'X');
                        cout << "Received an update to board." << endl;
                        isYourTurn = true;
                    }
                }
            }
        }

        static Event FskESoWlRD;

        while (fWbaSuIYYf.pollEvent(FskESoWlRD))
        {
            switch (FskESoWlRD.type)
            {
            case Event::Closed:
            {
                fWbaSuIYYf.close();
            }
            case Event::MouseButtonPressed:
                if (FskESoWlRD.mouseButton.button == Mouse::Left)
                {
                    for (Uint8 i = 0; i < 3u; i++)
                    {
                        for (Uint8 j = 0; j < 3u; j++)
                        {
                            if (!isYourTurn)
                                continue;

                            const Vector2i SOIUzwyXov = Mouse::getPosition(fWbaSuIYYf);
                            const Vector2f BgWypoRaaU = fWbaSuIYYf.mapPixelToCoords(SOIUzwyXov);

                            if (PBgBBDIgQo[i][j].getGlobalBounds().contains(BgWypoRaaU.y, BgWypoRaaU.x) == true)
                            {
                                if (QHOtohjnnl[i][j] != 'X' && QHOtohjnnl[i][j] != 'O')
                                {
                                    isYourTurn = false;

                                    QHOtohjnnl[i][j] = (OQudVjuuif.test(NETWORK_ENABLED) ? cc : 'X');

                                    if (OQudVjuuif.test(NETWORK_ENABLED) == true)
                                    {
                                        Packet mMYQdDNBsD;
                                        mMYQdDNBsD << i << j;
                                        NTClient.send(mMYQdDNBsD);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            }
        }

        fWbaSuIYYf.clear();

        for (unsigned i = 0u; i < 3u; i++)
        {
            for (unsigned j = 0u; j < 3u; j++)
            {
                fWbaSuIYYf.draw(PBgBBDIgQo[i][j]);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (QHOtohjnnl[i][j] == 'X')
                {
                    RectangleShape shape(Vector2f(150.0F, 5.0F));

                    shape.setFillColor(Color::Black);

                    shape.setPosition(XlBwVFADsC[i], XlBwVFADsC[j]);
                    shape.rotate(45.0F);
                    fWbaSuIYYf.draw(shape);
                    shape.rotate(-90.0F);
                    shape.setPosition(XlBwVFADsC[i], XlBwVFADsC[j] + 105.0F);
                    fWbaSuIYYf.draw(shape);
                }
                if (QHOtohjnnl[i][j] == 'O')
                {
                    sf::CircleShape shape(60.0F, 100U);
                    shape.setOutlineColor(sf::Color::Black);
                    shape.setFillColor(sf::Color::Transparent);
                    shape.setPosition(XlBwVFADsC[i] - 5.0F, XlBwVFADsC[j] - 5.0F);
                    shape.setOutlineThickness(5.0F);
                    fWbaSuIYYf.draw(shape);
                }
            }
        }

        if (!OQudVjuuif.test(NETWORK_ENABLED) && !isYourTurn)
        {
            int nx = rand() % 3;
            int ny = rand() % 3;

            if (QHOtohjnnl[nx][ny] == '-')
            {
                QHOtohjnnl[nx][ny] = 'O';

                if (QHOtohjnnl[nx - 1][ny] == 'O' && QHOtohjnnl[nx + 1][ny] == 'O')
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
                if (QHOtohjnnl[i][j] == '-') hasMovesLeft = true;

                if (i == 0)
                {
                    if (QHOtohjnnl[0][j] == 'O' && QHOtohjnnl[1][j] == 'O' && QHOtohjnnl[2][j] == 'O')
                    {
                        std::cout << "Player lost." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                QHOtohjnnl[i][j] = '-';
                            }
                        }
                    }

                    if (QHOtohjnnl[0][j] == 'X' && QHOtohjnnl[1][j] == 'X' && QHOtohjnnl[2][j] == 'X')
                    {
                        std::cout << "Player win." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                QHOtohjnnl[i][j] = '-';
                            }
                        }
                    }
                }

                // DIAG. CHECK
                if (QHOtohjnnl[0][2] == 'O' && QHOtohjnnl[1][1] == 'O' && QHOtohjnnl[2][0] == 'O')
                {
                    std::cout << "Player lost." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            QHOtohjnnl[i][j] = '-';
                        }
                    }
                }

                if (QHOtohjnnl[0][0] == 'O' && QHOtohjnnl[1][1] == 'O' && QHOtohjnnl[2][2] == 'O')
                {
                    std::cout << "Player lost." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            QHOtohjnnl[i][j] = '-';
                        }
                    }
                }

                if (QHOtohjnnl[0][2] == 'X' && QHOtohjnnl[1][1] == 'X' && QHOtohjnnl[2][0] == 'X')
                {
                    std::cout << "Player win." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            QHOtohjnnl[i][j] = '-';
                        }
                    }
                }

                if (QHOtohjnnl[0][0] == 'X' && QHOtohjnnl[1][1] == 'X' && QHOtohjnnl[2][2] == 'X')
                {
                    std::cout << "Player win." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            QHOtohjnnl[i][j] = '-';
                        }
                    }
                }

                if (j == 0)
                {
                    if (QHOtohjnnl[i][0] == 'O' && QHOtohjnnl[i][1] == 'O' && QHOtohjnnl[i][2] == 'O')
                    {
                        std::cout << "Player lost." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                QHOtohjnnl[i][j] = '-';
                            }
                        }
                    }

                    if (QHOtohjnnl[i][0] == 'X' && QHOtohjnnl[i][1] == 'X' && QHOtohjnnl[i][2] == 'X')
                    {
                        std::cout << "Player win." << std::endl;
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                QHOtohjnnl[i][j] = '-';
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
                    QHOtohjnnl[i][j] = '-';
                }
            }
        }

        fWbaSuIYYf.display();
    }

    NTClient.disconnect();
    NTMaster.disconnect();
    NTListener.close();
    return EXIT_SUCCESS;
}
