#include "board.h"
#include <memory>
#include <iostream>

Board::Board(int width, int height) 
{   
    m_width = width;
    m_height = height;
    // Implement your code here
    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            m_board[i][j] = nullptr;    //inicialitzar els apuntador de la matriu en nullptr
        }
    }
}

Board::~Board()
{
    // Implement your code here
}


Candy* Board::getCell(int x, int y) const
{
    // Implement your code here
    //comprobamos que la posicion de la matriz que nos pida no se este fuera de los limites de la matriz
    //si esta fuera devolvemos nullptr pq no existe y en otro caso devolvemos el puntero.
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        return m_board[x][y];
    }
    return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
    // Implement your code here
}


int Board::getWidth() const
{
    // Implement your code here
    return m_width;
}


int Board::getHeight() const
{
    // Implement your code here
    return m_height;
}

bool Board::shouldExplode(int x, int y) const
{
    // Implement your code here
    return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
    // Implement your code here
    return {};
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    return false;
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}
