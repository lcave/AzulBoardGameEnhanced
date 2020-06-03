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
    std::cout << COMMAND << "Factories: \n"
              << RESET;
}

void Menu::printFactory(Factory **factories)
{
    for (int i = 0; i < NUM_FACTORIES; i++)
    {
        std::string str;
        str.append(factoryHelper(factories[i], i));
        if (factories[5 + i] != nullptr && 5 + i < 9)
        {
            str.append("    ");
            str.append(factoryHelper(factories[5 + i], 5 + i));
        }
        std::cout << str << std::endl;
    }
}

std::string Menu::factoryHelper(Factory *factory, int i)
{
    std::string str = std::to_string(i + 1);
    str.append(": ");
    for (int j = 0; j < FACTORY_SIZE; j++)
    {
        str.append(getOutputColour(factory->getTiles()[j]));
        str.push_back(factory->getTiles()[j]);
        str.append(RESET);
        str.append(" ");
    }
    return str;
}

void Menu::printFactory(std::vector<std::vector<TileType>> centerPiles)
{
    int count = 1;
    for(auto pile : centerPiles)
    {
        std::cout << "C" << count << ": ";
        for (TileType tile : pile)
        {
            std::cout << getOutputColour(tile) << (char)tile << RESET << " ";
        }
        std::cout << std::endl;
        ++count;
    }   
}

void Menu::printMosaic(PlayerTree *players)
{
    std::cout << std::endl;
    std::cout << nameHelper(players->getRoot()) << std::endl;

    for (int j = 0; j < NUMBER_OF_LINES; j++)
    {
        std::cout << linesHelper(players->getRoot(), j) << std::endl;
    }
    std::cout << brokenHelper(players->getRoot()) << std::endl;
}

std::string Menu::nameHelper(PlayerNode *node)
{
    std::string str;
    str = nameHelperHelper(node);

    if (node->getRightNode() != nullptr)
    {
        str.append(nameHelper(node->getRightNode()));
    }
    if (node->getLeftNode() != nullptr)
    {
        str.append(nameHelper(node->getLeftNode()));
    }
    return str;
}

std::string Menu::nameHelperHelper(PlayerNode *node)
{
    std::string str = COMMAND;
    str.append("Mosaic for ");
    str.append(PARAMETER);
    str.append(node->getPlayer()->getName());
    str.append(RESET);
    for (int i = 0; i < 64 - (int)str.length(); i++)
    {
        str.append(" ");
    }
    return str;
}

std::string Menu::linesHelper(PlayerNode *node, int j)
{
    std::string str = std::to_string(j + 1);
    str.append(": ");
    int lineSize = node->getPlayer()->getMosaic()->getLine(j)->getMaxSize();
    int numTiles = node->getPlayer()->getMosaic()->getLine(j)->getNumTiles();

    for (int i = 0; i < 5 - lineSize; i++)
    {
        str.append("  ");
    }

    for (int i = 0; i < lineSize - numTiles; i++)
    {
        str.append(BULLET);
        str.append(" ");
    }
    for (int i = 0; i < numTiles; i++)
    {
        str.append(" ");
        TileType type = node->getPlayer()->getMosaic()->getLine(j)->getTileType();
        str.append(getOutputColour(type));
        str.push_back(type);
        str.append(RESET);
    }

    str.append(" || ");

    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        if (node->getPlayer()->getMosaic()->getWallLine(j)[i] == true)
        {
            str.append(" ");
            str.append(getOutputColour(master_wall[j][i]));
            str.push_back(master_wall[j][i]);
            str.append(RESET);
        }
        else
        {
            str.append(getOutputColour(master_wall[j][i]));
            str.append(" ");
            str.append(BULLET);
            str.append(RESET);
        }
    }
    str.append("    ");
    if (node->getRightNode() != nullptr)
    {
        str.append(linesHelper(node->getRightNode(), j));
    }
    if (node->getLeftNode() != nullptr)
    {
        str.append(linesHelper(node->getLeftNode(), j));
    }
    return str;
}

std::string Menu::brokenHelper(PlayerNode *node)
{
    std::string str = COMMAND;
    str.append("broken: ");
    str.append(RESET);
    str.append(node->getPlayer()->getMosaic()->getBrokenTiles()->toString());

    for (int i = 0; i < 63 - (int)str.length(); i++)
    {
        str.append(" ");
    }

    if (node->getRightNode() != nullptr)
    {
        str.append(brokenHelper(node->getRightNode()));
    }
    if (node->getLeftNode() != nullptr)
    {
        str.append(brokenHelper(node->getLeftNode()));
    }
    return str;
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

void Menu::gameOver()
{
    std::cout << std::endl;
    std::cout << ALERT << "=== Game Over ===" << RESET << std::endl;
    std::cout << "Draw!" << std::endl;
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
    }
    if (context == "replay")
    {
        std::cout << COMMAND << "  'next'" << RESET << " or " << COMMAND << "'n' " << RESET << "will advance to the next turn" << std::endl;
        std::cout << COMMAND << "  'resume'" << RESET << " or " << COMMAND << "'r' " << RESET << "will resume normal play at the current turn" << std::endl;
        std::cout << "  If the replay history concludes before the end of the game, play will continue normally" << std::endl;
    }
}