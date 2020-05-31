#include "Line.h"

Line::Line(int maxSize) : Line(maxSize, 0, NOTILE)
{
}

Line::Line(int maxSize, int numTiles, TileType tileType) : maxSize(maxSize), numTiles(numTiles), tileType(tileType)
{
}

int Line::addTiles(int quantity, TileType tileType)
{
    int overflow = 0;
    if (canAddTiles(tileType))
    {
        this->tileType = tileType;
        numTiles += quantity;
        if (numTiles >= maxSize)
        {
            overflow = numTiles - maxSize;
            numTiles = maxSize;
        }
    }
    return overflow;
}

void Line::removeTiles()
{
    numTiles = 0;
    tileType = NOTILE;
}

TileType Line::getTileType()
{
    return tileType;
}

int Line::getNumTiles()
{
    return numTiles;
}

int Line::getMaxSize()
{
    return maxSize;
}

std::string Line::toString()
{
    std::string result = "";
    for (int i = 0; i < maxSize - numTiles; ++i)
    {
        result += char(NOTILE);
    }
    for (int i = 0; i < numTiles; ++i)
    {
        result += char(tileType);
    }
    return result;
}

bool Line::canAddTiles(TileType tileType)
{
    return  tileType != FIRSTPLAYER && tileType != NOTILE 
        && (this->tileType == NOTILE || this->tileType == tileType)
        && numTiles < maxSize;
}
