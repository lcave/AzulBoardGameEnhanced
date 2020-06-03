#include "PlayerNode.h"

PlayerNode::PlayerNode(Player *player, int index)
{
    this->player = player;
    this->index = index;
    leftNode = nullptr;
    rightNode = nullptr;
}

PlayerNode::~PlayerNode()
{
    delete player;
    player = nullptr;
    if (leftNode != nullptr)
    {
        delete leftNode;
    }
    if (rightNode != nullptr)
    {
        delete rightNode;
    }
}

Player *PlayerNode::getPlayer()
{
    return player;
}

void PlayerNode::setLeftNode(PlayerNode *node)
{
    if (node == nullptr)
    {
        leftNode = node;
    }
    if (leftNode == nullptr)
    {
        leftNode = node;
    }
    else
    {
        leftNode->setLeftNode(node);
    }
}

void PlayerNode::setRightNode(PlayerNode *node)
{
    if (node == nullptr)
    {
        rightNode = node;
    }
    else if (rightNode == nullptr)
    {
        rightNode = node;
    }
    else
    {
        rightNode->setRightNode(node);
    }
}

void PlayerNode::setRightNode(Player *player, int index)
{
    if (rightNode == nullptr)
    {
        rightNode = new PlayerNode(player, index);
    }
    else
    {
        rightNode->setRightNode(player, index);
    }
}

PlayerNode *PlayerNode::getLeftNode()
{
    return leftNode;
}

PlayerNode *PlayerNode::getRightNode()
{
    return rightNode;
}

PlayerNode *PlayerNode::getBottomNode()
{
    PlayerNode *tmp = this;
    if (leftNode != nullptr)
    {
        tmp = leftNode->getBottomNode();
        if (tmp == leftNode)
        {
            leftNode = nullptr;
        }
    }
    return tmp;
}

int PlayerNode::getIndex()
{
    return index;
}
