#include "Player.h"
#include "Menu.h"

Player::Player(std::string name, bool sixtiles, bool grayboard)
{
    this->name = name;
    score = 0;
    mosaic = new Mosaic(sixtiles, grayboard);
}

Player::Player(std::string name, int score, Mosaic *mosaic)
{
    this->name = name;
    this->score = score;
    this->mosaic = mosaic;
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

std::vector<TileType> Player::calcScore(bool grayboard, Menu &menu)
{
    std::vector<TileType> toLid;
    for (int i = 0; i < mosaic->getNumLines(); i++)
    {
        if (getMosaic()->getLine(i)->getMaxSize() == getMosaic()->getLine(i)->getNumTiles())
        {
            TileType tile = getMosaic()->getLine(i)->getTileType();
            for (int j = 0; j < getMosaic()->getLine(i)->getMaxSize() - 1; j++)
            {
                toLid.push_back(tile);
            }
        }
        score += scoreLine(i, grayboard, menu);
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
            score = score - 2;
        }
        else if (i <= 6)
        {
            score = score - 3;
        }
        else if (i == 7)
        {
            score = score - 4;
        }
        TileType tile = mosaic->getBrokenTiles()->removeFront();
        if (tile != FIRSTPLAYER)
        {
            toLid.push_back(tile);
        }
    }

    return toLid;
}

int Player::scoreLine(int lineNum, bool grayboard, Menu &menu)
{
    int lineScore = 0;
    int numTiles = getMosaic()->getLine(lineNum)->getNumTiles();

    if (getMosaic()->getLine(lineNum)->getMaxSize() == numTiles)
    {
        TileType tile = getMosaic()->getLine(lineNum)->getTileType();

        if (grayboard)
        {
            bool valid = false;
            while (!valid)
            {
                std::string str = "Enter position to place tiles from line ";
                str.append(std::to_string(lineNum + 1));
                menu.printMessage(str);
                menu.printMosaic(this);
                std::stringstream ss(menu.getInput());
                int row, col;
                ss >> row >> col;
                if ((row > 0 && row < 7) && (col > 0 && col < 7))
                {
                    bool validPlacement = true;
                    for (int i = 0; i < NUMBER_OF_LINES && validPlacement; i++)
                    {
                        if (mosaic->getWallLine(i, grayboard)[col] == tile)
                        {
                            validPlacement = false;
                        }
                    }
                    if (validPlacement)
                    {
                        mosaic->setFilled(row - 1, col - 1, tile);
                        lineScore += calcRow(col, lineNum);
                        lineScore += calcCol(col, lineNum);
                        ++lineScore;
                        valid = true;
                    }
                }
            }
        }
        getMosaic()->getLine(lineNum)->removeTiles();
        if (!grayboard)
        {
            for (int i = 0; i < mosaic->getNumLines(); i++)
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
    }
    return lineScore;
}

int Player::calcRow(int index, int line)
{
    int score = 0;
    int pos = index + 1;
    int lines = mosaic->getNumLines();

    while (pos + 1 < lines)
    {
        if (mosaic->isFilled(line, pos))
        {
            ++score;
        }
        ++pos;
    }

    pos = index - 1;
    while (pos - 1 >= 0)
    {
        if (mosaic->isFilled(line, pos))
        {
            ++score;
        }
        --pos;
    }

    return score;
}

int Player::calcCol(int index, int line)
{
    int score = 0;
    int pos = line + 1;
    int lines = mosaic->getNumLines();

    do
    {
        if (mosaic->isFilled(pos, index))
        {
            ++score;
        }
        pos++;
    } while (pos + 1 < lines);

    pos = line - 1;
    while (pos - 1 >= 0)
    {
        if (mosaic->isFilled(pos, index))
        {
            ++score;
        }
        --pos;
    }

    return score;
}

Mosaic *Player::getMosaic()
{
    return mosaic;
}

bool Player::hasWon(bool grayboard)
{
    int line = 0;
    bool hasWon = false;
    int lines = mosaic->getNumLines();
    while (!hasWon && line < lines)
    {
        hasWon = true;
        for (int col = 0; col < lines; ++col)
            if (!grayboard)
            {
                hasWon &= mosaic->isFilled(line, col);
            }
            else
            {
                hasWon &= (mosaic->isFilled(line, col, grayboard) != NOTILE);
            }

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
