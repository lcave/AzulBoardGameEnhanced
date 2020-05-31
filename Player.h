#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Mosaic.h"

class Player
{
public:
    //New player
    Player(std::string name);
    //Player from save
    Player(std::string name, int score, Mosaic *mosaic);
    ~Player();
    //Return name
    std::string getName();
    //Return score
    int getScore();
    //Move tiles to wall, calc score, return tiles for lid
    std::vector<TileType> calcScore();
    //Return pointer to mosaic
    Mosaic *getMosaic();
    //Check whether horizontal row in wall is full
    bool hasWon();
    //Check whether a player has the FP tile
    bool hasFirstPlayer();

private:
    //Helper to score single line
    int scoreLine(int lineNum);
    //Calculate score for a row
    int calcRow(int index, int line);
    //Calculate score for a column
    int calcCol(int index, int line);
    std::string name;
    int score;
    Mosaic *mosaic;
    TileType master_wall[NUMBER_OF_LINES][NUMBER_OF_LINES] = {DARKBLUE, YELLOW, RED, BLACK, LIGTHBLUE,
                                                              LIGTHBLUE, DARKBLUE, YELLOW, RED, BLACK,
                                                              BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED,
                                                              RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                                                              YELLOW, RED, BLACK, LIGTHBLUE, DARKBLUE};
};

#endif // !PLAYER_H
