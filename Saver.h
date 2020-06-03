#ifndef SAVER_H
#define SAVER_H

#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include "GameEngine.h"
#include "Menu.h"
#include "Factory.h"
#include "Utils.h"
#include "PlayerTree.h"

using std::string;

class Saver
{
public:
    void save(GameEngine *gameEngine, std::string fileName);
    GameEngine *load(std::string fileName, Menu *menu);
    GameEngine *replay(std::string fileName, Menu *menu);

private:
    void save(GameEngine *gameEngine, std::ofstream &outputStream);
    GameEngine *load(std::istream &inputStream, Menu *menu);
    void outputWall(std::ofstream &outputStream, Mosaic *mosaic);
    Mosaic *generateMosiac(std::string lines[SAVE_FILE_LINES_LENGTH], int startingLine, bool &addedFirstTile, int numberOfEachTile[]);
    void cleanUpFactories(Factory *factories[]);
    bool isValidFactory(TileType tiles[]);
    int getTileIndex(char c);
    int calculateSumOfTiles(int numberOfEachTile[]);
};

#endif
