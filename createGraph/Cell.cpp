#include "Cell.h"
#include "GeoPos.h"
#include "Link.h"

Cell::Cell() : ID(-1), downLeft(nullptr), upRight(nullptr), indexX(-1), indexY(-1)
{
}

Cell::Cell(/*double ddimension,*/ int _ID) : /*dimension(ddimension),*/ ID(_ID), downLeft(nullptr), upRight(nullptr), indexX(-1), indexY(-1)
{
}

Cell::~Cell()
{
    if (downLeft != nullptr)
    {
        delete downLeft;
    }
    if (upRight != nullptr)
    {
        delete upRight;
    }
    if (!linksOfCell.empty())
    {
        linksOfCell.clear();
    }
}

void Cell::setID(const int _ID)
{
    ID = _ID;
}

int Cell::getID() const
{
    return ID;
}

std::vector<Link*>* Cell::getLinksOfCell()
{
    return &linksOfCell;
}

void Cell::setIndexX(const int _indexX)
{
    indexX = _indexX;
}

int Cell::getIndexX() const
{
    return indexX;
}

void Cell::setIndexY(const int _indexY)
{
    indexY = _indexY;
}

int Cell::getIndexY() const
{
    return indexY;
}

void Cell::setDownLeftPos(GeoPos* _downLeft)
{
    downLeft = _downLeft;
}

GeoPos* Cell::getDownLeftPos()
{
    return downLeft;
}

void Cell::setUpRightPos(GeoPos* _upRight)
{
    upRight = _upRight;
}

GeoPos* Cell::getUpRightPos()
{
    return upRight;
}

void Cell::addLink(Link* pLink)
{
    linksOfCell.push_back(pLink);
}