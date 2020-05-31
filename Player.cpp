#include "Player.h"

Player::Player(std::string name) : Player(name, 0, new Mosaic())
{
}

Player::Player(std::string name, int score, Mosaic *mosaic) : name(name), score(score), mosaic(mosaic)
{
}

Player::~Player()
{
    delete mosaic;
}

std::string Player::getName()
{
    return name;
}

int Player::getScore()
{
    return score;
}

std::vector<TileType> Player::calcScore()
{
    std::vector<TileType> toLid;

    for (int i = 0; i < NUMBER_OF_LINES; i++)
    {
        if (getMosaic()->getLine(i)->getMaxSize() == getMosaic()->getLine(i)->getNumTiles())
        {
            TileType tile = getMosaic()->getLine(i)->getTileType();
            for (int j = 0; j < getMosaic()->getLine(i)->getMaxSize() - 1; j++)
            {
                toLid.push_back(tile);
            }
        }
        score += scoreLine(i);
    }
    int size = mosaic->getBrokenTiles()->size();
    for (int i = 0; i < size; i++)
    {
        if (i <= 1)
        {
            score--;
        }
        else if (i <= 4)
        {
            score -= 2;
        }
        else
        {
            score -= 3;
        }
        TileType tile = mosaic->getBrokenTiles()->removeFront();
        if (tile != FIRSTPLAYER)
        {
            toLid.push_back(tile);
        }
    }

    return toLid;
}

int Player::scoreLine(int lineNum)
{
    int lineScore = 0;
    int numTiles = getMosaic()->getLine(lineNum)->getNumTiles();

    if (getMosaic()->getLine(lineNum)->getMaxSize() == numTiles)
    {
        TileType tile = getMosaic()->getLine(lineNum)->getTileType();
        getMosaic()->getLine(lineNum)->removeTiles();

        for (int i = 0; i < NUMBER_OF_LINES; i++)
        {
            if (master_wall[lineNum][i] == tile)
            {
                mosaic->setFilled(lineNum, i, true);
                lineScore += calcRow(i, lineNum);
                lineScore += calcCol(i, lineNum);
                ++lineScore;
            }
        }
    }
    return lineScore;
}

int Player::calcRow(int index, int line)
{
    int score = 0;
    int pos = index + 1;

    do
    {
        if (mosaic->isFilled(line, pos))
        {
            ++score;
        }
        ++pos;
    } while (pos + 1 < NUMBER_OF_LINES);

    pos = index - 1;
    do
    {
        if (mosaic->isFilled(line, pos))
        {
            ++score;
        }
        --pos;
    } while (pos - 1 >= 0);

    return score;
}

int Player::calcCol(int index, int line)
{
    int score = 0;
    int pos = line + 1;

    do
    {
        if (mosaic->isFilled(pos, index))
        {
            ++score;
        }
        pos++;
    } while (pos + 1 < NUMBER_OF_LINES);

    pos = line - 1;
    do
    {
        if (mosaic->isFilled(pos, index))
        {
            ++score;
        }
        --pos;
    } while (pos - 1 >= 0);

    return score;
}

Mosaic *Player::getMosaic()
{
    return mosaic;
}

bool Player::hasWon()
{
    int line = 0;
    bool hasWon = false;
    while (!hasWon && line < NUMBER_OF_LINES)
    {
        hasWon = true;
        for (int col = 0; col < NUMBER_OF_LINES; ++col)
            hasWon &= mosaic->isFilled(line, col);
        ++line;
    }
    return hasWon;
}

bool Player::hasFirstPlayer()
{
    bool ret = false;
    if (mosaic->getBrokenTiles()->size() != 0 && mosaic->getBrokenTiles()->head->getValue() == FIRSTPLAYER)
    {
        ret = true;
    }
    return ret;
}
