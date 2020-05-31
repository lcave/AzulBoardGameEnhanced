#ifndef GAMEENGINE_H
#define GAMEENGINE_H

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
    GameEngine(Menu *menu, int seed);
    //Destructor
    ~GameEngine();
    //Fill bag with seed and call playGame()
    bool playGame();
    // Log successful turn
    void logTurn(std::string turn);
    //Play game
    bool playGame(bool replay);
    //Play a single round
    bool playRound(bool replay);
    //Add a new player
    Player *addPlayer(string name);
    //Add an existing player from save
    Player *addPlayer(std::string name, int score, Mosaic *mosaic);
    //Helper method for new game
    void addPlayers();
    //Fill bag using seed
    void fillBag();
    //Import already filled bag
    void fillBag(TileList *bag);
    //Import already filled lid
    void fillLid(TileList *lid);
    //Fill center pile with tiles from vector
    void fillCenterPile(std::vector<TileType> centerPile);
    //Draw tiles from bag and fill factories
    void fillFactories(Factory *factories[]);
    //Set a pointer to the player whose turn it is
    void setPlayerTurn(int playerIndex);

    // Getters
    Factory *getFactory(int);
    Player *getPlayer(int playerIndex);
    bool isTurn(Player);
    void shuffleBag();
    bool isPlayer1Turn();
    std::vector<TileType> getCenterPile();
    TileList *getBag();
    TileList *getLid();
    Player *getPlayerTurnID();
    std::vector<std::string> getLog();
    int getSeed();

private:
    void handleInput(bool *inputDone, bool *exit);
    void handleReplay(bool *inputDone, bool *exit);
    bool saveCommand(bool *inputDone, std::stringstream *ss);
    std::string convertCommand(bool *inputDone, std::stringstream *ss);
    std::string convertCommand(bool *inputDone, std::string str);
    std::string turnCommand(bool *inputDone, int factoryNum, int lineNum, TileType colour);
    std::string centerCommand(bool *inputDone, TileType tileType, int lineNum);
    std::string factoryCommand(bool *inputDone, int factoryNum, int lineNum, TileType tileType);
    void changePlayerTurn();
    bool hasPlayerWon();
    int drawFromCenter(TileType colour);
    bool containsFirstPlayer();
    bool roundOver();
    bool factoriesAreEmpty();
    bool centerPileContains(TileType tileType);
    bool validLineNum(int lineNum);
    bool validFactoryNum(int factoryNum);

    int seed;
    int turnNum;
    std::vector<std::string> log;
    std::vector<Player *> players;
    Factory *factories[NUM_FACTORIES];
    std::vector<TileType> centerPile;
    Player *playerTurnID;
    TileList *bag;
    TileList *lid;
    Menu *menu;
};

#endif