#ifndef UTILS_H
#define UTILS_H

#include "Types.h"
#include <string>
#include <sstream>

TileType charToTileType(char c);
bool selectableTile(char c);
bool selectableTile(TileType tile);
bool validTile(char c);
char tileTypeToLower(TileType tileType);
bool isNotWhiteSpace(const std::string str);
bool isWhiteSpace(const std::string str);


#endif