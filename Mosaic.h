#ifndef MOSIAC_H
#define MOSIAC_H

#include "Types.h"
#include "Factory.h"
#include "Line.h"
#include "TileList.h"

class Mosaic
{
public:
    Mosaic();
    ~Mosaic();
    //Check if a position in wall is filled
    bool isFilled(int row, int col);
    //Check if a wall row contains a tile of a certain colour
    bool isFilled(int row, TileType tile);
    //Set a position on the wall to given bool
    void setFilled(int row, int col, bool filled);
    //Insert tiles into a pattern line
    void insertTilesIntoLine(int row, int quantity, TileType tileType);
    //Insert tiles into broken line
    void addToBrokenTiles(int quantity, TileType tileType);
    //Return a pointer to a line
    Line *getLine(int line);
    //Return a pointer to a line in the wall
    bool *getWallLine(int line);
    //Return a pointer to broken tiles
    TileList *getBrokenTiles();

private:
    //Player's pattern lines
    Line *lines[NUMBER_OF_LINES];
    //2D bool array to store whther a tile is present at index
    bool wall[NUMBER_OF_LINES][NUMBER_OF_LINES];
    //Player's broken tiles
    TileList *brokenTiles;
    //Master pattern wall for comparison with player's bool wall
    TileType master_wall[NUMBER_OF_LINES][NUMBER_OF_LINES] = {DARKBLUE, YELLOW, RED, BLACK, LIGTHBLUE,
                                                              LIGTHBLUE, DARKBLUE, YELLOW, RED, BLACK,
                                                              BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED,
                                                              RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                                                              YELLOW, RED, BLACK, LIGTHBLUE, DARKBLUE};
};

#endif
