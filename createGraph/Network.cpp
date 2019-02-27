#include "Network.h"
#include "Node.h"
#include "Link.h"
#include "VDS.h"
#include "Road.h"
#include "GeoPos.h"

Network::Network() : networkFilename(""), VDSFilename(""), minPos(nullptr), maxPos(nullptr)
{
}

Network::Network(std::string _networkFilename, std::string _VDSFilename) : networkFilename(_networkFilename), VDSFilename(_VDSFilename), minPos(nullptr), maxPos(nullptr)
{
}

Network::~Network()
{
    /*! The Link objects should be deleted before the Node objects because */
    /*! if the Node objects are deleted first then the Node pointers */
    /*! (start node and end node pointers) included in the Link objects will */
    /*! point to deallocated memory until the Links objects are deleted. */
    /*! This may lead to undefined behavior and hence the Link objects */
    /*! should be deleted first. */
    deleteNodes();
    deleteLinks();
    deleteRoads();
    deleteVDS();
    if (minPos != nullptr)
    {
        delete minPos;
    }
    if (maxPos != nullptr)
    {
        delete maxPos;
    }
}

void Network::createNodesAndLinks()
{
    int linkID = -1;
    int startNodeID = -1;
    int endNodeID = -1;
    double startNodeLon = 0.0;
    double startNodeLat = 0.0;
    double endNodeLon = 0.0;
    double endNodeLat = 0.0;
    std::string dataline = "";
    bool firstLine = true;

    std::ifstream in; 
    in.open(networkFilename);
    if (in.is_open())
    {   
        while (std::getline(in, dataline))
        {
            if (firstLine)
            {
                firstLine = false;
                continue;
            }
            std::stringstream ss(dataline);
            std::string item;
            StringVector items;
            while (std::getline(ss, item, ','))
                items.push_back(item);
            linkID = stoi(items[0]);

            /*! start node */
            startNodeID = stoi(items[1]);
            startNodeLon = stod(items[2]);
            startNodeLat = stod(items[3]);
            
            Node* startNode;
            auto it = nodes.find(startNodeID);
            if (it == nodes.end())
            {
                startNode = new Node(startNodeID, startNodeLat, startNodeLon);
                nodes.insert(std::make_pair(startNodeID, startNode));
            }
            else
            {
                startNode = it->second;
            }
            
            /*! end node */
            endNodeID = stoi(items[4]);
            endNodeLon = stod(items[5]);
            endNodeLat = stod(items[6]);
            
            Node* endNode;
            it = nodes.find(endNodeID);
            if (it == nodes.end())
            {
                endNode = new Node(endNodeID, endNodeLat, endNodeLon);
                nodes.insert(std::make_pair(endNodeID, endNode));
            }
            else
            {   
                endNode = it->second;
            }
     
            items.clear();
            
            /*! Create link */
            Link* link = new Link(linkID, startNode, endNode);
            link->computeLength();
            links.insert(std::make_pair(linkID, link));
            startNode->addOutgoingLink(linkID, link);
            endNode->addIncomingLink(linkID, link);
        }
        in.close();
    }
}

void Network::createBeforeAfterLinks()
{
   for (const auto& linkIt : links)
   {
       int linkID = linkIt.first;

       // First examine the start node
       LinkMap* incomingLinks = linkIt.second->getStartNode()->getIncomingLinks();
       LinkMap* outgoingLinks = linkIt.second->getStartNode()->getOutgoingLinks();
       
       // Incoming links of the start node are "before in" links of the link.
       for (LinkMap::iterator it = incomingLinks->begin(); it != incomingLinks->end(); ++it)
       {
           if (it->first != linkID)
           {
               linkIt.second->addBeforeInLink(it->first, it->second);
           }
       }
       // Outgoing links of the start node are "before out" links of the link.
       for (LinkMap::iterator it = outgoingLinks->begin(); it != outgoingLinks->end(); ++it)
       {
           if (it->first != linkID)
           {
               linkIt.second->addBeforeOutLink(it->first, it->second);
           }
       }
           
       // Then examine the end node
       incomingLinks = linkIt.second->getEndNode()->getIncomingLinks();
       outgoingLinks = linkIt.second->getEndNode()->getOutgoingLinks();
       
       // Incoming links of the end node are "after in" links of the link.
       for (LinkMap::iterator it = incomingLinks->begin(); it != incomingLinks->end(); ++it)
       {
           if (it->first != linkID)
           {
               linkIt.second->addAfterInLink(it->first, it->second);
           }
       }
       // Outgoing links of the end node are "after out" links of the link.
       for (LinkMap::iterator it = outgoingLinks->begin(); it != outgoingLinks->end(); ++it)
       {
           if (it->first != linkID)
           {
               linkIt.second->addAfterOutLink(it->first, it->second);
           }
       }
   }
}

void Network::createRoads()
{
    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        Node* startNode = it->second;
        if (!(startNode->isIntermediate()))
        {
            LinkMap* outgoingLinks = startNode->getOutgoingLinks();;
            for (auto it = outgoingLinks->begin(); it != outgoingLinks->end(); ++it)
            {
                Link* startLink = it->second;
                if (startLink->getRoadOfLink() == nullptr)
                {
                    int roadID = static_cast<int>(roads.size());
                    Road* road = addRoad(roadID);;
                    road->setStartNode(startNode);
                    road->addLink(startLink->getID(), startLink);
                    startLink->setRoadOfLink(road);

                    Node* endNode = startLink->getEndNode();
                    Link* endLink = endNode->isIntermediateGetDepar(startLink);
                    while (endLink != nullptr /*&& !eLink->IsLoop()*/)
                    {
                        road->addLink(endLink->getID(), endLink);
                        endLink->setRoadOfLink(road);
                        endNode = endLink->getEndNode();
                        endLink = endNode->isIntermediateGetDepar(endLink);
                    }
                    road->setEndNode(endNode);
                    road->computeLength();
                }
            } 
        }
    }
}

void Network::createVDS()
{
    /*! Load loop detectors coordinates file */
    int vdsID = -1;
    double lat = 0.0;
    double lon = 0.0;
    std::string dataline = "";
    bool firstLine = true;
    std::ifstream in(VDSFilename);
    if (in.is_open())
    {
        while (std::getline(in, dataline))
        {
            // to skip file header
            if (firstLine)
            {
                firstLine = false;
                continue;
            }
            
            std::istringstream ss(dataline);
            std::string item;
            StringVector items;
            while(std::getline(ss, item, ','))  // .csv file so the separator is ","
                items.push_back(item);
            vdsID = stoi(items[0]);
            lat = stod(items[1]);
            lon = stod(items[2]);
            items.clear();
            auto it = vds.find(vdsID);
            if (it == vds.end())
            {
                VDS* pVDS = new VDS(vdsID, lat, lon);
                vds.insert(std::make_pair(vdsID, pVDS));
            }
        }
        in.close();
    }
}

void Network::build()
{
    createNodesAndLinks();
    createBeforeAfterLinks();
    createRoads();
    createVDS();
}

size_t Network::getNumOfNodes() const
{
    return nodes.size();
}

NodeMap* Network::getNodes()
{
    return &nodes;
}

Node* Network::getNode(const int nodeID)
{
    if (nodes.find(nodeID) != nodes.end())
    {
        return nodes[nodeID];
    }
    else
    {
        return nullptr;
    }
}

size_t Network::getNumOfLinks() const
{
    return links.size();
}

LinkMap* Network::getLinks()
{
    return &links;
}

Link* Network::getLink(const int linkID) 
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

size_t Network::getNumOfVDS() const
{
    return vds.size();
}

VDSMap* Network::getVDS()
{
    return &vds;
}

VDS* Network::getVDS(const int vdsID)
{
    if (vds.find(vdsID) != vds.end())
    {
        return vds[vdsID];
    }
    else
    {
        return nullptr;
    }
}

size_t Network::getNumOfRoads() const
{
    return roads.size();
}

RoadMap* Network::getRoads()
{
    return &roads;
}

Road* Network::getRoad(const int roadID)
{
    if (roads.find(roadID) != roads.end())
    {
        return roads[roadID];
    }
    else
    {
        return nullptr;
    }
}

void Network::setPosLimits()
{
    double minLon, maxLon, minLat, maxLat;
    minLon = minLat = 1000000000.0;
    maxLon = maxLat = -1000000000.0;
    for (const auto& n : nodes)
    {
        Node* node = n.second;
        GeoPos* pos = node->getGeoPos();
        double lat = pos->getLat();
        double lon = pos->getLon();
        if (lat < minLat)
        {
            minLat = lat;
        }
        if (lat > maxLat)
        {
            maxLat = lat;
        }

        if (lon < minLon)
        {
            minLon = lon;
        }
        if (lon > maxLon)
        {
            maxLon = lon;
        }
    }
    GeoPos* minPos = new GeoPos(minLat, minLon);
    setMinPos(minPos);
    GeoPos* maxPos = new GeoPos(maxLat, maxLon);
    setMaxPos(maxPos);
}

void Network::setMinPos(GeoPos* _minPos)
{
    minPos = _minPos;
}

void Network::setMaxPos(GeoPos* _maxPos)
{
    maxPos = _maxPos;
}

void Network::getPosLimits(GeoPos* _minPos, GeoPos* _maxPos) const
{
    _minPos = minPos;
    _maxPos = maxPos;
}

void Network::getMinMaxPosCoords(double &minLon, double &maxLon, double &minLat, double &maxLat)
{
    minLon = minPos->getLon();
    maxLon= maxPos->getLon();
    minLat = minPos->getLat();
    maxLat= maxPos->getLat();
}

double Network::getMeanNumOfLinksPerRoad()
{
    double meanNumOfLinksPerRoad = 0.0;
    for (const auto& road : roads)
    {
        meanNumOfLinksPerRoad += road.second->getNumOfLinks();
    }
    meanNumOfLinksPerRoad /= static_cast<double>(roads.size());
    return meanNumOfLinksPerRoad;
}

void Network::findMinMaxMeanLengthOfLinks(double& minLength, double& maxLength, double& meanLength)
{
    meanLength = 0.0;
    auto it = links.begin();
    minLength = maxLength = it->second->getLength();
    it++;
    double length = 0.0;
    for (const auto& link : links)
    {
        length = link.second->getLength();
        meanLength += length;
        if (length < minLength)
        {
            minLength = length;
        }
        if (length > maxLength)
        {
            maxLength = length;
        }
    }
    meanLength /= static_cast<double>(links.size());
}

void Network::findMinMaxMeanLengthOfRoads(double& minLength, double& maxLength, double& meanLength)
{
    meanLength = 0.0;
    auto it = roads.begin();
    minLength = maxLength = it->second->getLength();
    it++;
    double length = 0.0;
    for (const auto& road : roads)
    {
        length = road.second->getLength();
        meanLength += length;
        if (length < minLength)
        {
            minLength = length;
        }
        if (length > maxLength)
        {
            maxLength = length;
        }
    }
    meanLength /= static_cast<double>(roads.size());
}

Road* Network::addRoad(const int roadID)
{
    Road* road = new Road(roadID);
    roads[roadID] = road;
    return road;
}

void Network::deleteNodes()
{
    if (!nodes.empty())
    {
        for (const auto& node : nodes)
        {
            delete node.second;
        }
        nodes.clear();
    }
}

void Network::deleteLinks()
{
    if (!links.empty())
    {
        for (const auto& link : links)
        {
            delete link.second;
        }
        links.clear();
    }
}

void Network::deleteRoads()
{
    if (!roads.empty())
    {
        for (const auto& road : roads)
        {
            delete road.second;
        }
        roads.clear();
    }
}

void Network::deleteVDS()
{
    if (!vds.empty())
    {
        for (const auto& v : vds)
        {
            delete v.second;
        }
        vds.clear();
    }
}

//int Network::getLinkFromStartAndEndNodeIDs(int startNodeID, int endNodeID)
//{
//    Node* SNode = getNode(startNodeID);
//    Node* ENode = getNode(endNodeID);
//    int linkOfNodes = -1;
//    if (SNode != nullptr && ENode != nullptr){
//        for (size_t i = 0; i < SNode->getNumOfOutgoingLinks(); i++){
//            int link1 = SNode->getOutgoingLink(i);
//            for (size_t j = 0 ; j < ENode->getNumOfIncomingLinks() ; j++){
//                int link2 = ENode->getIncomingLink(j);
//                if (link1 == link2)
//                    linkOfNodes = link1;
//            }
//        }
//        for (size_t i = 0 ; i < SNode->getNumOfIncomingLinks() ; i++){
//            int link1 = SNode->getIncomingLink(i);
//            for (size_t j = 0 ; j < ENode->getNumOfOutgoingLinks() ; j++){
//                int link2 = ENode->getOutgoingLink(j);
//                if (link1 == link2)
//                    linkOfNodes = link1;
//            }
//        }
//    }
//    return linkOfNodes;
//}