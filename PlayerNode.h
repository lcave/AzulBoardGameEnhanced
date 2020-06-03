#ifndef PLAYERNODE_H
#define PLAYERNODE_H

#include "Player.h"

class PlayerNode
{
public:
    PlayerNode(Player *player, int index);
    ~PlayerNode();
    void setLeftNode(PlayerNode *node);
    void setRightNode(PlayerNode *node);
    void setRightNode(Player *player, int index);
    PlayerNode *getLeftNode();
    PlayerNode *getRightNode();
    PlayerNode *getBottomNode();
    Player *getPlayer();
    int getIndex();

private:
    int index;
    Player *player;
    PlayerNode *leftNode;
    PlayerNode *rightNode;
};

#endif