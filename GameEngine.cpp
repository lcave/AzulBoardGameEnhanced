#include "GameEngine.h"

#include "Saver.h"
#include <iostream>
#include <algorithm>
#include <random>

GameEngine::GameEngine(Menu *menu, int seedIn, int numPlayers,
                       int numCenters, bool grayboard, bool sixTiles) : seed(seedIn),
                                                                        numPlayers(numPlayers),
                                                                        numCenters(numCenters),
                                                                        factories(),
                                                                        centerPiles(),
                                                                        menu(menu)

{

    bag = new TileList();
    lid = new TileList();
    numFactories = numPlayers * 2 + 1;
    //Create factories
    for (int i = 0; i < (numFactories); ++i)
    {
        factories[i] = new Factory();
    }
    for (int i = 0; i < numCenters; i++)
    {
        std::vector<TileType> centerPile;
        centerPiles.push_back(centerPile);
    }

    turnNum = 0;
    replay = false;
    players = new PlayerTree();
    this->grayboard = grayboard;
    this->sixTiles = sixTiles;
}

GameEngine::~GameEngine()
{
    for (int i = 0; i < numFactories; ++i)
    {
        if (&factories[i] != nullptr)
        {
            delete factories[i];
            factories[i] = nullptr;
        }
    }
    if (bag != nullptr)
        delete bag;
    if (lid != nullptr)
        delete lid;
    delete players;
}

void GameEngine::logTurn(std::string turn)
{
    log.push_back(turn);
}

void GameEngine::toggleReplay()
{
    replay = !replay;
}

std::vector<std::string> GameEngine::getLog()
{
    return log;
}

bool GameEngine::playGameFill()
{
    addPlayers();
    fillBag();
    playerTurnID = players->getPlayerWithTurn();
    return playGame();
}

bool GameEngine::playGame()
{
    //Bool to track exit condition
    bool exit = false;

    while (!exit && !hasPlayerWon())
    {
        exit = playRound();
    }
    //Check if a player has won after each round
    if (hasPlayerWon())
    {
        Player *winner = players->winningPlayer();
        //Game is a draw
        if (winner == nullptr)
        {
            //Print results
            menu->gameOver();
        }
        else
        {
            //Print results
            menu->gameOver(winner);
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

            Saver saver;
            saver.save(this, filename);
        }
    }
    return exit;
}

bool GameEngine::playRound()
{
    //Track whether all tiles have been drawn
    if (roundOver())
    {
        //Return FP tile to center
        for (int i = 0; i < numCenters; i++)
        {
            centerPiles[i].push_back(FIRSTPLAYER);
        }

        for (int i = 0; i < (numFactories); i++)
        {
            TileType temp[4] = {NOTILE, NOTILE, NOTILE, NOTILE};

            for (int j = 0; j < FACTORY_SIZE; j++)
            {
                if (bag->empty())
                {
                    TileList *tmp = bag;
                    bag = lid;
                    lid = tmp;
                    tmp = nullptr;
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
        menu->printFactory(centerPiles);
        menu->printFactory(factories);

        menu->printMosaic(players, grayboard);

        //Bool to check whether input command has been executed
        bool inputDone = false;

        //Handle user input
        if (replay && turnNum >= (int)log.size())
        {
            replay = false;
            menu->printMessage("Gameplay resumed!");
        }
        if (replay)
        {
            do
            {
                handleReplay(&inputDone, &exit);
                turnNum++;
            } while (!exit && !inputDone && replay);
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
        playerTurnID = players->endRound(lid, menu, grayboard);
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

        if (!*inputDone && command != "help")
            menu->printMessage("Invalid input: " + errorMessage + ", try again");
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
            replay = false;
            for (int i = turnNum; i < (int)log.size(); i++)
            {
                log.erase(log.end());
            }
            menu->printMessage("Gameplay resumed!");
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
    std::string facNum = "";
    int lineNum = NUMBER_OF_LINES + 1, factoryNum = 0;
    char colour = '\0';

    *ss >> facNum >> colour >> lineNum;
    if (facNum.at(0) == 'C')
    {
        int num = facNum.at(1);
        factoryNum = (num - 48) - (2 * (num - 48));
    }
    else
    {
        factoryNum = facNum.at(0) - 48;
    }
    TileType tileType = charToTileType(colour);
    --lineNum;

    errorMessage = turnCommand(inputDone, factoryNum, lineNum, tileType);
    if (errorMessage == "none")
    {
        logTurn(ss->str());
    }
    return errorMessage;
}

std::string GameEngine::convertCommand(bool *inputDone, std::string str)
{
    std::string errorMessage = "none";
    int factoryNum = (numPlayers * 2 + 1) + 1, lineNum = NUMBER_OF_LINES + 1;
    char colour = '\0';
    if (str.at(5) == 'C')
    {
        factoryNum = (str.at(6) - 48) - (2 * (str.at(6) - 48));
        colour = str.at(8);
        lineNum = str.at(10) - 48;
    }
    else
    {
        factoryNum = str.at(5) - 48;
        colour = str.at(7);
        lineNum = str.at(9) - 48;
    }
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
                if (factoryNum < 0)
                {
                    errorMessage = centerCommand(factoryNum, inputDone, colour, lineNum);
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
                    playerTurnID = players->switchPlayerTurn();
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

std::string GameEngine::centerCommand(int factoryNum, bool *inputDone, TileType tileType, int lineNum)
{
    std::string errorMessage = "none";
    int pileNum = 0;
    if (factoryNum == -1)
    {
        pileNum = 0;
    }
    else
    {
        pileNum = 1;
    }

    if (!centerPiles[pileNum].empty() && centerPileContains(pileNum, tileType))
    {
        //If moving tiles straight to broken line
        int broken = 5;
        if (sixTiles)
        {
            ++broken;
        }
        if (lineNum == broken)
        {
            if (containsFirstPlayer())
            {
                playerTurnID->getMosaic()->getBrokenTiles()->addFront(FIRSTPLAYER);
            }
            playerTurnID->getMosaic()->addToBrokenTiles(drawFromCenter(pileNum, tileType), tileType);
            *inputDone = true;
        }
        //If moving tiles to pattern line
        else if ((grayboard || (!playerTurnID->getMosaic()->isFilled(lineNum, tileType))) &&
                 playerTurnID->getMosaic()->getLine(lineNum)->canAddTiles(tileType))
        {
            if (containsFirstPlayer())
            {
                playerTurnID->getMosaic()->getBrokenTiles()->addFront(FIRSTPLAYER);
            }
            playerTurnID->getMosaic()->insertTilesIntoLine(lineNum, drawFromCenter(pileNum, tileType), tileType);
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
            int broken = 5;
            if (sixTiles)
            {
                ++broken;
            }
            if (lineNum == broken)
            {
                playerTurnID->getMosaic()->addToBrokenTiles(factories[factoryNum]->draw(tileType), tileType);
                int pile = 0;
                if (centerPiles.size() > 1)
                {
                    bool valid = false;
                    while (!valid)
                    {
                        menu->printMessage("Which center pile should tiles be added to?");
                        std::stringstream ss(menu->getInput());
                        std::string pile;
                        ss >> pile;
                        if (pile == "C1")
                        {
                            valid = true;
                        }

                        if (pile == "C2")
                        {
                            pile = 1;
                            valid = true;
                        }
                    }
                }
                for (TileType tile : factories[factoryNum]->empty())
                {
                    centerPiles[pile].push_back(tile);
                }
                *inputDone = true;
            }
            //Adding to pattern line
            else if ((grayboard || (!playerTurnID->getMosaic()->isFilled(lineNum, tileType))) &&
                     playerTurnID->getMosaic()->getLine(lineNum)->canAddTiles(tileType))
            {
                playerTurnID->getMosaic()->insertTilesIntoLine(lineNum, factories[factoryNum]->draw(tileType), tileType);
                int pileNum = 0;
                if (centerPiles.size() > 1)
                {
                    bool valid = false;
                    while (!valid)
                    {
                        menu->printMessage("Which center pile should tiles be added to?");
                        std::stringstream ss(menu->getInput());
                        std::string pile;
                        ss >> pile;
                        if (pile == "C1")
                        {
                            valid = true;
                        }

                        if (pile == "C2")
                        {
                            pileNum = 1;
                            valid = true;
                        }
                    }
                }
                for (TileType tile : factories[factoryNum]->empty())
                {
                    centerPiles[pileNum].push_back(tile);
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
    int lines = 5;
    if (sixTiles)
    {
        ++lines;
    }
    return lineNum >= 0 && lineNum <= lines;
}

bool GameEngine::validFactoryNum(int factoryNum)
{
    return factoryNum >= -2 && factoryNum <= numFactories;
}

bool GameEngine::roundOver()
{
    // centerPile.empty() && factoriesAreEmpty(); equivalent but less efficient
    bool empty = false;
    if (!centerPiles[0].empty())
    {
        empty = false;
    }
    else
    {
        if (numCenters > 1 && !centerPiles[1].empty())
        {
            empty = false;
        }
        else
        {
            empty = true;
        }
    }
    return empty;
}

// returns true if factories are empty
bool GameEngine::factoriesAreEmpty()
{
    bool factoriesEmpty = true;
    int factoryIndex = 0;
    while (factoriesEmpty && factoryIndex < (numPlayers * 2 + 1))
    {
        factoriesEmpty &= factories[factoryIndex++]->isEmpty();
    }
    return factoriesEmpty;
}

Factory *GameEngine::getFactory(int position)
{
    return factories[position];
}

void GameEngine::fillBag()
{
    //Create both a bag and shuffle vector

    std::vector<TileType> temp;
    if (sixTiles)
    {
        TileType tileTypes[6] = {BLACK, LIGTHBLUE, DARKBLUE, YELLOW, ORANGE, RED};
        //Store all tiles in temp bag SORTED
        for (int i = 0; i < 120; i++)
        {
            temp.push_back(tileTypes[i / 20]);
        }

        //Shuffle temp bag
        std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(seed));

        //Store shuffled tiles into the game bag
        for (int i = 0; i < 120; i++)
        {
            bag->addBack(temp[i]);
        }
    }
    else
    {
        TileType tileTypes[5] = {BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED};
        //Store all tiles in temp bag SORTED
        for (int i = 0; i < 100; i++)
        {
            temp.push_back(tileTypes[i / 20]);
        }

        //Shuffle temp bag
        std::shuffle(std::begin(temp), std::end(temp), std::default_random_engine(seed));

        //Store shuffled tiles into the game bag
        for (int i = 0; i < 100; i++)
        {
            bag->addBack(temp[i]);
        }
    }
}

int GameEngine::drawFromCenter(int pileNum, TileType colour)
{
    int count = 0;
    std::vector<int> remove;
    int decrement = 0;

    //Loop through vector saving indexes of tiles to be erased
    for (int i = 0; i < (int)centerPiles[pileNum].size(); i++)
    {
        if (centerPiles[pileNum][i] == colour)
        {
            count++;
            remove.push_back(i);
        }
    }
    //Loop through indexes
    for (int i = 0; i < (int)remove.size(); i++)
    {
        // erase element at index
        centerPiles[pileNum].erase(centerPiles[pileNum].begin() + (remove[i] - decrement));
        // Reduce all future indexes by 1
        decrement++;
    }
    return count;
}

void GameEngine::setPlayerTurn(int playerIndex)
{
    playerTurnID = players->setPlayerTurn(playerIndex);
}

bool GameEngine::containsFirstPlayer()
{
    bool contains = false;
    if (centerPiles[0][0] == FIRSTPLAYER)
    {
        for (int i = 0; i < numCenters; i++)
        {
            centerPiles[i].erase(centerPiles[i].begin());
            contains = true;
        }
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

void GameEngine::fillCenterPile(int pileNum, std::vector<TileType> centerPile)
{
    centerPiles[pileNum] = centerPile;
}

void GameEngine::fillFactories(Factory *factories[])
{
    for (int i = 0; i < (numPlayers * 2 + 1); ++i)
    {
        this->factories[i] = factories[i];
    }
}

void GameEngine::addPlayer(string name)
{
    //creates a new player
    Mosaic *mosaic = new Mosaic(sixTiles, grayboard);
    int score = 0;
    addPlayer(name, score, mosaic);
}

void GameEngine::addPlayer(std::string name, int score, Mosaic *mosaic)
{
    Player *player = new Player(name, score, mosaic);
    players->addPlayer(player);
}

void GameEngine::addPlayers()
{
    for (int i = 0; i < numPlayers; i++)
    {
        bool hasValidName = true;
        string name;
        do
        {
            //Checks for player names and adds them to player tree
            menu->printMessage("Enter the name for player " + std::to_string(i) + ":");
            name = menu->getInput();
            hasValidName = isNotWhiteSpace(name);
            if (!hasValidName)
                menu->printMessage("Error - Invalid Name");
        } while (!hasValidName);
        addPlayer(name);
    }
    menu->printMessage("Let's Play!");
}

std::vector<std::vector<TileType>> GameEngine::getCenterPile()
{
    return centerPiles;
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

Player *GameEngine::getPlayer(int index)
{
    return players->findPlayer(index);
}

int GameEngine::getNumPlayers()
{
    return numPlayers;
}

bool GameEngine::hasPlayerWon()
{
    bool playerWon = false;
    int playerIndex = 1;
    while (!playerWon && playerIndex < players->getNumPlayers())
        playerWon = players->findPlayer(playerIndex++)->hasWon(grayboard);
    return playerWon;
}

bool GameEngine::centerPileContains(int pileNum, TileType tileType)
{
    bool centerPileContainsTile = false;
    unsigned int index = 0;
    while (!centerPileContainsTile && index < centerPiles[pileNum].size())
    {
        centerPileContainsTile = centerPiles[pileNum][index] == tileType;
        index++;
    }
    return centerPileContainsTile;
}

int GameEngine::getSeed()
{
    return seed;
}

PlayerTree *GameEngine::getPlayers()
{
    return players;
}

int GameEngine::getGameMode()
{
    int mode = 1;
    if (grayboard)
    {
        mode = 2;
    }
    if (sixTiles)
    {
        mode = 3;
    }
    return mode;
}