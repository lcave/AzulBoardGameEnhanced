#include "Mosaic.h"

Mosaic::Mosaic()
{
    for (int i = 0; i < NUMBER_OF_LINES; ++i)
    {
        lines[i] = new Line(i + 1);
    }

    for (int i = 0; i < NUMBER_OF_LINES; ++i)
    {
        for (int j = 0; j < NUMBER_OF_LINES; ++j)
        {
            wall[i][j] = false;
        }
    }
    brokenTiles = new TileList();
}

Mosaic::~Mosaic()
{
    for (int i = 0; i < NUMBER_OF_LINES; ++i)
    {
        delete lines[i];
    }
    delete brokenTiles;
}

bool Mosaic::isFilled(int row, int col)
{
    return wall[row][col];
}

bool Mosaic::isFilled(int row, TileType tile)
{
    bool filled = false;
    for (size_t i = 0; i < NUMBER_OF_LINES; i++)
    {
        if (master_wall[row][i] == tile)
        {
            if (getWallLine(row)[i])
            {
                filled = true;
            }
        }
    }
    return filled;
}

void Mosaic::setFilled(int row, int col, bool filled)
{
    wall[row][col] = filled;
}

void Mosaic::insertTilesIntoLine(int row, int quantity, TileType tileType)
{
    if (row == 6)
    {
        addToBrokenTiles(quantity, tileType);
    }
    else
    {
        int overflow = lines[row]->addTiles(quantity, tileType);
        addToBrokenTiles(overflow, tileType);
    }
}

void Mosaic::addToBrokenTiles(int quantity, TileType tileType)
{
    if (tileType != NOTILE)
    {
        for (int i = 0; i < quantity; ++i)
            brokenTiles->addBack(tileType);
    }
}

Line *Mosaic::getLine(int line)
{
    return lines[line];
}

bool *Mosaic::getWallLine(int line)
{
    return wall[line];
}

TileList *Mosaic::getBrokenTiles()
{
    return brokenTiles;
}
