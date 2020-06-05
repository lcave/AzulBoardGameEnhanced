#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "Mosaic.h"

class Menu;

class Player
{
public:
    //New player
    Player(std::string name, bool sixtiles, bool grayboard);
    //Player from save
    Player(std::string name, int score, Mosaic *mosaic);
    ~Player();
    //Return name
    std::string getName();
    //Return score
    int getScore();
    //Move tiles to wall, calc score, return tiles for lid
    std::vector<TileType> calcScore(bool grayboard, Menu &menu);
    //Return pointer to mosaic
    Mosaic *getMosaic();
    //Check whether horizontal row in wall is full
    bool hasWon(bool grayboard);
    //Check whether a player has the FP tile
    bool hasFirstPlayer();

private:
    //Helper to score single line
    int scoreLine(int lineNum, bool grayboard, Menu &menu);
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
    TileType master_wall_6[NUMBER_OF_LINES + 1][NUMBER_OF_LINES + 1] = {DARKBLUE, YELLOW, ORANGE, RED, BLACK, LIGTHBLUE,
                                                                        LIGTHBLUE, DARKBLUE, YELLOW, ORANGE, RED, BLACK,
                                                                        BLACK, LIGTHBLUE, DARKBLUE, YELLOW, ORANGE, RED,
                                                                        RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW, ORANGE,
                                                                        ORANGE, RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                                                                        YELLOW, ORANGE, RED, BLACK, LIGTHBLUE, DARKBLUE};
};

#endif // !PLAYER_H
