#include "GameEngine.h"
#include "Saver.h"
#include "Menu.h"
#include "Saver.h"

int main(int argc, char const *argv[])
{
    //GameEngine engine = new GameEngine();
    Menu menu;
    bool exit = false;
    std::string input;
    GameEngine *engine = nullptr;
    Saver saver;
    do
    {
        menu.printMenu();
        input = menu.getInput();
        if (input == "1")
        {
            menu.printMessage("Starting a New Game");
            int seed = 1;
            if (argc > 1)
            {
                std::istringstream iss(argv[1]);
                iss >> seed;
            }
            bool inputValid = false;
            int numPlayers = 0;
            do
            {
                menu.printMessage("How many players? (2, 3 or 4");
                try
                {
                    numPlayers = stoi(menu.getInput());
                }
                catch (...)
                {
                    menu.printMessage("Invalid number, try again");
                }
                if (numPlayers >= 2 && numPlayers <= 4)
                {
                    inputValid = true;
                }
                else
                    menu.printMessage("Invalid number, try again");
            } while (!inputValid);
            int numCenters = 0;
            inputValid = false;
            do
            {
                menu.printMessage("How many center factories? (1 or 2");
                try
                {
                    numCenters = stoi(menu.getInput());
                }
                catch (...)
                {
                    menu.printMessage("Invalid number, try again");
                }
                if (numCenters >= 1 && numCenters <= 2)
                {
                    inputValid = true;
                }
                else
                    menu.printMessage("Invalid number, try again");
            } while (!inputValid);

            int gameType = 1;
            inputValid = false;
            do
            {
                menu.printMessage("Which game mode? (1, 2 or 3");
                menu.printMessage("1. Standard");
                menu.printMessage("2. Gray Board");
                menu.printMessage("3. Six Tiles");
                try
                {
                    gameType = stoi(menu.getInput());
                }
                catch (...)
                {
                    menu.printMessage("Invalid number, try again");
                }
                if (numCenters >= 1 && numCenters <= 3)
                {
                    inputValid = true;
                }
                else
                    menu.printMessage("Invalid number, try again");
            } while (!inputValid);

            bool grayboard = false, sixTiles = false;
            if (gameType == 2)
            {
                grayboard = true;
            }
            else if (gameType == 3)
            {
                sixTiles = true;
            }

            engine = new GameEngine(&menu, seed, numPlayers, numCenters, grayboard, sixTiles);

            exit = engine->playGameFill();
        }
        else if (input == "2")
        {
            menu.printMessage("Enter the filename from which load a game");
            std::string fileName = menu.getInput();
            if (!std::cin.eof())
            {
                try
                {
                    engine = saver.load(fileName, &menu);
                    if (engine != nullptr)
                    {
                        if (engine->hasPlayerWon())
                        {
                            delete engine;
                            throw "Game in save file is complete, use the replay function to review/resume completed games";
                        }
                        else
                        {
                            menu.printMessage("Azul game successfully loaded");
                            exit = engine->playGame();
                        }
                    }
                }
                catch (const char *errorMessage)
                {
                    std::string message(errorMessage);
                    menu.printMessage("Error reading file - " + message + "\n");
                }
            }
            else
                exit = true;
        }
        else if (input == "3")
        {
            menu.printMessage("Enter the filename from which load game to replay");
            std::string fileName = menu.getInput();
            if (!std::cin.eof())
            {
                try
                {
                    engine = saver.replay(fileName, &menu);
                    if (engine != nullptr)
                    {
                        if (engine->getGameMode() == 2)
                        {
                           throw "Replay is not enabled for gray board gamemode";
                        }
                        else
                        {
                            menu.printMessage("Azul game successfully loaded");
                            engine->toggleReplay();
                            exit = engine->playGame();
                        }
                    }
                }
                catch (const char *errorMessage)
                {
                    std::string message(errorMessage);
                    menu.printMessage("Error reading file - " + message + "\n");
                }
            }
            else
                exit = true;
        }
        else if (input == "4")
        {
            menu.printCredits();
        }
        else if (std::cin.eof() || input == "5")
        {
            exit = true;
        }
        else if (input == "help")
        {
            menu.help("menu");
        }
        else
        {
            menu.printMessage("Invalid input, try again");
        }
    } while (!exit);
    menu.printMessage("Goodbye");
    if (engine != nullptr)
        delete engine;

    return 0;
}
