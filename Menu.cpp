#include "Menu.h"

void Menu::printMenu()
{
    std::cout << std::endl;
    std::cout << "Menu" << std::endl
              << "----" << std::endl
              << "1. New Game" << std::endl
              << "2. Load Game" << std::endl
              << "3. Replay Game" << std::endl
              << "4. Credits" << std::endl
              << "5. Quit" << std::endl
              << std::endl;
}
std::string Menu::getInput()
{
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    std::cout << input << std::endl;
    return input;
}

void Menu::printCredits()
{
    std::cout << std::endl
              << "----------------------------------" << std::endl
              << "Name: Luca Cave" << std::endl
              << "Student ID: s3787946" << std::endl
              << "Email: s3787946@student.rmit.edu.au" << std::endl
              << std::endl
              << "Name: Michael Sartorel" << std::endl
              << "Student ID: s3786267" << std::endl
              << "Email: s3786267@student.rmit.edu.au" << std::endl
              << std::endl
              << "Name: Dylan Dimkovski" << std::endl
              << "Student ID: s3717379" << std::endl
              << "Email: s3717379@student.rmit.edu.au" << std::endl
              << "----------------------------------" << std::endl
              << std::endl;
}

void Menu::printMessage(std::string message)
{
    std::cout << std::endl
              << ALERT << message << RESET
              << std::endl;
}

void Menu::handStart(string playername)
{
    std::cout << "TURN FOR PLAYER: " << PARAMETER << playername << RESET << std::endl;
    std::cout << COMMAND << "Factories: \n" << RESET;
}

void Menu::printFactory(int id, TileType* contents)
{
    std::cout << id << ": ";
    for (int i = 0; i < FACTORY_SIZE; i++)
    {
        std::cout << getOutputColour(contents[i]) << (char)contents[i] << RESET << " ";
    }
    std::cout << std::endl;
    std::cout << (id == 5 ? "\n" : "");
}

void Menu::printFactory(std::vector<TileType> *centerPile)
{
    std::cout << "0: ";
    for (TileType tile : *centerPile)
    {
        std::cout << getOutputColour(tile) << (char)tile << RESET << " ";
    }
    std::cout << std::endl;
}

void Menu::printMosaic(Player *player)
{
    std::cout << COMMAND << "Mosaic for " << PARAMETER << player->getName() << RESET << std::endl;
    for (int j = 0; j < NUMBER_OF_LINES; j++)
    {
        std::cout << j + 1 << ": ";
        std::string output;
        int lineSize = player->getMosaic()->getLine(j)->getMaxSize();
        int numTiles = player->getMosaic()->getLine(j)->getNumTiles();

        for (int i = 0; i < 5 - lineSize; i++)
        {
            std::cout << "  ";
        }

        for (int i = 0; i < lineSize - numTiles; i++)
        {
            std::cout << BULLET << " ";
        }
        for (int i = 0; i < numTiles; i++)
        {
            std::cout << " ";
            TileType type = player->getMosaic()->getLine(j)->getTileType();
            std::cout << getOutputColour(type) << (char)type << RESET;
        }

        std::cout << " || ";

        for (int i = 0; i < NUMBER_OF_LINES; i++)
        {
            if (player->getMosaic()->getWallLine(j)[i] == true)
            {
                std::cout << " ";
                std::cout << getOutputColour(master_wall[j][i]);
                std::cout << (char)master_wall[j][i];
                std::cout << RESET;
            }
            else
            {
                std::cout << getOutputColour(master_wall[j][i]) << " " << BULLET << RESET;
            }
        }
        std::cout << std::endl;
    }
    std::cout << COMMAND << "broken: " << RESET << player->getMosaic()->getBrokenTiles()->toString() << std::endl;
}

std::string Menu::getOutputColour(TileType tileType)
{
    std::string retValue = "";
    if (tileType == RED)
    {
        retValue = "\033[31m";
    }
    if (tileType == LIGTHBLUE)
    {
        retValue = "\033[36m";
    }
    if (tileType == DARKBLUE)
    {
        retValue = "\033[34m";
    }
    if (tileType == YELLOW)
    {
        retValue = "\033[33m";
    }
    if (tileType == YELLOW)
    {
        retValue = "\033[33m";
    }
    return retValue;
}

void Menu::printScore(string name, int score)
{
    std::cout << PARAMETER << name << RESET << " score: " << PARAMETER << score << RESET << std::endl;
}

void Menu::gameOver(Player *player)
{
    std::cout << std::endl;
    std::cout << ALERT << "=== Game Over ===" << std::endl;
    std::cout << PARAMETER << "Player " << player->getName() << RESET << " wins!" << std::endl;
    printScore(player->getName(), player->getScore());
}

void Menu::gameOver(string name1, string name2, int score)
{
    std::cout << std::endl;
    std::cout << ALERT << "=== Game Over ===" << RESET << std::endl;
    std::cout << "Draw!" << std::endl;
    std::cout << "Player " << PARAMETER << name1 << RESET << " score: " << PARAMETER << score << RESET << std::endl;
    std::cout << "Player " << PARAMETER << name2 << RESET << " score: " << PARAMETER << score << RESET << std::endl;
}

void Menu::help(string context)
{
    std::cout << "The following commands are accepted:" << std::endl;
    if (context == "round")
    {
        std::cout << COMMAND << "   turn " << PARAMETER << "<factory number> <colour as letter> <line to insert into>" << RESET << std::endl;
        std::cout << "      For example: " << COMMAND << "turn " << PARAMETER << "1 R 3" << RESET << std::endl;
        std::cout << COMMAND << "   save " << PARAMETER << "<save file name/path>" << RESET << std::endl;
        std::cout << "      For example: " << COMMAND << "save " << PARAMETER << "/saves/save1" << RESET << std::endl;
        std::cout << COMMAND << "   Control + D " << RESET << "to quit" << std::endl;
    }
    if (context == "menu")
    {
        std::cout << "  Numerals " << COMMAND << "1,2,3,4" << RESET << " select the corresponding menu item" << std::endl;
    } if (context == "replay"){
        std::cout << COMMAND << "  'next'" << RESET << " or " << COMMAND << "'n' " << RESET << "will advance to the next turn" << std::endl;
        std::cout << COMMAND << "  'resume'" << RESET << " or " << COMMAND << "'r' " << RESET << "will resume normal play at the current turn" << std::endl;
        std::cout << "  If the replay history concludes before the end of the game, play will continue normally" << std::endl;
    }

}