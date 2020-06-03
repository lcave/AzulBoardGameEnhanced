#include "PlayerTree.h"

PlayerTree::PlayerTree()
{
    root = nullptr;
    numberOfPlayers = 0;
}

PlayerTree::~PlayerTree()
{
    delete root;
}

void PlayerTree::addPlayer(Player *player)
{
    ++numberOfPlayers;
    if (root == nullptr)
    {
        root = new PlayerNode(player, numberOfPlayers);
    }
    else
    {
        root->setRightNode(player, numberOfPlayers);
    }
}

Player *PlayerTree::getPlayerWithTurn()
{
    return root->getPlayer();
}

Player *PlayerTree::switchPlayerTurn()
{
    PlayerNode *tmp = root;
    if (root->getRightNode() == nullptr)
    {
        root = tmp->getBottomNode();
        do
        {
            root->setRightNode(tmp->getBottomNode());
        } while (tmp->getLeftNode() != nullptr);
    }
    else
    {
        root = root->getRightNode();
        tmp->setRightNode(nullptr);
        root->setLeftNode(tmp);
    }
    return root->getPlayer();
}

Player *PlayerTree::setFirstPlayer()
{
    if (root->getPlayer()->hasFirstPlayer())
    {
        while (root->getLeftNode() != nullptr)
        {
            PlayerNode *tmp = root->getBottomNode();
            if (tmp != root)
            {
                root->setRightNode(tmp);
            }
        }
    }
    else
    {
        PlayerNode *tmp = root;
        root = tmp->getLeftNode();
        tmp->setLeftNode(nullptr);
        if (root == nullptr)
        {
            root = tmp->getRightNode();
            tmp->setRightNode(nullptr);
        }
        root->setRightNode(tmp);
        setFirstPlayer();
    }
    return root->getPlayer();
}

Player *PlayerTree::setPlayerTurn(int index)
{
    if (root->getIndex() != index)
    {
        PlayerNode *tmp = root;
        root = tmp->getLeftNode();
        if (root == nullptr)
        {
            root = tmp->getRightNode();
        }
        tmp->setRightNode(nullptr);
        root->setRightNode(tmp);
        if (root->getIndex() == index)
        {
            do
            {
                tmp = root->getBottomNode();
                if (tmp != root)
                {
                    root->setRightNode(tmp);
                }
            } while (root->getLeftNode() != nullptr);
        }
        else
        {
            tmp = root;
            root = tmp->getLeftNode();
            if (root == nullptr)
            {
                root = tmp->getRightNode();
                tmp->setRightNode(nullptr);
            }
            root->setRightNode(tmp);
            setPlayerTurn(index);
        }
    }
    return root->getPlayer();
}

Player *PlayerTree::endRound(TileList *lid, Menu *menu)
{
    Player *player = setFirstPlayer();
    endRound(lid, root, menu);
    return player;
}

void PlayerTree::endRound(TileList *lid, PlayerNode *node, Menu *menu)
{
    if (node != nullptr)
    {
        Player *player = node->getPlayer();
        std::vector<TileType> lidTiles = player->calcScore();
        for (TileType tile : lidTiles)
        {
            lid->addBack(tile);
        }
        menu->printScore(player->getName(), player->getScore());
        endRound(lid, node->getLeftNode(), menu);
        endRound(lid, node->getRightNode(), menu);
    }
}

Player *PlayerTree::findPlayer(int index)
{
    return searchIndex(root, index);
}

Player *PlayerTree::searchIndex(PlayerNode *node, int index)
{
    Player *ret = nullptr;
    if (node != nullptr)
    {
        if (node->getIndex() == index)
        {
            ret = node->getPlayer();
        }
        else
        {
            ret = searchIndex(node->getRightNode(), index);
            if (ret == nullptr)
            {
                ret = searchIndex(node->getLeftNode(), index);
            }
        }
    }
    return ret;
}

Player *PlayerTree::winningPlayer()
{
    return highestBranchScore(root);
}

Player *PlayerTree::highestBranchScore(PlayerNode *node)
{
    Player *ret = nullptr;
    int currentScore = node->getPlayer()->getScore(), rightScore = 0, leftScore = 0;
    if (node->getRightNode() != nullptr)
    {
        rightScore = highestBranchScore(node->getRightNode())->getScore();
    }
    if (node->getLeftNode() != nullptr)
    {
        leftScore = highestBranchScore(node->getLeftNode())->getScore();
    }
    if (rightScore > leftScore)
    {
        if (rightScore > currentScore)
        {
            ret = node->getRightNode()->getPlayer();
        }
        else
        {
            ret = node->getPlayer();
        }
    }
    else if (leftScore > rightScore)
    {
        if (leftScore > currentScore)
        {
            ret = node->getLeftNode()->getPlayer();
        }
        else
        {
            ret = node->getPlayer();
        }
    }
    else if (leftScore == rightScore)
    {
        if (currentScore > rightScore)
        {
            ret = node->getPlayer();
        }
        else
        {
            ret = nullptr;
        }
    }
    return ret;
}

int PlayerTree::getNumPlayers()
{
    return numberOfPlayers;
}

PlayerNode *PlayerTree::getRoot()
{
    return root;
}
