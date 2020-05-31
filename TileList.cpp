#include "TileList.h"

#include <iostream>

TileList::TileList() : head(nullptr), tail(nullptr), length(0)
{}

TileList::~TileList()
{
    clear();
}

int TileList::size()
{
    return length;
}

void TileList::clear()
{
    Node* currentNode = head;
    while (currentNode != nullptr)
    {
        Node* nextNode = currentNode->getNextNode();
        delete currentNode;
        currentNode = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    length = 0;
}

void TileList::addFront(TileType tile)
{
    head = new Node(tile, head);
    if (tail == nullptr)
        tail = head; 
    ++length;
}

void TileList::addBack(TileType tile)
{
    if (tail == nullptr)
    {
        addFront(tile);
    }
    else
    {
        tail->setNextNode(new Node(tile));
        tail = tail->getNextNode();
        ++length;
    }
}

TileType TileList::removeFront()
{
    TileType removedTile = NOTILE;
    if (head != nullptr)
    {
        Node* toRemove = head;
        head = head->getNextNode();
        if (head == nullptr || head->getNextNode() == nullptr)
            tail = head;
        removedTile = toRemove->getValue();
        delete toRemove; 
        --length;
    }
    return removedTile;
}

TileType TileList::removeBack()
{
    TileType removedTile = NOTILE;
    if (head != tail)
    {
        Node* toRemove = tail;
        Node* currentNode = head;
        while (currentNode->getNextNode() != tail)
        {
            currentNode = currentNode->getNextNode();
        }
        tail = currentNode;
        currentNode->setNextNode(nullptr);
        removedTile = toRemove->getValue();
        delete toRemove;
        --length;
    }
    else removedTile = removeFront();
    return removedTile;
}

std::string TileList::toString()
{
    std::string result = "";
    Node* currentNode = head;
    while (currentNode != nullptr)
    {
        result += char(currentNode->getValue());
        currentNode = currentNode->getNextNode();
    }
    return result;
}
