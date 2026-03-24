#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"
#include <iostream>
using namespace std;
bool test()
{
    // Test board 2D container
    Candy c(CandyType::TYPE_ORANGE);
    Board b(10, 10);
    b.setCell(&c, 0, 0);
    if (b.getCell(0, 0) != &c)
    {
        return false;
    }

    // Dump and load board
    {
        Board b2(10, 10);
        if (!b.dump(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (!b2.load(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (b2.getCell(0, 0)->getType() != c.getType())
        {
            return false;
        }
        std::filesystem::remove(getDataDirPath() + "dump_board.txt");
    }

    // Dump and load game
    {
        Game g;
        Controller cont;
        g.update(cont);
        if (!g.dump(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        Game g2;
        if (!g2.load(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        if (g != g2)
        {
            return false;
        }
        std::filesystem::remove(getDataDirPath() + "dump_game.txt");
    }
    //return true;



    //test personalizats
    bool allTestsPass = true;
    {
        Board b1;
        if (b1.getWidth() != DEFAULT_BOARD_WIDTH || b1.getHeight() != DEFAULT_BOARD_HEIGHT)
        {
            cout << "Fallo test constructor" << endl;
            allTestsPass = false;
        }
        Board b2(8, 12);
        if (b2.getWidth() != 8 || b2.getHeight() != 12)
        {
            allTestsPass = false;
            cout << "Fallo test constructor parametros" << endl;
        }
        Board b3(100, 100);
        if (b3.getWidth() != DEFAULT_BOARD_WIDTH || b3.getHeight() != DEFAULT_BOARD_HEIGHT)
        {
            cout << "Fallo test constructor parametros limites maximos" << endl;
            allTestsPass = false;
        }
        for (int x = 0; x < b1.getWidth(); x++)
        {
            for (int y = 0; y < b1.getHeight(); y++)
            {
                if (b1.getCell(x, y) != nullptr)
                {
                    allTestsPass = false;
                    break;
                }
            }
        }
    }
    {
        Board b;
        string FalseFile = getDataDirPath() + "ficherofalso.txt";
        bool result = b.load(FalseFile);
        if (result)
        {
            cout << "Reotrna true quant el fitxer no existeix";
            allTestsPass = false;
        }
    }
    {
        vector<Candy*> candies;
        Board b(10, 10);

        // un solo caramelo no deberia explotar
        Candy* single = new Candy(CandyType::TYPE_RED);
        candies.push_back(single);
        b.setCell(single, 5, 5);

        if (b.shouldExplode(5, 5))
        {
            cout << "ha explotado con solo un caramelo" << endl;
            allTestsPass = false;;
        }
        //2 caramelos no deberia explotar
        Candy* two1 = new Candy(CandyType::TYPE_BLUE);
        Candy* two2 = new Candy(CandyType::TYPE_BLUE);
        candies.push_back(two1);
        candies.push_back(two2);
        b.setCell(two1, 2, 3);
        b.setCell(two2, 3, 3);

        if (b.shouldExplode(2, 3) || b.shouldExplode(3, 3))
        {
            cout << "ha explotat con 2 caramelos" << endl;
            allTestsPass = false;
        }
        b.setCell(nullptr, 2, 3);
        b.setCell(nullptr, 3, 3);
    }
    if (allTestsPass)
        std::cout << "Passat tots els test";
    return allTestsPass;

}
