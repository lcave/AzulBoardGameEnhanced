#ifndef FACTORY_H
#define FACTORY_H

#include <string>
#include <vector>
#include "Types.h"

using std::string;

class Factory
{
public:
    //Initial constructor
    Factory();
    //Constructor taking array of tiles from saver
    Factory(TileType array[FACTORY_SIZE]);
    //Add tiles in array to factory
    void fill(TileType array[FACTORY_SIZE]);
    //Remove and return remaining tiles
    std::vector<TileType> empty();
    //Remove specified colour tile and return quantity
    int draw(TileType tileType);
    //Return string of contents
    string toString();
    TileType* getTiles();
    //Return stroing with no whitespace (for saver)
    string toStringNoSpace();
    //Check if factory is empty
    bool isEmpty();
    //Check if factory contains given tile
    bool contains(TileType tileType);

private:
    //Storage of tiles
    TileType tiles[FACTORY_SIZE];
};

#endif // !FACTORY_H