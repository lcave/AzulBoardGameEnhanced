#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "PlayerTree.h"
#include "Factory.h"
#include "Player.h"
#include "TileList.h"
#include "Menu.h"
#include "Utils.h"

#include <string>
#include <sstream>
#include <vector>

using std::string;

class GameEngine
{
public:
    //Constructor
    GameEngine(Menu *menu, int seed, int numPlayers, int numCenters, bool grayboard, bool sixTiles);
    //Destructor
    ~GameEngine();
    //Fill bag with seed and call playGame()
    bool playGameFill();
    // Log successful turn
    void logTurn(std::string turn);
    //Play game
    bool playGame();
    //Play a single round
    bool playRound();
    //Add a new player
    void addPlayer(string name);
    //Add an existing player from save
    void addPlayer(std::string name, int score, Mosaic *mosaic);
    //Helper method for new game
    void addPlayers();
    //Fill bag using seed
    void fillBag();
    //Import already filled bag
    void fillBag(TileList *bag);
    //Import already filled lid
    void fillLid(TileList *lid);
    //Fill center pile with tiles from vector
    void fillCenterPile(int pileNum, std::vector<TileType> centerPile);
    //Draw tiles from bag and fill factories
    void fillFactories(Factory *factories[]);
    //Set a pointer to the player whose turn it is
    void setPlayerTurn(int playerIndex);
    bool hasPlayerWon();

    // Getters
    Factory *getFactory(int);
    Player *getPlayer(int index);
    bool isTurn(Player);
    void shuffleBag();
    bool isPlayer1Turn();
    std::vector<std::vector<TileType>> getCenterPile();
    TileList *getBag();
    TileList *getLid();
    Player *getPlayerTurnID();
    std::vector<std::string> getLog();
    int getSeed();
    void toggleReplay();
    int getNumPlayers();
    PlayerTree* getPlayers();
    int getGameMode();

private:
    void handleInput(bool *inputDone, bool *exit);
    void handleReplay(bool *inputDone, bool *exit);
    bool saveCommand(bool *inputDone, std::stringstream *ss);
    std::string convertCommand(bool *inputDone, std::stringstream *ss);
    std::string convertCommand(bool *inputDone, std::string str);
    std::string turnCommand(bool *inputDone, int factoryNum, int lineNum, TileType colour);
    std::string centerCommand(int factoryNum, bool *inputDone, TileType tileType, int lineNum);
    std::string factoryCommand(bool *inputDone, int factoryNum, int lineNum, TileType tileType);
    void changePlayerTurn();
    int drawFromCenter(int pileNum, TileType colour);
    bool containsFirstPlayer();
    bool roundOver();
    bool factoriesAreEmpty();
    bool centerPileContains(int pileNum, TileType tileType);
    bool validLineNum(int lineNum);
    bool validFactoryNum(int factoryNum);

    int seed;
    int numPlayers;
    int numFactories;
    int numCenters;
    int turnNum;
    bool replay;
    bool grayboard;
    bool sixTiles;
    std::vector<std::string> log;
    PlayerTree *players;
    Factory *factories[9] = {NULL};
    std::vector<std::vector<TileType>> centerPiles;
    Player *playerTurnID;
    TileList *bag;
    TileList *lid;
    Menu *menu;
};

#endif