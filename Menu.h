#ifndef MENU_H
#define MENU_H

#define RESET "\033[0m"
#define COMMAND "\033[32m"   // Green
#define PARAMETER "\033[33m" // Yellow
#define ALERT "\033[31m"     // Red

#define BULLET "\u2022"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Menu.h"
#include "PlayerTree.h"
#include "PlayerNode.h"

class PlayerTree;
class PlayerNode;
class Player;

class Menu
{
public:
    //Print main menu
    void printMenu();
    //Get input with getLine
    std::string getInput();
    //Print credits
    void printCredits();
    //Print given string
    void printMessage(std::string message);
    //Print GUI for player turn
    void handStart(std::string playerName);
    //Print a factory
    void printFactory(Factory **factories);
    //Print centerPile
    void printFactory(std::vector<std::vector<TileType>> centerPiles);
    //Print player's mosaic
    void printMosaic(PlayerTree *players, bool grayboard);
    void printMosaic(Player *player);
    //Print player's score
    void printScore(string name, int score);
    //Print game results with winner
    void gameOver(Player *player);
    //Print game results with draw
    void gameOver();
    //Print help section
    void help(std::string context);

private:
    std::string nameHelper(PlayerNode *node);
    std::string nameHelperHelper(PlayerNode *node);
    std::string linesHelper(PlayerNode *node, int j, bool grayboard);
    std::string brokenHelper(PlayerNode *node);
    std::string getOutputColour(TileType tileType);
    std::string factoryHelper(Factory *factory, int i);

    //Master wall pattern for comparison
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

#endif // !MENU_H