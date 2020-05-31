#include "GameEngine.h"

#include "Saver.h"
#include <iostream>
#include <algorithm>
#include <random>

GameEngine::GameEngine(Menu *menu, int seedIn) : players(),
                                                 factories(),
                                                 centerPile(),
                                                 bag(new TileList()),
                                                 lid(new TileList()),
                                                 menu(menu)
{
    //Create factories
    for (int i = 0; i < NUM_FACTORIES; ++i)
    {
        factories[i] = new Factory();
    }
    seed = seedIn;
    turnNum = 0;
}

GameEngine::~GameEngine()
{
    for (int i = 0; i < NUM_FACTORIES; ++i)
    {
        if (factories[i] != nullptr)
            delete factories[i];
    }
    if (bag != nullptr)
        delete bag;
    if (lid != nullptr)
        delete lid;
    for (Player *player : players)
        if (player != nullptr)
            delete player;
}

void GameEngine::logTurn(std::string turn)
{
    log.push_back(turn);
}

std::vector<std::string> GameEngine::getLog()
{
    return log;
}

bool GameEngine::playGame()
{
    addPlayers();
    fillBag();
    playerTurnID = players[0];
    return playGame(false);
}

bool GameEngine::playGame(bool replay)
{
    //Bool to track exit condition
    bool exit = false;

    while (!exit && !hasPlayerWon())
    {
        if (turnNum >= (int)log.size())
        {
            replay = false;
        }
        exit = playRound(replay);
    }
    //Check if a player has won after each round
    if (hasPlayerWon())
    {
        //Game is a draw
        if (players[0]->getScore() == players[1]->getScore())
        {
            //Print results
            menu->gameOver(players[0]->getName(), players[1]->getName(), players[0]->getScore());
        }
        //Set pointer to winning player
        else
        {
            Player *winningPlayer = playerTurnID;
            for (auto player : players)
            {
                if (player->getScore() > winningPlayer->getScore())
                {
                    winningPlayer = player;
                }
            }
            //Print results
            menu->gameOver(winningPlayer);
        }
        menu->printMessage("Do you want to save this game? (y/n)");
        std::stringstream ss(menu->getInput());
        string command;
        ss >> command;

        if (command == "y" || command == "yes")
        {
            menu->printMessage("Please enter name for save file");
            std::stringstream ss(menu->getInput());
            string filename;
            ss >> filename;

            Saver *saver = new Saver();
            saver->save(this, filename);
        }
    }
    return exit;
}

bool GameEngine::playRound(bool replay)
{
    //Track whether all tiles have been drawn
    if (roundOver())
    {
        //Return FP tile to center
        centerPile.push_back(FIRSTPLAYER);
        for (int i = 0; i < NUM_FACTORIES; i++)
        {
            TileType temp[4] = {NOTILE, NOTILE, NOTILE, NOTILE};

            for (int j = 0; j < FACTORY_SIZE; j++)
            {
                if (bag->size() == 0)
                {
                    TileList *tmp = bag;
                    bag = lid;
                    lid = tmp;
                    tmp = nullptr;
                    delete tmp;
                }

                temp[j] = bag->removeFront();
            }
            //Refill factories
            factories[i]->fill(temp);
        }
    }

    bool exit = false;

    menu->printMessage("=== Start Round ===");

    while (!exit && !roundOver())
    {
        //Print player turn GUI
        menu->handStart(playerTurnID->getName());
        menu->printFactory(&centerPile);
        for (int i = 0; i < NUM_FACTORIES; i++)
        {
            menu->printFactory(i + 1, factories[i]->getTiles());
        }
        menu->printMosaic(playerTurnID);

        //Bool to check whether input command has been executed
        bool inputDone = false;

        //Handle user input
        if (replay)
        {
            do
            {
                handleReplay(&inputDone, &exit);
                turnNum++;
            } while (!exit && !inputDone);
        }
        else
        {
            do
            {
                handleInput(&inputDone, &exit);
            } while (!exit && !inputDone);
        }
    }

    if (!exit)
    {
        //Distribute tiles to walls
        menu->printMessage("=== END OF ROUND ===");

        for (auto player : players)
        {
            if (player->hasFirstPlayer())
            {
                playerTurnID = player;
            }

            for (TileType tile : player->calcScore())
            {
                lid->addBack(tile);
            }
            menu->printScore(player->getName(), player->getScore());
        }
    }
    return exit;
}

void GameEngine::handleInput(bool *inputDone, bool *exit)
{
    std::stringstream ss(menu->getInput());
    if (!std::cin.eof())
    {
        string command;
        ss >> command;
        string errorMessage = "Command not recognised";

        // Print help
        if (command == "help")
        {
            menu->help("round");
        }

        // Command to save game to file
        else if (command == "save")
        {
            if (!saveCommand(inputDone, &ss))
            {
                errorMessage = "Invalid syntax for save command";
            };
        }

        // Command to take turn
        else if (command == "turn")
        {
            errorMessage = convertCommand(inputDone, &ss);
        }

        if (!*inputDone)
            menu->printMessage("Invalid input: " + errorMessage + ", try again");

        if (inputDone)
        {
            logTurn(ss.str());
        }
    }
    else
    {
        *exit = true;
    }
}

void GameEngine::handleReplay(bool *inputDone, bool *exit)
{
    if (!std::cin.eof())
    {
        std::string command = log[turnNum];
        std::stringstream ss(menu->getInput());
        string next;
        ss >> next;

        if (next == "next" || next == "n")
        {
            menu->printMessage(command);
            convertCommand(inputDone, command);
        }
        else if (next == "help")
        {
            menu->help("replay");
        }
        else if (next == "resume" || next == "r")
        {
        }
        if (!inputDone)
        {
            menu->printMessage("Invalid input: incorrect command, try again");
        }
    }
    else
    {
        *exit = true;
    }
}

bool GameEngine::saveCommand(bool *inputDone, std::stringstream *ss)
{
    bool retValue = false;
    std::string fileName;
    if (!ss->eof())
    {
        ss->get(); // removes initial whitespace between save and filename
        std::getline(*ss, fileName);
        if (isNotWhiteSpace(fileName))
        {
            Saver saver;
            saver.save(this, fileName);
            *inputDone = true;
        }
    }
    if (*inputDone)
    {
        menu->printMessage("Game successfully saved to '" + fileName + "'");
        retValue = true;
    }
    return retValue;
}

std::string GameEngine::convertCommand(bool *inputDone, std::stringstream *ss)
{
    std::string errorMessage = "none";
    unsigned int factoryNum = NUM_FACTORIES + 1, lineNum = NUMBER_OF_LINES + 1;
    char colour = '\0';

    *ss >> factoryNum >> colour >> lineNum;
    TileType tileType = charToTileType(colour);
    --lineNum;

    errorMessage = turnCommand(inputDone, factoryNum, lineNum, tileType);
    return errorMessage;
}

std::string GameEngine::convertCommand(bool *inputDone, std::string str)
{
    std::string errorMessage = "none";
    unsigned int factoryNum = NUM_FACTORIES + 1, lineNum = NUMBER_OF_LINES + 1;
    char colour = '\0';

    factoryNum = str.at(5) - 48;
    colour = str.at(7);
    lineNum = str.at(9) - 48;
    TileType tileType = charToTileType(colour);
    --lineNum;

    errorMessage = turnCommand(inputDone, factoryNum, lineNum, tileType);
    return errorMessage;
}

std::string GameEngine::turnCommand(bool *inputDone, int factoryNum, int lineNum, TileType colour)
{
    std::string errorMessage = "none";
    //Error checking with helpful messages
    if (validFactoryNum(factoryNum))
    {
        if (selectableTile(colour))
        {
            if (validLineNum(lineNum))
            {
                //If drawing from center pile
                if (factoryNum == 0)
                {
                    errorMessage = centerCommand(inputDone, colour, lineNum);
                }
                //If drawing from factory
                else
                {
                    errorMessage = factoryCommand(inputDone, factoryNum, lineNum, colour);
                }

                // If the input has been executed
                if (*inputDone)
                {
                    //Change player turn
                    changePlayerTurn();
                    menu->printMessage("Turn successful.");
                }
            }
            else
                errorMessage = "Invalid line number";
        }
        else
            errorMessage = "Invalid colour";
    }
    else
        errorMessage = "Invalid factory number";
    return errorMessage;
}

std::string GameEngine::centerCommand(bool *inputDone, TileType tileType, int lineNum)
{
    std::string errorMessage = "none";
    if (!centerPile.empty() && centerPileContains(tileType))
    {
        //If moving tiles straight to broken line
        if (lineNum == 5)
        {
            playerTurnID->getMosaic()->addToBrokenTiles(drawFromCenter(tileType), tileType);
            *inputDone = true;
        }
        //If moving tiles to pattern line
        else if (!playerTurnID->getMosaic()->isFilled(lineNum, tileType) &&
                 playerTurnID->getMosaic()->getLine(lineNum)->canAddTiles(tileType))
        {
            if (containsFirstPlayer())
            {
                playerTurnID->getMosaic()->getBrokenTiles()->addFront(FIRSTPLAYER);
            }
            playerTurnID->getMosaic()->insertTilesIntoLine(lineNum, drawFromCenter(tileType), tileType);
            *inputDone = true;
        }
        else
            errorMessage = "Specified line cannot add tile";
    }
    else
        errorMessage = "Center pile does not contain specified tile";
    return errorMessage;
}

std::string GameEngine::factoryCommand(bool *inputDone, int factoryNum, int lineNum, TileType tileType)
{
    std::string errorMessage = "none";
    //-1 from factory num input as index starts at 0
    --factoryNum;
    if (!factories[factoryNum]->isEmpty())
    {
        if (factories[factoryNum]->contains(tileType))
        {
            //if Adding straight to broken tiles
            if (lineNum == 5)
            {
                playerTurnID->getMosaic()->addToBrokenTiles(factories[factoryNum]->draw(tileType), tileType);
                for (TileType tile : factories[factoryNum]->empty())
                {
                    centerPile.push_back(tile);
                }
                *inputDone = true;
            }
            //Adding to pattern line
            else if (!playerTurnID->getMosaic()->isFilled(lineNum, tileType) &&
                     playerTurnID->getMosaic()->getLine(lineNum)->canAddTiles(tileType))
            {
                playerTurnID->getMosaic()->insertTilesIntoLine(lineNum, factories[factoryNum]->draw(tileType), tileType);

                for (TileType tile : factories[factoryNum]->empty())
                {
                    centerPile.push_back(tile);
                }

                *inputDone = true;
            }
            else
                errorMessage = "Specified line cannot add tile";
        }
        else
            errorMessage = "Selected factory does not contain specified tile";
    }
    else
        errorMessage = "Selected factory is empty";
    return errorMessage;
}

bool GameEngine::validLineNum(int lineNum)
{
    return lineNum >= 0 && lineNum <= NUMBER_OF_LINES;
}

bool GameEngine::validFactoryNum(int factoryNum)
{
    return factoryNum >= 0 && factoryNum <= NUM_FACTORIES;
}

bool GameEngine::roundOver()
{
    // centerPile.empty() && factoriesAreEmpty(); equivalent but less efficient
    return !(!centerPile.empty() || !factoriesAreEmpty());
}

// returns true if factories are empty
bool GameEngine::factoriesAreEmpty()
{
    bool factoriesEmpty = true;
    int factoryIndex = 0;
    while (factoriesEmpty && factoryIndex < NUM_FACTORIES)
    {
        factoriesEmpty &= factories[factoryIndex++]->isEmpty();
    }
    return factoriesEmpty;
}

void GameEngine::setPlayerTurn(int playerIndex)
{
    playerTurnID = players[playerIndex];
}

void GameEngine::changePlayerTurn()
{
    if (players[0] == playerTurnID)
    {
        playerTurnID = players[1];
    }
    else
    {
        playerTurnID = players[0];
    }
}

Factory *GameEngine::getFactory(int position)
{
    return factories[position];
}

void GameEngine::fillBag()
{
    //Create both a bag and shuffle vector
    TileType tileTypes[5] = {BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED};
    TileType temp[100];

    //Store all tiles in temp bag SORTED
    for (int i = 0; i < 100; i++)
    {
        temp[i] = tileTypes[i / 20];
    }

    //Shuffle temp bag
    std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(seed));

    //Store shuffled tiles into the game bag
    for (int i = 0; i < 100; i++)
    {
        bag->addBack(temp[i]);
    }
}

int GameEngine::drawFromCenter(TileType colour)
{
    int count = 0;
    std::vector<int> remove;
    int decrement = 0;

    //Loop through vector saving indexes of tiles to be erased
    for (int i = 0; i < (int)centerPile.size(); i++)
    {
        if (centerPile[i] == colour)
        {
            count++;
            remove.push_back(i);
        }
    }
    //Loop through indexes
    for (int i = 0; i < (int)remove.size(); i++)
    {
        // erase element at index
        centerPile.erase(centerPile.begin() + (remove[i] - decrement));
        // Reduce all future indexes by 1
        decrement++;
    }
    return count;
}

bool GameEngine::containsFirstPlayer()
{
    bool contains = false;
    if (centerPile[0] == FIRSTPLAYER)
    {
        centerPile.erase(centerPile.begin());
        contains = true;
    }
    return contains;
}

void GameEngine::fillBag(TileList *bag)
{
    if (this->bag != nullptr)
        delete this->bag;
    this->bag = bag;
}

void GameEngine::fillLid(TileList *lid)
{
    if (this->lid != nullptr)
        delete this->lid;
    this->lid = lid;
}

void GameEngine::fillCenterPile(std::vector<TileType> centerPile)
{
    this->centerPile = centerPile;
}

void GameEngine::fillFactories(Factory *factories[])
{
    for (int i = 0; i < NUM_FACTORIES; ++i)
    {
        if (this->factories[i] != nullptr)
            delete this->factories[i];
        this->factories[i] = factories[i];
    }
}

Player *GameEngine::addPlayer(string name)
{
    //creates a new player
    return addPlayer(name, 0, new Mosaic());
}

Player *GameEngine::addPlayer(std::string name, int score, Mosaic *mosaic)
{
    players.push_back(new Player(name, score, mosaic));
    return players.back();
}

void GameEngine::addPlayers()
{
    //Checks for player names and adds them to player vector
    bool hasValidName = true;
    string name1;
    do
    {
        menu->printMessage("Enter the name for player 1:");
        name1 = menu->getInput();
        hasValidName = isNotWhiteSpace(name1);
        if (!hasValidName)
            menu->printMessage("Error - Invalid Name");
    } while (!hasValidName);

    hasValidName = true;
    string name2;
    do
    {
        menu->printMessage("Enter the name for player 2:");
        name2 = menu->getInput();
        hasValidName = isNotWhiteSpace(name2);
        if (!hasValidName)
            menu->printMessage("Error - Invalid Name");
    } while (!hasValidName);

    addPlayer(name1);
    addPlayer(name2);

    menu->printMessage("Let's Play!");
}

std::vector<TileType> GameEngine::getCenterPile()
{
    return centerPile;
}

TileList *GameEngine::getBag()
{
    return bag;
}

TileList *GameEngine::getLid()
{
    return lid;
}

Player *GameEngine::getPlayerTurnID()
{
    return playerTurnID;
}

Player *GameEngine::getPlayer(int playerIndex)
{
    return players.at(playerIndex);
}

bool GameEngine::hasPlayerWon()
{
    bool playerWon = false;
    unsigned int playerIndex = 0;
    while (!playerWon && playerIndex < players.size())
        playerWon = players.at(playerIndex++)->hasWon();
    return playerWon;
}

bool GameEngine::centerPileContains(TileType tileType)
{
    bool centerPileContainsTile = false;
    unsigned int index = 0;
    while (!centerPileContainsTile && index < centerPile.size())
    {
        centerPileContainsTile = centerPile[index] == tileType;
        index++;
    }
    return centerPileContainsTile;
}

int GameEngine::getSeed()
{
    return seed;
}
