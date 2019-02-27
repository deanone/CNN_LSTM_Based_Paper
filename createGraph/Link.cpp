#include "Link.h"
#include "Node.h"
#include "MathFunc.h"

Link::Link() : NetworkElement(-1), startNode(nullptr), endNode(nullptr), length(0.0), direction(oneway), oppositeLink(nullptr), roadOfLink(nullptr)
{
}

Link::Link(int ID, Node* _startNode, Node* _endNode) : NetworkElement(ID), startNode(_startNode), endNode(_endNode), length(0.0), direction(oneway), oppositeLink(nullptr), roadOfLink(nullptr)
{
}

Link::~Link()
{
    if (!beforeInLinks.empty())
    {
        beforeInLinks.clear();
    }
    if (!beforeOutLinks.empty())
    {
        beforeOutLinks.clear();
    }
    if (!afterInLinks.empty())
    {
        afterInLinks.clear();
    }
    if (!afterOutLinks.empty())
    {
        afterOutLinks.clear();
    }
}

Link::Link(const Link& link) : NetworkElement(link.ID), startNode(link.startNode), endNode(link.endNode), length(link.length), direction(link.direction), oppositeLink(link.oppositeLink), roadOfLink(link.roadOfLink)
{
}

Link& Link::operator=(const Link& link)
{
    ID = link.ID;
    startNode = link.startNode;
    endNode = link.endNode;
    length = link.length; 
    direction = link.direction;
    oppositeLink = link.oppositeLink;
    roadOfLink = link.roadOfLink;
    return *this;
}

void Link::setNodes(Node* _startNode, Node* _endNode)
{
    startNode = _startNode;
    endNode = _endNode;
}

void Link::getNodes(Node* _startNode, Node* _endNode) const
{
    _startNode = startNode;
    _endNode = endNode;
}

Node* Link::getStartNode() const
{
    return startNode;
}

Node* Link::getEndNode() const
{
    return endNode;
}

void Link::setLength(const double _length)
{
    length = _length;
}

double Link::getLength() const
{
    return length;
}

void Link::setDirection(const Direction _direction)
{
    direction = _direction;
}

Direction Link::getDirection() const
{
    return direction;
}

void Link::setOppositeLink(Link* _oppositeLink)
{
    oppositeLink = _oppositeLink;
}

Link* Link::getOppositeLink() const
{
    return oppositeLink;
}

LinkMap* Link::getBeforeInLinks()
{
    return &beforeInLinks;
}

LinkMap* Link::getBeforeOutLinks()
{
    return &beforeOutLinks;
}

LinkMap* Link::getAfterInLinks()
{
    return &afterInLinks;
}

LinkMap* Link::getAfterOutLinks()
{
    return &afterOutLinks;
}

size_t Link::getNumOfBeforeInLinks() 
{
    return beforeInLinks.size();
}

size_t Link::getNumOfBeforeOutLinks()
{
    return beforeOutLinks.size();
}

size_t Link::getNumOfAfterInLinks()
{
    return afterInLinks.size();
}

size_t Link::getNumOfAfterOutLinks()
{
    return afterOutLinks.size();
}
void Link::setRoadOfLink(Road* _roadOfLink)
{
    roadOfLink = _roadOfLink;
}

Road* Link::getRoadOfLink() const 
{
    return roadOfLink;
}

void Link::computeLength()
{
    length = mfnc::calcPointsDistance(startNode->getLon(), startNode->getLat(), endNode->getLon(), endNode->getLat());
}

/*! Distance between link and point (implementation 1) */
/*! Refer to 2002_Greenfeld, equations (1)-(4) */
double Link::calcLinkDistanceFromPoint(double pointX, double pointY)
{
    // verX and verY stand for vertical X and vertical Y. 
    // They are the coordinates of the vertical projection of point on the Link.
    double m = 0.0;
    double verX = 0.0;
    double verY = 0.0;
    double distance = 0.0;
    double distS = 0.0;
    double distE = 0.0;

    double startNodeLon = startNode->getLon();
    double startNodeLat = startNode->getLat();
    double endNodeLon = endNode->getLon();
    double endNodeLat = endNode->getLat();

    double minLon = std::min(startNodeLon, endNodeLon);
    double maxLon = std::max(startNodeLon, endNodeLon);
    double minLat = std::min(startNodeLat, endNodeLat);
    double maxLat = std::max(startNodeLat, endNodeLat);

    if (endNodeLon == startNodeLon)
    {
        if (endNodeLat == startNodeLat) // The link is a point and so the distance is the distance between two points
        {
            distance = mfnc::calcPointsDistance(startNodeLon, startNodeLat, pointX, pointY);
        }
        else    // The link is parallel to the y-axis
        {
            verX = startNodeLon;
            verY = pointY;
            if ((verX >= minLon) && (verX <= maxLon) && (verY >= minLat) && (verY <= maxLat))
            {
                distance = mfnc::calcPointsDistance(verX, verY, pointX, pointY);
            }
            else
            {
                distS = mfnc::calcPointsDistance(pointX, pointY, startNodeLon, startNodeLat);
                distE = mfnc::calcPointsDistance(pointX, pointY, endNodeLon, endNodeLat);
                distance = std::min(distS, distE);
            }
        }
    }
    else
    {
        // m = (y2 - y1) / (x2 - x1) (coefficient of the straight line that corresponds to Link)
        m = (endNodeLat - startNodeLat) / (endNodeLon - startNodeLon);
        verX = (m * (pointY - startNodeLat + m * startNodeLon) + pointX) / (m * m + 1.0);
        verY = startNodeLat + m * (verX - startNodeLon);
        if ((verX >= minLon) && (verX <= maxLon) && (verY >= minLat) && (verY <= maxLat))
        {
            distance = mfnc::calcPointsDistance(verX, verY, pointX, pointY);
        }
        else
        {
            distS = mfnc::calcPointsDistance(pointX, pointY, startNodeLon, startNodeLat);
            distE = mfnc::calcPointsDistance(pointX, pointY, endNodeLon, endNodeLat);
            distance = std::min(distS, distE);
        }
    }       
    return distance;
}

void Link::addBeforeInLink(const int beforeInLinkID, Link* beforeInLink)
{
   if (beforeInLinks.find(beforeInLinkID) == beforeInLinks.end())
   {
       beforeInLinks.insert(std::make_pair(beforeInLinkID, beforeInLink));
   }
}

void Link::addBeforeOutLink(const int beforeOutLinkID, Link* beforeOutLink)
{
   if (beforeOutLinks.find(beforeOutLinkID) == beforeOutLinks.end())
   {
       beforeOutLinks.insert(std::make_pair(beforeOutLinkID, beforeOutLink));
   }
}

void Link::addAfterInLink(const int afterInLinkID, Link* afterInLink)
{
   if (afterInLinks.find(afterInLinkID) == afterInLinks.end())
   {
       afterInLinks.insert(std::make_pair(afterInLinkID, afterInLink));
   }
}

void Link::addAfterOutLink(const int afterOutLinkID, Link* afterOutLink)
{
   if (afterOutLinks.find(afterOutLinkID) == afterOutLinks.end())
   {
       afterOutLinks.insert(std::make_pair(afterOutLinkID, afterOutLink));
   }
}

bool Link::isOppositeOf(Link *pLink)
{
    if (oppositeLink == pLink)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// void Link::computeLength()
// {
//     double startNodeLat_d = startNode->getLat();
//     double startNodeLon_d = startNode->getLon();
//     double endNodeLat_d = endNode->getLat();
//     double endNodeLon_d = endNode->getLon();
//     double startNodeLat_r, startNodeLon_r, endNodeLat_r, endNodeLon_r, u, v;
//     startNodeLat_r = deg2rad(startNodeLat_d);
//     startNodeLon_r = deg2rad(startNodeLon_d);
//     endNodeLat_r = deg2rad(endNodeLat_d);
//     endNodeLon_r = deg2rad(endNodeLon_d);
//     u = sin((endNodeLat_r - startNodeLat_r)/2);
//     v = sin((endNodeLon_r - startNodeLon_r)/2);
//     length = 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(startNodeLat_r) * cos(endNodeLat_r) * v * v));
// }

/*!
 * This function checks if a point (X,Y) is covered by the link.
 * @param the X coordinate of the point.
 * @param the Y coordinate of the point.
 */
// TODO: check where it is used
//bool Link::IsPointCovered(double pointX, double pointY)
//{
//    bool covered = false;
//    // verX and verY stand for vertical X and vertical Y. They are the coordinates of the vertical projection of point on the Link.
//    double m = 0.0;
//    double verX = 0.0;
//    double verY =  0.0;
//
//    double startNodeLon = startNode->GetLon();
//    double startNodeLat = startNode->GetLat();
//    double endNodeLon = endNode->GetLon();
//    double endNodeLat = endNode->GetLat();
//
//    double minLon = std::min(startNodeLon, endNodeLon);
//    double maxLon = std::max(startNodeLon, endNodeLon);
//    double minLat = std::min(startNodeLat, endNodeLat);
//    double maxLat = std::max(startNodeLat, endNodeLat);
//
//    if (endNodeLon == startNodeLon)
//    {
//        if (endNodeLat == startNodeLat)	// The link is a point
//                covered = false;
//        else // The link is parallel to the y-axis
//        {
//            verY = pointY;
//            if ((verY >= minLat) && (verY <= maxLat))
//                covered = true;
//            else
//                covered = false;
//        }
//    }
//    else
//    {
//        // m = (y2 - y1)/(x2 - x1)
//        m = (endNodeLat - startNodeLat) / (endNodeLon - startNodeLon);
//        verX = (m * (pointY - startNodeLat + m * startNodeLon) + pointX) / (m * m + 1.0);
//        verY = startNodeLat + m * (verX - startNodeLon);
//        if ((verX >= minLon) && (verX <= maxLon) && (verY >= minLat) && (verY <= maxLat))
//            covered = true;
//        else
//            covered = false;
//    }		
//    return covered;	
//}

//bool Link::StartEndCoincide()
//{
//    bool seCoincide = false;
//    if ((startNode->GetLon() == endNode->GetLon()) && (startNode->GetLat() == endNode->GetLat()))
//        seCoincide = true;
//    return seCoincide;
//}

// Checks if the current link is the opposite of a given one (implementation 1)
// TODO: check where it is used
//bool Link::IsOppositeOf(Link *pLink)
//{
//    if (oppositeLink == pLink)
//        return true;
//    else
//        return false;
//}

//checks if the current link is the opposite of a given one (implementation 2)
// bool Link::IsOppositeOf(Link *pLink)
// {
// 	if ((startNode() == pLink->getEndNode()) && (endNode == pLink->getStartNode()))
// 		return true;
// 	else
// 		return false;
// }

// Creates a new link as the opposite (in direction) from a given one
// TODO: check where it is used
//void Link::MakeOppositeOf(Link& srLink)
//{
//    startNode = srLink.endNode;
//    endNode = srLink.startNode;
//    startNode->AddLink(this->GetID(), OUTGOING_LINK);
//    endNode->AddLink(this->GetID(), INCOMING_LINK);
//    length = srLink.length;
//    if (srLink.dir == forward)
//            dir = backwards;
//    else if(srLink.dir == backwards)
//            dir = forward;
//    srLink.oppositeLink = this;
//    oppositeLink = &srLink;
//}

// void Link::GetMiddlePointCoordinates(double& lonDeg, double& latDeg)
// {
//    lonDeg = 0.5 * (startNode->GetLon() + endNode->GetLon()) / 10000.0;
//    latDeg  = 0.5 * (startNode->GetLon() + endNode->GetLat()) / 10000.0;
// }