#include "Saver.h"

// need to put this in gameEngine
TileType Master_Wall[5][5]{DARKBLUE, YELLOW, RED, BLACK, LIGTHBLUE,
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

void Saver::save(GameEngine *gameEngine, std::string fileName)
{
    string path = "saves/" + fileName;
    std::ofstream logStream(path + ".log");
    for (int i = 0; i < (int)gameEngine->getLog().size(); i++)
    {
        logStream << gameEngine->getLog()[i] << std::endl;
    }

    std::ofstream outputStream(path);
    return save(gameEngine, outputStream);
}

void Saver::save(GameEngine *gameEngine, std::ofstream &outputStream)
{
    outputStream << (gameEngine->getSeed()) << std::endl;

    outputStream << gameEngine->getNumPlayers() << std::endl;
    int numCenters = gameEngine->getCenterPile().size();
    outputStream << numCenters << std::endl;
    outputStream << gameEngine->getGameMode() << std::endl;

    outputStream << gameEngine->getPlayers()->getRoot()->getIndex() << std::endl;

    for (int i = 1; i <= gameEngine->getNumPlayers(); i++)
    {
        outputStream << gameEngine->getPlayer(i)->getName() << std::endl;
        outputStream << gameEngine->getPlayer(i)->getScore() << std::endl;
    }

    std::vector<std::vector<TileType>> centerPiles = gameEngine->getCenterPile();
    for (int j = 0; j < numCenters; j++)
    {
        for (unsigned int i = 0; i < centerPiles[j].size(); ++i)
        {
            outputStream << char(centerPiles[j].at(i));
        }
        outputStream << std::endl;
    }
    int numFactories = gameEngine->getNumPlayers() * 2 + 1;

    for (int i = 0; i < numFactories; ++i)
    {
        outputStream << gameEngine->getFactory(i)->toStringNoSpace() << std::endl;
    }

    for (int i = 1; i <= gameEngine->getNumPlayers(); i++)
    {
        Mosaic *playerMosaic = gameEngine->getPlayer(i)->getMosaic();
        for (int i = 0; i < playerMosaic->getNumLines(); ++i)
        {
            outputStream << playerMosaic->getLine(i)->toString() << std::endl;
        }
        outputStream << playerMosaic->getBrokenTiles()->toString() << std::endl;
        int gameMode = gameEngine->getGameMode();
        bool grayboard = (gameMode == 2) ? true : false;

        outputWall(outputStream, playerMosaic, grayboard);
    }

    outputStream << gameEngine->getLid()->toString() << std::endl;
    outputStream << gameEngine->getBag()->toString() << std::endl;
    outputStream.close();
}

GameEngine *Saver::load(std::string fileName, Menu *menu)
{
    GameEngine *gameEngine = nullptr;
    std::ifstream inputStream("saves/" + fileName);
    if (inputStream.good())
    {
        gameEngine = load(inputStream, menu);
    }

    return gameEngine;
}

GameEngine *Saver::replay(std::string fileName, Menu *menu)
{
    GameEngine *gameEngine = load(fileName, menu);
    GameEngine *replayEngine = nullptr;
    if (gameEngine == nullptr)
    {
        throw "Save file not found";
    }
    else
    {
        bool grayboard = false, sixTiles = false;
        if (gameEngine->getGameMode() == 2)
        {
            grayboard = true;
        }
        if (gameEngine->getGameMode() == 3)
        {
            sixTiles = true;
        }
        replayEngine = new GameEngine(menu, gameEngine->getSeed(), gameEngine->getNumPlayers(), 1, grayboard, sixTiles);

        for (int i = 1; i <= gameEngine->getNumPlayers(); i++)
        {
            std::string playerName = gameEngine->getPlayer(i)->getName();
            replayEngine->addPlayer(playerName);
        }
        replayEngine->setPlayerTurn(1);
        replayEngine->fillBag();
        std::ifstream logStream("saves/" + fileName + ".log");
        if (logStream.good())
        {
            std::string str;
            while (std::getline(logStream, str))
            {
                replayEngine->logTurn(str);
            }
        }
    }
    delete gameEngine;
    gameEngine = nullptr;
    return replayEngine;
}

GameEngine *Saver::load(std::istream &inputStream, Menu *menu)
{
    int numberOfEachTile[NUMBER_OF_COLOURS] = {0};
    bool addedFirstTile = false;
    int currentLine = 0;
    std::string lines[SAVE_FILE_LINES_LENGTH];
    while (inputStream.good() && currentLine < SAVE_FILE_LINES_LENGTH)
    {
        std::getline(inputStream, lines[currentLine++]);
    }

    int seed = stoi(lines[0]);
    int numPlayers = stoi(lines[1]);
    int numFactories = numPlayers * 2 + 1;
    int numCenters = stoi(lines[2]);

    bool grayboard = false, sixTiles = false;
    if (stoi(lines[3]) == 2)
    {
        grayboard = true;
    }
    if (stoi(lines[3]) == 3)
    {
        sixTiles = true;
    }

    GameEngine *gameEngine = new GameEngine(menu, seed, numPlayers, numCenters, grayboard, sixTiles);
    char c = '\0';

    int playerTurnIndex = 0;
    std::istringstream playerTurnStream(lines[4]);
    if (playerTurnStream.good())
    {
        int value;
        playerTurnStream >> value;
        if (value < 1 || value > 4)
        {
            delete gameEngine;
            throw "Player turn id is invalid";
        }
        playerTurnIndex = value;
    }

    int startingLine = 5;

    std::vector<int> playerScores;
    std::vector<std::string> playerNames;
    for (int i = 0; i < numPlayers; i++)
    {
        if (isWhiteSpace(lines[startingLine]))
        {
            delete gameEngine;
            throw "Player name cannot be empty";
        }
        playerNames.push_back(lines[startingLine]);
        int playerScore;
        try
        {
            playerScore = std::stoi(lines[startingLine + 1]);
            playerScores.push_back(playerScore);
        }
        catch (...)
        {
            delete gameEngine;
            throw "Player score is not valid";
        }
        startingLine += 2;
    }

    // Create center factory
    for (int i = 0; i < numCenters; i++)
    {
        std::vector<TileType> centerFactory;
        std::istringstream centerFactoryStream(lines[startingLine]);
        while (centerFactoryStream.get(c))
        {
            if (selectableTile(c) || (c == FIRSTPLAYER))
            {
                if (selectableTile(c))
                    ++numberOfEachTile[getTileIndex(c)];
                addedFirstTile |= c == FIRSTPLAYER;
                centerFactory.push_back(charToTileType(c));
            }
            else
            {
                delete gameEngine;
                throw "Problem reading center factory";
            }
        }
        gameEngine->fillCenterPile(i, centerFactory);
        ++startingLine;
    }

    // Create all factories
    Factory *factories[numFactories];
    for (int i = 0; i < (numFactories); ++i)
    {
        // Create a single factory
        TileType tiles[FACTORY_SIZE] = {NOTILE, NOTILE, NOTILE, NOTILE};
        if (!lines[8 + i + numPlayers].empty())
        {
            std::istringstream factoryStream(lines[startingLine]);
            for (int j = 0; j < FACTORY_SIZE; ++j)
            {
                if (factoryStream.get(c))
                {
                    if (c == NOTILE || selectableTile(c))
                    {
                        if (selectableTile(c))
                            ++numberOfEachTile[getTileIndex(c)];
                        tiles[j] = charToTileType(c);
                    }
                    else
                    {
                        delete gameEngine;
                        cleanUpFactories(factories);
                        throw "A factory contains an invalid tile";
                    }
                }
                else
                {
                    delete gameEngine;
                    cleanUpFactories(factories);
                    throw "A factory is missing some tiles";
                }
            }
            ++startingLine;
        }

        if (isValidFactory(tiles))
            factories[i] = new Factory(tiles);
        else
        {
            delete gameEngine;
            cleanUpFactories(factories);
            throw "A factory is invalid";
        }
    }
    gameEngine->fillFactories(factories);

    for (int i = 0; i < numPlayers; i++)
    {
        Mosaic *playerMosaic = nullptr;
        try
        {
            playerMosaic = generateMosiac(lines, (startingLine), addedFirstTile, numberOfEachTile, sixTiles, grayboard);
            startingLine += 7 + sixTiles;
        }
        catch (const char *errorMessage)
        {
            delete gameEngine;
            throw errorMessage;
        }
        gameEngine->addPlayer(playerNames[i], playerScores[i], playerMosaic);
    }

    // Create lid
    TileList *lid = new TileList();
    std::istringstream lidStream(lines[startingLine]);
    while (lidStream.get(c))
    {
        if (!selectableTile(c))
        {
            delete gameEngine;
            delete lid;
            throw "The lid contains an unselectable tile";
        }
        else
        {
            ++numberOfEachTile[getTileIndex(c)];
            lid->addBack(charToTileType(c));
        }
    }
    ++startingLine;
    gameEngine->fillLid(lid);

    // Create bag
    TileList *bag = new TileList();
    std::istringstream bagStream(lines[startingLine]);
    while (bagStream.get(c))
    {
        if (!selectableTile(c))
        {
            delete gameEngine;
            delete bag;
            throw "The bag contains an unselectable tile";
        }
        else
        {
            ++numberOfEachTile[getTileIndex(c)];
            bag->addBack(charToTileType(c));
        }
    }
    gameEngine->fillBag(bag);

    if (!addedFirstTile)
    {
        delete gameEngine;
        throw "Missing first player tile";
    }

    int sumOfTiles = calculateSumOfTiles(numberOfEachTile) + (addedFirstTile ? 1 : 0);
    int tileSum = sixTiles ? 121 : 101;
    if (sumOfTiles < tileSum)
    {
        delete gameEngine;
        throw "Incorrect number of tiles, not enough tiles in file";
    }
    else if (sumOfTiles > tileSum)
    {
        delete gameEngine;
        throw "Incorrect number of tiles, too many tiles in file";
    }

    if (playerTurnIndex > 0)
        gameEngine->setPlayerTurn(playerTurnIndex);
    return gameEngine;
}

void Saver::outputWall(std::ofstream &outputStream, Mosaic *mosaic, bool grayboard)
{
    for (int row = 0; row < mosaic->getNumLines(); ++row)
    {
        for (int col = 0; col < mosaic->getNumLines(); ++col)
        {
            if (mosaic->getNumLines() == 6)
            {
                if (mosaic->isFilled(row, col))
                    outputStream << char(master_wall_6[row][col]);
                else
                    outputStream << tileTypeToLower(master_wall_6[row][col]);
            }
            else if (!grayboard)
            {
                if (mosaic->isFilled(row, col))
                    outputStream << char(Master_Wall[row][col]);
                else
                    outputStream << tileTypeToLower(Master_Wall[row][col]);
            }
            else
            {
                outputStream << char(mosaic->getWallLine(row, grayboard)[col]);
            }
        }
    }
    outputStream << std::endl;
}

Mosaic *Saver::generateMosiac(std::string lines[SAVE_FILE_LINES_LENGTH], int startingLine, bool &addedFirstTile, int numberOfEachTile[], bool sixTiles, bool grayboard)
{
    Mosaic *mosaic = new Mosaic(sixTiles, grayboard);

    int numLines = 5;
    if (sixTiles)
    {
        ++numLines;
    }
    std::istringstream wallStream(lines[startingLine + (numLines + 1)]);
    for (int row = 0; row < numLines; ++row)
    {
        for (int col = 0; col < numLines; ++col)
        {
            char c = '\0';
            if (wallStream.get(c))
            {
                if (!grayboard)
                {
                    if (selectableTile(std::toupper(c)))
                    {
                        if (std::isupper(c))
                            ++numberOfEachTile[getTileIndex(c)];
                        mosaic->setFilled(row, col, std::isupper(c));
                    }
                    else
                    {
                        delete mosaic;
                        throw "A mosaic wall contains an invalid tile";
                    }
                }
                else
                {
                    if (c != '.')
                    {
                       mosaic->setFilled(row, col, charToTileType(c));
                    }
                    
                }
                
            }
            else
            {
                delete mosaic;
                throw "A mosaic wall is missing tiles";
            }
        }
    }

    for (int i = 0; i < numLines; ++i)
    {
        std::istringstream lineStream(lines[startingLine + i]);
        char c = '\0';
        for (int j = 0; j < i + 1; ++j)
        {
            lineStream >> c;
            TileType toAdd = charToTileType(c);
            if (c == NOTILE || (selectableTile(c) && mosaic->getLine(i)->canAddTiles(toAdd) && (grayboard || !mosaic->isFilled(i, toAdd))))
            {
                if (c != NOTILE)
                    ++numberOfEachTile[getTileIndex(c)];
                mosaic->insertTilesIntoLine(i, 1, toAdd);
            }
            else
            {
                delete mosaic;
                throw "A mosaic contains an invalid line";
            }
        }
    }

    std::istringstream brokenTilesStream(lines[startingLine + numLines]);
    char c = '\0';
    while (brokenTilesStream.get(c))
    {
        if (selectableTile(c) || (c == FIRSTPLAYER && !addedFirstTile))
        {
            if (selectableTile(c))
                ++numberOfEachTile[getTileIndex(c)];
            addedFirstTile |= c == FIRSTPLAYER;
            mosaic->addToBrokenTiles(1, charToTileType(c));
        }
        else
        {
            delete mosaic;
        }
    }

    return mosaic;
}

void Saver::cleanUpFactories(Factory *factories[])
{
    for (int i = 0; i < 9; ++i)
        if (factories[i] != nullptr)
            delete factories[i];
}

bool Saver::isValidFactory(TileType tiles[])
{
    bool allNotile = true;
    bool allSelectable = true;
    for (int i = 0; i < FACTORY_SIZE; ++i)
    {
        allNotile &= tiles[i] == NOTILE;
        allSelectable &= selectableTile(tiles[i]);
    }
    return allNotile || allSelectable;
}

int Saver::getTileIndex(char c)
{
    int index = 0;
    if (c == RED)
        index = 0;
    else if (c == YELLOW)
        index = 1;
    else if (c == DARKBLUE)
        index = 2;
    else if (c == LIGTHBLUE)
        index = 3;
    else if (c == BLACK)
        index = 4;
    return index;
}

int Saver::calculateSumOfTiles(int numberOfEachTile[])
{
    int total = 0;
    for (int i = 0; i < NUMBER_OF_COLOURS; ++i)
        total += numberOfEachTile[i];
    return total;
}
