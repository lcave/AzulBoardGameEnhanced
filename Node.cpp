#include "Node.h"

Node::Node(TileType value) : Node(value, nullptr)
{}

Node::Node(TileType value, Node* nextNode) : value(value), nextNode(nextNode)
{}

Node::~Node()
{}

void Node::setNextNode(Node* nextNode)
{
    this->nextNode = nextNode;
}

Node* Node::getNextNode()
{
    return nextNode;
}

TileType Node::getValue()
{
    return value;
}
