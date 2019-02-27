#include "Road.h"
#include "Node.h"
#include "Link.h"
#include "Network.h"

Road::Road() : NetworkElement(-1), startNode(nullptr), endNode(nullptr)
{
}

Road::Road(int ID) : NetworkElement(ID), startNode(nullptr), endNode(nullptr)
{
}

Road::~Road()
{
	if (!links.empty())
	{
		links.clear();
	}
}

void Road::setStartNode(Node* _startNode)
{
	startNode = _startNode;
}

Node* Road::getStartNode() const
{
	return startNode;
}

void Road::setEndNode(Node* _endNode)
{
	endNode = _endNode;
}

Node* Road::getEndNode() const
{
	return endNode;
}

size_t Road::getNumOfLinks() const
{
	return links.size();
}

void Road::setLength(const double _length)
{
	length = _length;
}

double Road::getLength() const
{
	return length;
}

LinkMap* Road::getLinks()
{
	return &links;
}

Link* Road::getLink(const int linkID)
{
	if (links.find(linkID) != links.end())
	{
		return links[linkID];
	}
	else
	{
		return nullptr;
	}
}

size_t Road::getNumOfLinks()
{
	return links.size();
}

double Road::calRoadDistanceFromPoint(double pointX, double pointY)
{		
	double minDist;
	double tempDist = 0.0;
	if (!links.empty())
	{
		minDist = 0.0;
		LinkMap::iterator it = links.begin();
		minDist = it->second->calcLinkDistanceFromPoint(pointX, pointY);
		it++;
		for (LinkMap::iterator it1 = it; it1 != links.end(); ++it1)
		{
			tempDist = it1->second->calcLinkDistanceFromPoint(pointX, pointY);
			if (tempDist < minDist)
			{
				minDist = tempDist;
			}
		}
	} 
	else
	{
		minDist = -1.0;
	}
	return minDist;
}

void Road::addLink(const int linkID, Link* link)
{
	if(links.find(linkID) == links.end())
	{	
		links.insert(std::make_pair(linkID, link));
	}
}

void Road::computeLength()
{
	length = 0.0;
	for (LinkMap::iterator it = links.begin(); it != links.end(); ++it)
	{
		length += it->second->getLength();
	}
}