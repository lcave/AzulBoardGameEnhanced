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
            engine = new GameEngine(&menu, seed);

            exit = engine->playGame();
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
                        menu.printMessage("Azul game successfully loaded");
                        exit = engine->playGame(false);
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
                        menu.printMessage("Azul game successfully loaded");
                        exit = engine->playGame(true);
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
