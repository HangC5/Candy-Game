#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "candy.h"

const int DEFAULT_BOARD_WIDTH = 10;
const int DEFAULT_BOARD_HEIGHT = 10;
const int SHORTEST_EXPLOSION_LINE = 3;
const int MAX_SIZE = 20;

class Board
{
public:
    Board(int width = DEFAULT_BOARD_WIDTH, int height = DEFAULT_BOARD_HEIGHT);
    ~Board();

    bool shouldExplode(int x, int y) const;
    std::vector<Candy*> explodeAndDrop();
    bool dump(const std::string& output_path) const;
    bool load(const std::string& input_path);
    Candy* getCell(int x, int y) const;
    void setCell(Candy* candy, int x, int y);
    int getWidth() const;
    int getHeight() const;

private:
    int m_width;
    int m_height;
    Candy* m_storage[MAX_SIZE][MAX_SIZE];
    Candy* m_board[MAX_SIZE][MAX_SIZE];
};

#endif
