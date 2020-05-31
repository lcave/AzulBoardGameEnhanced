#ifndef NODE_H
#define NODE_H

#include "Types.h"

class Node
{
    public:
        Node(TileType value);
        Node(TileType value, Node* nextNode);
        ~Node();
        void setNextNode(Node* nextNode);
        Node* getNextNode();
        TileType getValue();

    private:
        TileType value;
        Node* nextNode;
};

#endif