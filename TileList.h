#ifndef TILELIST_H
#define TILELIST_H

#include "Types.h"
#include "Node.h"
#include <string>

class TileList
{
public:
    TileList();
    ~TileList();
    int size();
    void clear();
    void addFront(TileType tile);
    void addBack(TileType tile);
    TileType removeFront();
    TileType removeBack();
    std::string toString();
    bool empty();
    Node *head;

private:
    Node *tail;
    int length;
};

#endif