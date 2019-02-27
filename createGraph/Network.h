#ifndef NETWORK_H
#define NETWORK_H

#include "DataTypes.h"

class GeoPos;
class Node;
class Link;
class Road;
class VDS;

class Network
{			
    std::string networkFilename;
    std::string VDSFilename;
    NodeMap nodes;
    LinkMap links;
    RoadMap roads;
    VDSMap vds;
    GeoPos* minPos;
    GeoPos* maxPos;

public:
    /*! Default constructor */
    Network();
    /*! Constructor */
    Network(std::string _networkFilename, std::string _VDSFilename);
    /*! destructor */ 
    ~Network();
    
    /*! Routines for constructing the topology of the network */
    void createNodesAndLinks();
    void createBeforeAfterLinks();
    void createRoads();
    void createVDS();
    void build();

    /*! Setters - Getters */
    size_t getNumOfNodes() const;
    NodeMap* getNodes();
    
    /*! Returns a specific node of the network based on node ID.
     *  @return pointer to the specific Node object
     */
    Node* getNode(const int nodeID);
    
    size_t getNumOfLinks() const;
    LinkMap* getLinks();

    /*! Returns a specific link of the network based on link ID.
     *  @return pointer to the specific Link object
     */
    Link* getLink(const int linkID);
    
    size_t getNumOfVDS() const;
    VDSMap* getVDS();
    VDS* getVDS(const int vdsID);

    size_t getNumOfRoads() const;
    RoadMap* getRoads();
    Road* getRoad(const int roadID);

    void setPosLimits();
    void setMinPos(GeoPos* _minPos);
    void setMaxPos(GeoPos* _maxPos);
    void getPosLimits(GeoPos* _minPos, GeoPos* _maxPos) const;
    void getMinMaxPosCoords(double &minLon, double &maxLon, double &minLat, double &maxLat);
    double getMeanNumOfLinksPerRoad();
    
    void findMinMaxMeanLengthOfLinks(double& minLength, double& maxLength, double& meanLength);
    void findMinMaxMeanLengthOfRoads(double& minLength, double& maxLength, double& meanLength);
    //int GetLinkFromStartAndEndNodeIDs(int startNodeID, int endNodeID);

    Road* addRoad(const int roadID);

    /*! Deletes the Link objects of the network. */
    void deleteNodes();
    void deleteLinks();
    void deleteRoads();
    void deleteVDS();
};

#endif  //  NETWORK_H