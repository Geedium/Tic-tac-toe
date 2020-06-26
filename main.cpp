#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <bitset>

using namespace std;
using namespace sf;

constexpr Uint16 BOARD_COLS = 3U;
constexpr Uint16 BOARD_ROWS = BOARD_COLS;
constexpr Uint16 BOARD = BOARD_COLS * BOARD_ROWS;

constexpr Uint16 SCREEN_WIDTH = 450U;
constexpr Uint16 SCREEN_HEIGHT = SCREEN_WIDTH;

constexpr Uint16 NETWORK_ENABLED = 2U;
constexpr Uint16 NETWORK_PORT = 2000U;

static signed Weights[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

struct sc
{
    int i;
    int v;
};

bool isLost(Uint8 data[][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 0)
            {
                if (data[0][j] == 'O' && data[1][j] == 'O' && data[2][j] == 'O')
                {
                    std::cout << "Player lost." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            data[i][j] = '-';
                        }
                    }
                    return true;
                }

                if (data[0][j] == 'X' && data[1][j] == 'X' && data[2][j] == 'X')
                {
                    std::cout << "Player win." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            data[i][j] = '-';
                        }
                    }
                    return true;
                }
            }

            if (data[0][2] == 'O' && data[1][1] == 'O' && data[2][0] == 'O')
            {
                std::cout << "Player lost." << std::endl;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        data[i][j] = '-';
                    }
                }
                return true;
            }

            if (data[0][0] == 'O' && data[1][1] == 'O' && data[2][2] == 'O')
            {
                std::cout << "Player lost." << std::endl;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        data[i][j] = '-';
                    }
                }
                return true;
            }

            if (data[0][2] == 'X' && data[1][1] == 'X' && data[2][0] == 'X')
            {
                std::cout << "Player win." << std::endl;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        data[i][j] = '-';
                    }
                }
                return true;
            }

            if (data[0][0] == 'X' && data[1][1] == 'X' && data[2][2] == 'X')
            {
                std::cout << "Player win." << std::endl;
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        data[i][j] = '-';
                    }
                }
                return true;
            }

            if (j == 0)
            {
                if (data[i][0] == 'O' && data[i][1] == 'O' && data[i][2] == 'O')
                {
                    std::cout << "Player lost." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            data[i][j] = '-';
                        }
                    }
                    return true;
                }

                if (data[i][0] == 'X' && data[i][1] == 'X' && data[i][2] == 'X')
                {
                    std::cout << "Player win." << std::endl;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            data[i][j] = '-';
                        }
                    }
                    return true;
                }
            }
        }
    }

    return false;
}

bool isMovesLeft(const Uint8 data[][3])
{
    for (unsigned i{ 0 }; i < BOARD; i++)
    {
        unsigned x = i % BOARD_COLS;
        unsigned y = i / BOARD_ROWS;

        if (data[x][y] == '-')
            return true;
    }
    cout << "It's a tie!" << endl;
    return false;
}

bool isConnected(TcpSocket& socket)
{
    return socket.getRemoteAddress() != IpAddress::None;
}

struct Range
{
    int a;
    int b;
    int c;
};

sc Compute()
{
    Range index{ 0,0,0 };

    int iRs{ 0 };

    int s_y = Weights[0] + Weights[3] + Weights[6];
    int s_y2 = Weights[1] + Weights[4] + Weights[7];
    int s_y3 = Weights[2] + Weights[5] + Weights[8];

    if (s_y > iRs)
    {
        index = { 0, 3, 6 };
        iRs = s_y;
    }

    if (s_y2 > iRs)
    {
        index = { 1, 4, 7 };
        iRs = s_y2;
    }

    if (s_y3 >= iRs)
    {
        index = { 2, 5, 8 };
        iRs = s_y3;
    }

    int s_x = Weights[0] + Weights[1] + Weights[2];
    int s_x2 = Weights[3] + Weights[4] + Weights[5];
    int s_x3 = Weights[6] + Weights[7] + Weights[8];

    if (s_x > iRs)
    {
        index = { 0,1,2 };
        iRs = s_x;
    }

    if (s_x2 > iRs)
    {
        index = { 3,4,5 };
        iRs = s_x2;
    }

    if (s_x3 >= iRs)
    {
        index = { 6,7,8 };
        iRs = s_x3;
    }

    int s_xy = Weights[0] + Weights[4] + Weights[8];

    if (s_xy > iRs)
    {
        index = { 0, 4, 8 };
        iRs = s_xy;
    }

    int s_xy2 = Weights[2] + Weights[4] + Weights[6];

    if (s_xy2 > iRs)
    {
        index = { 2, 4, 6 };
        iRs = s_xy2;
    }

    for (int i = 0; i < 3; i++)
    {
        int n[3] = { index.a, index.b, index.c };
        int g = n[i];

        if (Weights[g] == 0)
        {
            return { g, iRs };
        }
    }

    return { rand() % 9, 0 };
}

int main(int argc, char* argv[])
{
    bitset<8U> OQudVjuuif;

    TcpListener NTListener;
    TcpSocket NTMaster;
    TcpSocket NTClient;
    Packet NTRPacket;

    bool assignedTurn = false;

    for (signed i{ 0 }; i < argc; i++)
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
                        cout << "Master socket connected." << endl;

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

    bool isYourTurn = true;
    unsigned char cc = 'X';
    int origin = 0;

    if (OQudVjuuif.test(NETWORK_ENABLED))
    {
        isYourTurn = false;
        cc = '-';
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

                    Uint8 x, y;
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
                                    QHOtohjnnl[i][j] = (OQudVjuuif.test(NETWORK_ENABLED) ? cc : 'X');

                                    if (OQudVjuuif.test(NETWORK_ENABLED) == true)
                                    {
                                        Packet mMYQdDNBsD;
                                        mMYQdDNBsD << i << j;
                                        NTClient.send(mMYQdDNBsD);
                                    }
                                    else
                                    {
                                        Weights[i + 3 * j] = +1;
                                    }

                                    isYourTurn = false;
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
            sc move = Compute();
            int nx = move.i % 3;
            int ny = move.i / 3;

            if (QHOtohjnnl[nx][ny] == '-')
            {
                QHOtohjnnl[nx][ny] = 'O';
                Weights[nx + 3 * ny] = -1;
                isYourTurn = true;
            }
        }

        if (isLost(QHOtohjnnl) || !isMovesLeft(QHOtohjnnl))
        {
            for (int i = 0; i < 9; i++)
            {
                int x = i % 3;
                int y = i / 3;
                QHOtohjnnl[x][y] = '-';
                Weights[i] = 0;
            }
        }

        fWbaSuIYYf.display();
    }

    NTClient.disconnect();
    NTMaster.disconnect();
    NTListener.close();
    return EXIT_SUCCESS;
}
