#include "Mosaic.h"

Mosaic::Mosaic(bool sixtiles, bool graywall)
{
    this->grayboard = graywall;
    numLines = NUMBER_OF_LINES;
    if (sixtiles)
    {
        ++numLines;
    }

    for (int i = 0; i < numLines; ++i)
    {
        lines.push_back(*new Line(i + 1));
    }

    if (!graywall)
    {
        for (int i = 0; i < numLines; ++i)
        {
            std::vector<bool> line;
            for (int j = 0; j < numLines; ++j)
            {
                line.push_back(false);
            }
            wall.push_back(line);
        }
    }
    else
    {
        for (int i = 0; i < numLines; ++i)
        {
            std::vector<TileType> line;
            for (int j = 0; j < numLines; ++j)
            {
                line.push_back(NOTILE);
            }
            this->graywall.push_back(line);
        }
    }
    brokenTiles = new TileList();
}

Mosaic::~Mosaic()
{
    lines.clear();
    delete brokenTiles;
}

bool Mosaic::isFilled(int row, int col)
{
    bool filled = false;
    if (row < numLines && col < numLines)
    {
        if (grayboard)
        {
            if (graywall[row][col] != NOTILE)
            {
                filled = true;
            }
        }
        else
        {
            filled = wall[row][col];
        }
    }
    return filled;
}

TileType Mosaic::isFilled(int row, int col, bool graywall)
{
    return this->graywall[row][col];
}

bool Mosaic::isFilled(int row, TileType tile)
{
    bool filled = false;
    for (int i = 0; i < numLines; i++)
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

void Mosaic::setFilled(int row, int col, TileType type)
{
    graywall[row][col] = type;
}

void Mosaic::insertTilesIntoLine(int row, int quantity, TileType tileType)
{
    if (row == 6)
    {
        addToBrokenTiles(quantity, tileType);
    }
    else
    {
        int overflow = lines[row].addTiles(quantity, tileType);
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
    return &lines[line];
}

std::vector<bool> Mosaic::getWallLine(int line)
{
    return wall[line];
}

std::vector<TileType> Mosaic::getWallLine(int line, bool graywall)
{
    return this->graywall[line];
}

TileList *Mosaic::getBrokenTiles()
{
    return brokenTiles;
}

int Mosaic::getNumLines()
{
    return numLines;
}
