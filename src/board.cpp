#include <iostream>
#include <fstream>
#include "board.h"

using namespace std;

Board::Board(int width, int height)
{
    // Validate dimensions
    if (width <= 0 || width > MAX_SIZE)
        width = DEFAULT_BOARD_WIDTH;
    if (height <= 0 || height > MAX_SIZE)
        height = DEFAULT_BOARD_HEIGHT;

    m_width = width;
    m_height = height;

    // Initialize all matrices to nullptr
    for (int y = 0; y < MAX_SIZE; y++)
    {
        for (int x = 0; x < MAX_SIZE; x++)
        {
            m_storage[x][y] = nullptr;
            m_board[x][y] = nullptr;
        }
    }
}

Board::~Board()
{
    // Free dynamically created Candy objects
    for (int y = 0; y < MAX_SIZE; y++)
    {
        for (int x = 0; x < MAX_SIZE; x++)
        {
            if (m_storage[x][y] != nullptr)
            {
                delete m_storage[x][y];
                m_storage[x][y] = nullptr;
            }
        }
    }
}

Candy* Board::getCell(int x, int y) const
{
    // Check boundaries
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        return m_board[x][y];
    }
    return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        m_board[x][y] = candy;
    }
}

int Board::getWidth() const
{
    return m_width;
}

int Board::getHeight() const
{
    return m_height;
}

bool Board::shouldExplode(int x, int y) const
{
    Candy* candy = getCell(x, y);
    if (candy == nullptr)
        return false;

    CandyType type = candy->getType();

    // Directions to check: (dx, dy)
    // 0: horizontal (left and right)
    // 1: vertical (up and down)
    // 2: main diagonal (down-right and up-left)
    // 3: secondary diagonal (up-right and down-left)

    int directions[4][2][2] = {
        {{1, 0}, {-1, 0}},   // horizontal
        {{0, 1}, {0, -1}},   // vertical
        {{1, 1}, {-1, -1}},  // main diagonal
        {{1, -1}, {-1, 1}}   // secondary diagonal
    };

    for (int d = 0; d < 4; d++)
    {
        int count = 1;  // Start with current piece

        // Check in both directions
        for (int dir = 0; dir < 2; dir++)
        {
            int dx = directions[d][dir][0];
            int dy = directions[d][dir][1];
            int step = 1;

            while (true)
            {
                int newX = x + dx * step;
                int newY = y + dy * step;

                Candy* other = getCell(newX, newY);
                if (other == nullptr)
                    break;
                if (other->getType() != type)
                    break;

                count++;
                step++;
            }
        }

        if (count >= SHORTEST_EXPLOSION_LINE)
            return true;
    }

    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    std::vector<Candy*> exploded;
    bool hasExplosions;

    do {
        hasExplosions = false;

        // 1. Mark positions that should explode
        bool toExplode[MAX_SIZE][MAX_SIZE] = { false };

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                if (shouldExplode(x, y))
                {
                    toExplode[x][y] = true;
                    hasExplosions = true;
                }
            }
        }

        // 2. Explode marked positions
        if (hasExplosions)
        {
            for (int y = 0; y < m_height; y++)
            {
                for (int x = 0; x < m_width; x++)
                {
                    if (toExplode[x][y] && m_board[x][y] != nullptr)
                    {
                        exploded.push_back(m_board[x][y]);
                        m_board[x][y] = nullptr;
                    }
                }
            }

            // 3. Drop pieces by column
            for (int x = 0; x < m_width; x++)
            {
                int writeY = m_height - 1;

                for (int y = m_height - 1; y >= 0; y--)
                {
                    if (m_board[x][y] != nullptr)
                    {
                        if (y != writeY)
                        {
                            m_board[x][writeY] = m_board[x][y];
                            m_board[x][y] = nullptr;
                        }
                        writeY--;
                    }
                }
            }
        }

    } while (hasExplosions);

    return exploded;
}

bool Board::dump(const std::string& output_path) const
{
    ofstream fitxer(output_path);
    if (!fitxer.is_open())
        return false;

    // Write dimensions
    fitxer << m_width << " " << m_height << endl;

    // Write content
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            if (m_board[x][y] != nullptr)
            {
                fitxer << static_cast<int>(m_board[x][y]->getType());
            }
            else
            {
                fitxer << -1;
            }

            if (x < m_width - 1)
                fitxer << " ";
        }
        fitxer << endl;
    }

    return true;
}

bool Board::load(const std::string& input_path)
{
    ifstream fitxer(input_path);
    if (!fitxer.is_open())
        return false;

    int width, height;
    fitxer >> width >> height;

    // Validate dimensions
    if (width <= 0 || width > MAX_SIZE || height <= 0 || height > MAX_SIZE)
        return false;

    m_width = width;
    m_height = height;

    // Initialize all matrices to nullptr (free previous memory)
    for (int y = 0; y < MAX_SIZE; y++)
    {
        for (int x = 0; x < MAX_SIZE; x++)
        {
            if (m_storage[x][y] != nullptr)
            {
                delete m_storage[x][y];
                m_storage[x][y] = nullptr;
            }
            m_board[x][y] = nullptr;
        }
    }

    // Read content
    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int tipus;
            fitxer >> tipus;

            if (tipus != -1)
            {
                // Verify type is valid
                if (tipus >= 0 && tipus < static_cast<int>(CandyType::COUNT))
                {
                    m_storage[x][y] = new Candy(static_cast<CandyType>(tipus));
                    m_board[x][y] = m_storage[x][y];
                }
            }
        }
    }

    return true;
}
