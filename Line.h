#ifndef LINE_H
#define LINE_H

#include "TileList.h"
#include "Types.h"
#include <string>

class Line
{
public:
    //Constructor with maximum size of line
    Line(int maxSize);
    //Constructor with line contents
    Line(int maxSize, int numTiles, TileType tileType);
    //Add a quanitity of tiles of a colour
    int addTiles(int quantity, TileType tileType);
    //Reset contents
    void removeTiles();
    //Get the colour of tiles in the line
    TileType getTileType();
    //Get num of tiles in line
    int getNumTiles();
    //Get max size of line
    int getMaxSize();
    //Return line contents as string
    std::string toString();
    //Check whether tiles can be added to line
    bool canAddTiles(TileType tileType);

private:
    int maxSize;
    int numTiles;
    TileType tileType;
};

#endif