#ifndef MENU_H
#define MENU_H

#define RESET "\033[0m"             
#define COMMAND "\033[32m"          // Green
#define PARAMETER "\033[33m"        // Yellow
#define ALERT "\033[31m"              // Red

#define BULLET "\u2022"

#include <string>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Menu.h"

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
    void printFactory(int id, TileType* contents);
    //Print centerPile
    void printFactory(std::vector<TileType> *centerPile);
    //Print player's mosaic
    void printMosaic(Player *player);
    //Print player's score
    void printScore(string name, int score);
    //Print game results with winner
    void gameOver(Player *player);
    //Print game results with draw
    void gameOver(std::string name1, std::string name2, int score);
    //Print help section
    void help(std::string context);

private:
    std::string getOutputColour(TileType tileType);

    //Master wall pattern for comparison
    TileType master_wall[NUMBER_OF_LINES][NUMBER_OF_LINES] = {DARKBLUE, YELLOW, RED, BLACK, LIGTHBLUE,
                                                              LIGTHBLUE, DARKBLUE, YELLOW, RED, BLACK,
                                                              BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED,
                                                              RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                                                              YELLOW, RED, BLACK, LIGTHBLUE, DARKBLUE};
};

#endif // !MENU_H