#include "Utils.h"

TileType charToTileType(char c)
{
    TileType result = NOTILE;
    if (c == 'R')
        result = RED;
    else if (c == 'Y')
        result = YELLOW;
    else if (c == 'O')
        result = ORANGE;
    else if (c == 'B')
        result = DARKBLUE;
    else if (c == 'L')
        result = LIGTHBLUE;
    else if (c == 'U')
        result = BLACK;
    else if (c == 'F')
        result = FIRSTPLAYER;
    else if (c == '.')
        result = NOTILE;
    return result;
}

bool selectableTile(char c)
{
    bool selectable = false;
    selectable |= c == 'R';
    selectable |= c == 'Y';
    selectable |= c == 'O';
    selectable |= c == 'B';
    selectable |= c == 'L';
    selectable |= c == 'U';
    return selectable;
}

bool selectableTile(TileType tile)
{
    return tile != NOTILE && tile != FIRSTPLAYER;
}

bool validTile(char c)
{
    bool validTile = false;
    validTile |= c == 'R';
    validTile |= c == 'Y';
    validTile |= c == 'O';
    validTile |= c == 'B';
    validTile |= c == 'L';
    validTile |= c == 'U';
    validTile |= c == 'F';
    validTile |= c == '.';
    return validTile;
}

char tileTypeToLower(TileType tileType)
{
    char result = '\0';
    if (tileType == RED)
        result = 'r';
    else if (tileType == YELLOW)
        result = 'y';
    else if (tileType == ORANGE)
        result = 'o';
    else if (tileType == DARKBLUE)
        result = 'b';
    else if (tileType == LIGTHBLUE)
        result = 'l';
    else if (tileType == BLACK)
        result = 'u';
    else
        result = char(tileType);
    return result;
}

bool isNotWhiteSpace(const std::string str)
{
    bool isWhiteSpace = true;
    std::istringstream ss(str);
    char c = '\0';
    while (isWhiteSpace && ss.get(c))
    {
        isWhiteSpace = std::isspace(c);
    }
    return !isWhiteSpace;
}

bool isWhiteSpace(const std::string str)
{
    return !isNotWhiteSpace(str);
}
