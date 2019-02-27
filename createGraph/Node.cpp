#include "Node.h"
#include "Link.h"
#include "GeoPos.h"

Node::Node() : NetworkElement(-1), pos(new GeoPos())
{
}

Node::Node(int ID, double lat, double lon) : NetworkElement(ID), pos(new GeoPos(lat, lon))
{
}

/*! Responsible for freeing the memory for all node, link, road and VDS objects if the Network object. */
/*! The data structures incomingLinks and outgoing links contain only references to these objects, */
/*! and they are not responsible for freeing memory.*/
Node::~Node()
{
    if (!incomingLinks.empty())
    {
        incomingLinks.clear();
    }
    if (!outgoingLinks.empty())
    {
        outgoingLinks.clear();
    }
    /*! Each node object encapsulates a GeoPos object, and therefore it is responsible for freeing it. */
    if (pos != nullptr)
    {
        delete pos;
    }
}

Node::Node(const Node& node) : NetworkElement(node.ID), pos(node.pos)
{
}

Node& Node::operator=(const Node& node)
{
    ID = node.ID;
    pos = node.pos;
    return *this;
}

void Node::setGeoPos(GeoPos* _pos)
{
	pos = _pos;
}

GeoPos* Node::getGeoPos() const
{
	return pos;
}

double Node::getLat() const
{
    return pos->getLat();
}

double Node::getLon() const
{
	return pos->getLon();
}

size_t Node::getNumOfIncomingLinks() const
{
    return incomingLinks.size();
}

size_t Node::getNumOfOutgoingLinks() const
{
    return outgoingLinks.size();
}

Link* Node::getIncomingLink(const int incomingLinkID) 
{
    auto it = incomingLinks.find(incomingLinkID);
    if (it != incomingLinks.end())
    {
        return incomingLinks[incomingLinkID];
    }
    else
    {
        return nullptr;
    }
}

Link* Node::getOutgoingLink(const int outgoingLinkID)
{
    auto it = outgoingLinks.find(outgoingLinkID);
    if (it != outgoingLinks.end())
    {
        return outgoingLinks[outgoingLinkID];
    }
    else
    {
        return nullptr;
    }
}

LinkMap* Node::getIncomingLinks()
{
    return &incomingLinks;
}

LinkMap* Node::getOutgoingLinks()
{
    return &outgoingLinks;
}

void Node::addIncomingLink(const int incomingLinkID, Link* incomingLink)
{
    auto it = incomingLinks.find(incomingLinkID);
    if (it == incomingLinks.end())
    {
        incomingLinks.insert(std::make_pair(incomingLinkID, incomingLink));
    }
}

void Node::addOutgoingLink(const int outgoingLinkID, Link* outgoingLink)
{
    auto it = outgoingLinks.find(outgoingLinkID);
    if (it == outgoingLinks.end())
    {
        outgoingLinks.insert(std::make_pair(outgoingLinkID, outgoingLink));
    }
}

bool Node::isIntermediate()
{
    bool Intermediate = false;
    if ((outgoingLinks.size() == 1) && (incomingLinks.size() == 1))
    {
        auto itOutgoing = outgoingLinks.begin();
        auto itIncoming = incomingLinks.begin();
        if (!(itOutgoing->second->isOppositeOf(itIncoming->second)))
        {
            Intermediate = true;
        }
    }
    else
    {
        if ((outgoingLinks.size() == 2) && (incomingLinks.size() == 2))
        {
            auto itOutgoing1 = outgoingLinks.begin();
            auto itOutgoing2 = itOutgoing1++;
            auto itIncoming1 = incomingLinks.begin();
            auto itIncoming2 = itIncoming1++;
            if ((itOutgoing1->second->isOppositeOf(itIncoming1->second) && itOutgoing2->second->isOppositeOf(itIncoming2->second)) 
                || (itOutgoing1->second->isOppositeOf(itIncoming2->second) && itOutgoing2->second->isOppositeOf(itIncoming1->second)))
            {
                Intermediate = true;
            }
        }
    }
    return Intermediate;
}

Link* Node::isIntermediateGetDepar(Link* ArrLink)
{
  Link* DepLink = nullptr;
  if ((outgoingLinks.size() == 1) && (incomingLinks.size() == 1))
  {
    LinkMap::iterator itOutgoing = outgoingLinks.begin();
    LinkMap::iterator itIncoming = incomingLinks.begin();
    if (!(itOutgoing->second->isOppositeOf(itIncoming->second)))
      DepLink = itOutgoing->second;
  }
  else
  {
    if ((outgoingLinks.size() == 2) && (incomingLinks.size() == 2))
    {
      LinkMap::iterator itOutgoing1 = outgoingLinks.begin();
      LinkMap::iterator itOutgoing2 = itOutgoing1++;
      LinkMap::iterator itIncoming1 = incomingLinks.begin();
      LinkMap::iterator itIncoming2 = itIncoming1++;
      if ((itOutgoing1->second->isOppositeOf(itIncoming1->second) &&
        itOutgoing2->second->isOppositeOf(itIncoming2->second))
        ||
        (itOutgoing1->second->isOppositeOf(itIncoming2->second) &&
        itOutgoing2->second->isOppositeOf(itIncoming1->second)))
      {
        if ((!itOutgoing1->second->isOppositeOf(ArrLink)))
          DepLink = itOutgoing1->second;
        else
        {
          if ((!itOutgoing2->second->isOppositeOf(ArrLink)))
            DepLink = itOutgoing2->second;
        }
      }
    }
  }
  return DepLink;
}