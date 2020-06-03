#ifndef PLAYERTREE_H
#define PLAYERTREE_H

#include "Player.h"
#include "PlayerNode.h"
#include "TileList.h"

#include "PlayerTree.h"
#include "Menu.h"

class Menu;

class PlayerTree
{

public:
    PlayerTree();
    ~PlayerTree();
    void addPlayer(Player *player);
    Player *switchPlayerTurn();
    Player *getPlayerWithTurn();
    Player *findPlayer(int index);
    Player *setFirstPlayer();
    Player *setPlayerTurn(int index);
    Player *endRound(TileList *lid, Menu *menu);
    int getNumPlayers();
    Player *winningPlayer();
    PlayerNode *getRoot();
    void printMosaic(Menu *menu);
    void printMosaic(Menu *menu, PlayerNode *node);

private:
    Player *highestBranchScore(PlayerNode *node);
    Player *searchIndex(PlayerNode *node, int index);
    Player *searchWinningPlayer(PlayerNode *node, int score);
    void endRound(TileList *lid, PlayerNode *node, Menu *menu);
    PlayerNode *root;
    int numberOfPlayers;
};

#endif // !PLAYERTREE_H