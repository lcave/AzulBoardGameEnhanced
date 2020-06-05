#ifndef PLAYERTREE_H
#define PLAYERTREE_H

#include "Player.h"
#include "PlayerNode.h"
#include "TileList.h"
#include "Menu.h"

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
    Player *endRound(TileList *lid, Menu *menu, bool grayboard);
    int getNumPlayers();
    Player *winningPlayer();
    PlayerNode *getRoot();
    void printMosaic(Menu *menu);
    void printMosaic(Menu *menu, PlayerNode *node);

private:
    PlayerNode *highestBranchScore(PlayerNode *node);
    Player *searchIndex(PlayerNode *node, int index);
    void endRound(TileList *lid, PlayerNode *node, Menu *menu, bool grayboard);
    PlayerNode *root;
    int numberOfPlayers;
};

#endif // !PLAYERTREE_H