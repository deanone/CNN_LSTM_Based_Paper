#include <omp.h>

#include "DataTypes.h"
#include "Network.h"
#include "VDS.h"
#include "Grid.h"
#include "Cell.h"
#include "Link.h"
#include "Road.h"

std::string getExecutablePath()
{
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    return std::string( result, (count > 0) ? count : 0 );
}

/*!
 *Function that concatenates the path of the project's executable with a filename.
 *@return the concatenation of the path of the project's executable with a filename.
 */
std::string getExecutablePathAndMatchItWithFilename(std::string fileName)
{
    std::string execPath = getExecutablePath();
    size_t found = execPath.find_last_of("/");
    execPath = execPath.substr(0, found);
    std::stringstream ss;
    ss << execPath << "/" << fileName;
    return ss.str();
}

Network* loadNetwork()
{
    std::string networkFilename = getExecutablePathAndMatchItWithFilename("Map/CALTRANS_ALLCALI.csv");
    std::string VDSFilename = getExecutablePathAndMatchItWithFilename("VDS.csv");
    Network* network = new Network(networkFilename, VDSFilename);
    network->build();
    return network;
}

void matchVDSToRoads_Greedy(Network* network, std::string outFilename)
{
    std::cout << "Map-matching VDS to links...\n";
    LinkMap* links = network->getLinks();
    VDSMap* vds = network->getVDS();
    std::map<int, int> vdsID_roadID;
    
    clock_t startTime = clock();
    for (auto it = vds->begin(); it != vds->end(); ++it)
    {
        int vdsID = it->first;
        double lat = it->second->getLat();
        double lon = it->second->getLon();
        auto it1 = links->begin();
        double minDistance = it1->second->calcLinkDistanceFromPoint(lon, lat);
        Link* linkOfVDS = it1->second;
        ++it1;
        for (auto it2 = it1; it2 != links->end(); ++it2)
        {
            double distance = it2->second->calcLinkDistanceFromPoint(lon, lat);
            if (distance < minDistance)
            {
                minDistance = distance;
                linkOfVDS = it2->second;
            }
        }
        int roadID = -1;
        Road* roadOfVDS = linkOfVDS->getRoadOfLink();
        if (roadOfVDS != nullptr)
        {
            roadID = roadOfVDS->getID();
            vdsID_roadID.insert(std::make_pair(vdsID, roadID));   
        }
    }
    clock_t endTime = clock();
    std::cout << "Matched!\n";
    
    // Write VDS ID - link ID pairs into file
    std::ofstream out(outFilename);
    for (const auto& x : vdsID_roadID)
    {
        int vdsID = x.first;
        int roadID = x.second;
        out << vdsID << "," << roadID << std::endl;
    }
    out.close();

    double elapsedTimeInSecs = (double)(endTime - startTime) / (double)CLOCKS_PER_SEC;
    double elapsedTimeInMins = elapsedTimeInSecs / 60.0;
    std::cout << "Elapsed Time in seconds: " << elapsedTimeInSecs << std::endl;
    std::cout << "Elapsed Time in minutes: " << elapsedTimeInMins << std::endl;
}

void matchVDSToRoads_PIC(Network* network, double dimension, std::string outFilename, int numThreads)
{
    std::cout << "Map-matching VDS to links...\n";
    // Create Grid
    Grid* grid = new Grid(dimension, network);
    grid->build();
    // Assign links to Grid
    grid->assignLinksToGrid();
    // Match VDS to links
    VDSMap* vds = network->getVDS();
    std::map<int, int> vdsID_roadID;
    int i;

/********************************************************************************** Parallel section ******************************************************************************************************/
    double start = omp_get_wtime();
#pragma omp parallel for num_threads(numThreads) private(i)
    for (i = 0; i < vds->size(); i++)
    {
        auto it = vds->begin();
        std::advance(it, i);
        VDS* vds = it->second;
        Cell* cell = grid->getCellContainingVDS(vds);
        #pragma omp critical
        {
            if (cell != nullptr)
            {
                std::vector<Link*>* linksOfCell = cell->getLinksOfCell();
                if (linksOfCell->size() > 0)
                {
                    double minDistance = -1.0;
                    double distance = -1.0;
                    auto it1 = linksOfCell->begin();
                    Link* link = *it1;
                    Link* linkOfVDS = link;
                    distance = link->calcLinkDistanceFromPoint(vds->getLon(), vds->getLat());
                    minDistance = distance;
                    it1++;
                    for (auto it2 = it1; it2 != linksOfCell->end(); ++it2)
                    {
                        link = *it2;
                        distance = link->calcLinkDistanceFromPoint(vds->getLon(), vds->getLat());
                        if (distance < minDistance)
                        {
                            minDistance = distance;
                            linkOfVDS = link;
                        }
                    }
                    int roadID = -1;
                    Road* roadOfVDS = linkOfVDS->getRoadOfLink();
                    if (roadOfVDS != nullptr)
                    {
                        roadID = roadOfVDS->getID();
                        vdsID_roadID.insert(std::make_pair(vds->getID(), roadID));
                    }
                }
            }
        }
    }
    double end = omp_get_wtime();
/********************************************************************************** End of parallel section ***********************************************************************************************/
    std::cout << "Matched!\n";
    std::cout << "Elapsed time: " << end - start << std::endl;
    
    delete grid;

    // Write VDS ID - link ID pairs into file
    std::ofstream out(outFilename);
    for (const auto& x : vdsID_roadID)
        out << x.first << "," << x.second << std::endl;
    out.close();
}

int main()
{
    Network* network = loadNetwork();
    int choice1 = 0;
    std::cout << "Match VDS to roads (1) or check network's info (2) or create adjacency matrix of graph (3)?\n";
    std::cin >> choice1;

    if (choice1 == 1)
    {
        std::string outFilename = getExecutablePathAndMatchItWithFilename("VDS_Roads");
        int choice2 = 0;
        std::cout << "Method 1 (Naive/Greedy) or 2 (PIC)?\n";
        std::cin >> choice2;

        if (choice2 == 1)
        {
            matchVDSToRoads_Greedy(network, outFilename);
        }
        else if (choice2 == 2)
        {
            // always initialize the variables (even with trivial values)
            double minLengthOfLink = 0.0;
            double maxLengthOfLink = 0.0;
            double meanLengthOfLink = 0.0;
            network->findMinMaxMeanLengthOfLinks(minLengthOfLink, maxLengthOfLink, meanLengthOfLink);
            double divideWith = 0.0;
            int numThreads = 1;

            // User input
            std::cout << "Give the number by which the maximum link length will be divided\n";
            std::cin >> divideWith;
            std::cout << "Give number of threads\n";
            std::cin >> numThreads;

            double dimension = maxLengthOfLink / divideWith; // the most crucial point, determine the size of the cells in the grid
            matchVDSToRoads_PIC(network, dimension, outFilename, numThreads);
        }
    }
    else if (choice1 == 2)
    {
        double minLon = -1.0;
        double maxLon = -1.0;
        double minLat = -1.0;
        double maxLat = -1.0;
        double minLengthOfLink = -1.0;
        double maxLengthOfLink = -1.0;
        double meanLengthOfLink = -1.0;
        double minLengthOfRoad = -1.0;
        double maxLengthOfRoad = -1.0;
        double meanLengthOfRoad = -1.0;

        network->setPosLimits();
        network->getMinMaxPosCoords(minLon, maxLon, minLat, maxLat);
        network->findMinMaxMeanLengthOfLinks(minLengthOfLink, maxLengthOfLink, meanLengthOfLink);
        network->findMinMaxMeanLengthOfRoads(minLengthOfRoad, maxLengthOfRoad, meanLengthOfRoad);
        size_t numOfNodes = network->getNumOfNodes();
        size_t numOfLinks = network->getNumOfLinks();
        size_t numOfRoads = network->getNumOfRoads();
        size_t numOfVDS = network->getNumOfVDS();
        double lonSize = maxLon - minLon;
        double latSize = maxLat - minLat;
        double meanNumOfLinksPerRoad = network->getMeanNumOfLinksPerRoad();

        std::cout << "Network info:\n";
        std::cout << "minLat: " << minLat << std::endl;
        std::cout << "minLon: " << minLon << std::endl;

        std::cout << "maxLat: " << maxLat << std::endl;
        std::cout << "maxLon: " << maxLon << std::endl;
        
        std::cout << "latSize: " << latSize << std::endl;
        std::cout << "lonSize: " << lonSize << std::endl;
        
        std::cout << "Nodes: " << numOfNodes << std::endl;
        std::cout << "Links: " << numOfLinks << std::endl;
        std::cout << "Roads: " << numOfRoads << std::endl;
        std::cout << "Mean num of links per road: " << meanNumOfLinksPerRoad << std::endl;
        std::cout << "VDS: " << numOfVDS << std::endl;

        std::cout << "Min link length: " << minLengthOfLink << std::endl;
        std::cout << "Max link length: " << maxLengthOfLink << std::endl;
        std::cout << "Mean link length: " << meanLengthOfLink << std::endl;
 
        std::cout << "Min road length: " << minLengthOfRoad << std::endl;
        std::cout << "Max road length: " << maxLengthOfRoad << std::endl;
        std::cout << "Mean road length: " << meanLengthOfRoad << std::endl;
    }
    else if (choice1 == 3)
    {
        std::string outFilename = getExecutablePathAndMatchItWithFilename("graph_adjacency_matrix.csv");
        std::ofstream out(outFilename);
        LinkMap* links = network->getLinks();
        for (LinkMap::iterator linkIt = links->begin(); linkIt != links->end(); ++linkIt)
        {
            out << linkIt->first;

            size_t numOfBeforeInLinks = linkIt->second->getNumOfBeforeInLinks();
            size_t numOfBeforeOutLinks = linkIt->second->getNumOfBeforeOutLinks();
            size_t numOfBeforeLinks = numOfBeforeInLinks + numOfBeforeOutLinks;
            out << "," << numOfBeforeLinks;

            size_t numOfAfterInLinks = linkIt->second->getNumOfAfterInLinks();
            size_t numOfAfterOutLinks = linkIt->second->getNumOfAfterOutLinks();
            size_t numOfAfterLinks = numOfAfterInLinks + numOfAfterOutLinks;
            out << "," << numOfAfterLinks;

            LinkMap* beforeInLinks = linkIt->second->getBeforeInLinks();
            for (LinkMap::iterator it = beforeInLinks->begin(); it != beforeInLinks->end(); ++it)
            {
                out << "," << it->first;
            }

            LinkMap* beforeOutLinks = linkIt->second->getBeforeOutLinks();
            for (LinkMap::iterator it = beforeOutLinks->begin(); it != beforeOutLinks->end(); ++it)
            {
                out << "," << it->first;
            }

            LinkMap* afterInLinks = linkIt->second->getAfterInLinks();
            for (LinkMap::iterator it = afterInLinks->begin(); it != afterInLinks->end(); ++it)
            {
                out << "," << it->first;
            }

            LinkMap* afterOutLinks = linkIt->second->getAfterOutLinks();
            for (LinkMap::iterator it = afterOutLinks->begin(); it != afterOutLinks->end(); ++it)
            {
                out << "," << it->first;
            }

            out << std::endl;

        }
        out.close();
    } 

    delete network;
    return 0;
}