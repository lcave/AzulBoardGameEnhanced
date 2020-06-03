#include "Saver.h"

// need to put this in gameEngine
TileType Master_Wall[5][5]{DARKBLUE, YELLOW, RED, BLACK, LIGTHBLUE,
                           LIGTHBLUE, DARKBLUE, YELLOW, RED, BLACK,
                           BLACK, LIGTHBLUE, DARKBLUE, YELLOW, RED,
                           RED, BLACK, LIGTHBLUE, DARKBLUE, YELLOW,
                           YELLOW, RED, BLACK, LIGTHBLUE, DARKBLUE};

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
        for (int i = 0; i < NUMBER_OF_LINES; ++i)
        {
            outputStream << playerMosaic->getLine(i)->toString() << std::endl;
        }
        outputStream << playerMosaic->getBrokenTiles()->toString() << std::endl;

        outputWall(outputStream, playerMosaic);
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
        replayEngine = new GameEngine(menu, gameEngine->getSeed(), gameEngine->getNumPlayers(), 1);

        for (int i = 1; i <= gameEngine->getNumPlayers(); i++)
        {
            replayEngine->addPlayer(gameEngine->getPlayer(i)->getName());
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
    GameEngine *gameEngine = new GameEngine(menu, seed, numPlayers, numCenters);
    char c = '\0';

    int playerTurnIndex = 0;
    std::istringstream playerTurnStream(lines[3]);
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

    std::vector<int> playerScores;
    std::vector<std::string> playerNames;
    for (int i = 0; i < numPlayers; i++)
    {
        if (isWhiteSpace(lines[i * 2 + 4]))
        {
            delete gameEngine;
            throw "Player name cannot be empty";
        }
        playerNames.push_back(lines[i * 2 + 4]);
        int playerScore;
        try
        {
            playerScore = std::stoi(lines[i * 2 + 5]);
            playerScores.push_back(playerScore);
        }
        catch (...)
        {
            delete gameEngine;
            throw "Player score is not valid";
        }
    }

    // Create center factory
    for (int i = 0; i < numCenters; i++)
    {
        std::vector<TileType> centerFactory;
        std::istringstream centerFactoryStream(lines[i + 4 + numPlayers * 2]);
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
    }

    // Create all factories
    Factory *factories[numFactories];
    for (int i = 0; i < (numFactories); ++i)
    {
        // Create a single factory
        TileType tiles[FACTORY_SIZE] = {NOTILE, NOTILE, NOTILE, NOTILE};
        if (!lines[8 + i + numPlayers].empty())
        {
            std::istringstream factoryStream(lines[4 + numCenters + i + (numPlayers * 2)]);
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
            playerMosaic = generateMosiac(lines, (4 + numCenters + numFactories + (numPlayers * 2) + (7 * i)), addedFirstTile, numberOfEachTile);
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
    int pos = 4 + (7 * numPlayers) + numFactories + (numPlayers * 2) + numCenters;
    std::istringstream lidStream(lines[pos]);
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
    gameEngine->fillLid(lid);

    // Create bag
    TileList *bag = new TileList();
    std::istringstream bagStream(lines[pos + 1]);
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
    if (sumOfTiles < 101)
    {
        delete gameEngine;
        throw "Incorrect number of tiles, not enough tiles in file";
    }
    else if (sumOfTiles > 101)
    {
        delete gameEngine;
        throw "Incorrect number of tiles, too many tiles in file";
    }

    if (playerTurnIndex > 0)
        gameEngine->setPlayerTurn(playerTurnIndex);
    return gameEngine;
}

void Saver::outputWall(std::ofstream &outputStream, Mosaic *mosaic)
{
    for (int row = 0; row < NUMBER_OF_LINES; ++row)
    {
        for (int col = 0; col < NUMBER_OF_LINES; ++col)
        {
            if (mosaic->isFilled(row, col))
                outputStream << char(Master_Wall[row][col]);
            else
                outputStream << tileTypeToLower(Master_Wall[row][col]);
        }
    }
    outputStream << std::endl;
}

Mosaic *Saver::generateMosiac(std::string lines[SAVE_FILE_LINES_LENGTH], int startingLine, bool &addedFirstTile, int numberOfEachTile[])
{
    Mosaic *mosaic = new Mosaic();
    std::istringstream wallStream(lines[startingLine + 6]);
    for (int row = 0; row < NUMBER_OF_LINES; ++row)
    {
        for (int col = 0; col < NUMBER_OF_LINES; ++col)
        {
            char c = '\0';
            if (wallStream.get(c))
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
                delete mosaic;
                throw "A mosaic wall is missing tiles";
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_LINES; ++i)
    {
        std::istringstream lineStream(lines[startingLine + i]);
        char c = '\0';
        for (int j = 0; j < i + 1; ++j)
        {
            lineStream >> c;
            TileType toAdd = charToTileType(c);
            if (c == NOTILE || (selectableTile(c) && mosaic->getLine(i)->canAddTiles(toAdd) && !mosaic->isFilled(i, toAdd)))
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

    std::istringstream brokenTilesStream(lines[startingLine + 5]);
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
            if (addedFirstTile)
                throw "Duplicate first player tile found in a mosaic";
            throw "There is a problem in the broken tiles";
        }
    }

    return mosaic;
}

void Saver::cleanUpFactories(Factory *factories[])
{
    for (int i = 0; i < NUM_FACTORIES; ++i)
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
