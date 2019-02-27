#ifndef LINK_H
#define LINK_H

#include "NetworkElement.h"
#include "DataTypes.h"

class Node;
class Road;

class Link : public NetworkElement
{
    Node* startNode;
    Node* endNode;
    double length;
    Direction direction;
    Link* oppositeLink;
    Road* roadOfLink;
    /*! neighbors */
    LinkMap beforeInLinks;
    LinkMap beforeOutLinks;
    LinkMap afterInLinks;
    LinkMap afterOutLinks;
public:
    /*! Default constructor */
    Link();
    /*! Constructor */
    Link(int ID, Node* _startNode, Node* _endNode);	
    /*! Destructor */
    ~Link();
    /*! Copy-constructor */
    Link(const Link& link);
    /*! Copy assignment operator */
    Link& operator=(const Link& link);

    /*! Setters - Getters */
    void setNodes(Node* _startNode, Node* _endNode);
    void getNodes(Node* _startNode, Node* _endNode) const;
    Node* getStartNode() const;
    Node* getEndNode() const;
    void setLength(const double _length);
    double getLength() const;
    void setDirection(const Direction _direction);
    Direction getDirection() const;
    void setOppositeLink(Link* _oppositeLink);
    Link* getOppositeLink() const;
    LinkMap* getBeforeInLinks();
    LinkMap* getBeforeOutLinks();
    LinkMap* getAfterInLinks();
    LinkMap* getAfterOutLinks();
    size_t getNumOfBeforeInLinks();
    size_t getNumOfBeforeOutLinks();
    size_t getNumOfAfterInLinks();
    size_t getNumOfAfterOutLinks();
    void setRoadOfLink(Road* _roadOfLink);
    Road* getRoadOfLink() const;

    /*! Other members functions */

    /*!
     * Returns the length of the link based on Haversine formula.
     * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
     * @return The length of the link in kilometers
     */
    void computeLength();
    double calcLinkDistanceFromPoint(double pointX, double pointY);
    void addBeforeInLink(const int beforeInLinkID, Link* beforeInLink);
    void addBeforeOutLink(const int beforeOutLinkID, Link* beforeOutLink);
    void addAfterInLink(const int afterInLinkID, Link* afterInLink);
    void addAfterOutLink(const int afterOutLinkID, Link* afterOutLink);
    bool isOppositeOf(Link *pLink);

//    bool IsPointCovered(double pointX, double pointY);
//    bool StartEndCoincide();
//    bool IsOppositeOf(Link* pLink);
//    void MakeOppositeOf(Link& srLink);
//    void getMiddlePointCoordinates(double& lonDeg, double& latDeg);
        
};

#endif  // LINK_H