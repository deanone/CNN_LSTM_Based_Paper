#ifndef NODE_H
#define NODE_H

#include "NetworkElement.h"
#include "DataTypes.h"

class Link;
class GeoPos;

class Node : public NetworkElement
{
    GeoPos* pos;	
    LinkMap incomingLinks;
    LinkMap outgoingLinks;

public:
    /*! Default constructor */
    Node();
    /*! Constructor */
    Node(int ID, double lat, double lon);
    /*! Destructor */ 
    ~Node();
    /*! Copy-constructor */
    Node(const Node& node);
    /*! Copy assignment operator */
    Node& operator=(const Node& node);

    /*! Setters - Getters */
    void setGeoPos(GeoPos* _pos);
    GeoPos* getGeoPos() const;
    double getLat() const;
    double getLon() const;
    size_t getNumOfIncomingLinks() const;
    size_t getNumOfOutgoingLinks() const;
    Link* getIncomingLink(const int incomingLinkID);
    Link* getOutgoingLink(const int outgoingLinkID);
    LinkMap* getIncomingLinks();
    LinkMap* getOutgoingLinks();

    /*! Other members functions */
    void addIncomingLink(const int incomingLinkID, Link* incomingLink);
    void addOutgoingLink(const int outgoingLinkID, Link* outgoingLink); 
    bool isIntermediate(); 
    Link* isIntermediateGetDepar(Link* ArrLink);
};

#endif  // NODE_H