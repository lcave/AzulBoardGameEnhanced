#ifndef MOSIAC_H
#define MOSIAC_H

#include "Types.h"
#include "Factory.h"
#include "Line.h"
#include "TileList.h"
#include <array>

class Mosaic
{
public:
    Mosaic(bool sixtiles, bool graywall);
    ~Mosaic();
    //Check if a position in wall is filled
    bool isFilled(int row, int col);
    TileType isFilled(int row, int col, bool graywall);
    //Check if a wall row contains a tile of a certain colour
    bool isFilled(int row, TileType tile);
    //Set a position on the wall to given bool
    void setFilled(int row, int col, bool filled);
    //Add tile to gray wall
    void setFilled(int row, int col, TileType type);
    //Insert tiles into a pattern line
    void insertTilesIntoLine(int row, int quantity, TileType tileType);
    //Insert tiles into broken line
    void addToBrokenTiles(int quantity, TileType tileType);
    //Return a pointer to a line
    Line *getLine(int line);
    //Return a pointer to a line in the wall
    std::vector<bool> getWallLine(int line);
    std::vector<TileType> getWallLine(int line, bool graywall);
    //Return a pointer to broken tiles
    TileList *getBrokenTiles();

    int getNumLines();

private:
    bool grayboard;
    int numLines;
    //Player's pattern lines
    std::vector<Line> lines;
    //2D bool array to store whther a tile is present at index
    std::vector<std::vector<bool>> wall;
    std::vector<std::vector<TileType>> graywall;
    //Player's broken tiles
    TileList *brokenTiles;
    //Master pattern wall for comparison with player's bool wall
    TileType master_wall[NUMBER_OF_LINES][NUMBER_OF_LINES] = {DARKBLUE, YELLOW, RED, BLACK, LIGTHBLUE,
                                                              LIGTHBLUE, DARKBLUE, YELLOW, RED, BLACK,
                                                              BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED,
                                                              RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                                                              YELLOW, RED, BLACK, LIGTHBLUE, DARKBLUE};
    TileType master_wall_6[NUMBER_OF_LINES + 1][NUMBER_OF_LINES + 1] = {DARKBLUE, YELLOW, ORANGE, RED, BLACK, LIGTHBLUE,
                                                                        LIGTHBLUE, DARKBLUE, YELLOW, ORANGE, RED, BLACK,
                                                                        BLACK, LIGTHBLUE, DARKBLUE, YELLOW, ORANGE, RED,
                                                                        RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW, ORANGE,
                                                                        ORANGE, RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                                                                        YELLOW, ORANGE, RED, BLACK, LIGTHBLUE, DARKBLUE};
};

#endif
