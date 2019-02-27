#include "Grid.h"
#include "Cell.h"
#include "GeoPos.h"
#include "VDS.h"
#include "Node.h"
#include "Link.h"
#include "Network.h"

Grid::Grid(double _dimension, Network* _network) : dimension(_dimension), network(_network), minLat(-1), maxLat(-1), minLon(-1), maxLon(-1), numOfCellsInX(-1), numOfCellsInY(-1)
{
}

Grid::~Grid()
{
    if (!cells.empty())
    {
        for (const auto& v : cells)
        {
            for (const auto& cell : v)
            {
                if (cell != nullptr)
                {
                    delete cell;
                }
            }
        }
    }
}

void Grid::build()
{
    // Find the (lon, lat) = (x,y) limits of the network 
    NodeMap* nodeMap = network->getNodes();
    NodeMap::iterator it = nodeMap->begin();
    minLat = maxLat = it->second->getLat();
    minLon = maxLon = it->second->getLon();
    it++;
    for (auto it2 = it; it2 != nodeMap->end(); ++it2)
    {
        double lat = it2->second->getLat();
        double lon = it2->second->getLon();
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
    minLat -= 0.001;
    maxLat += 0.001;
    minLon -= 0.001;
    maxLon += 0.001;

    // Find number of cells in the x-axis and y-axis
    double diffx = static_cast<double>(fabs(maxLon - minLon));
    // std::cout << "X dimension size: " << diffx << "\n";
    double numOfCellsInX_d = diffx / dimension;
    numOfCellsInX = ceil(numOfCellsInX_d);
    // std::cout << "Num cells in X: " << numOfCellsInX << "\n";
    double diffy = (double)fabs(maxLat - minLat);
    // std::cout << "Y dimension size: " << diffy << "\n";
    double numOfCellsInY_d = diffy / dimension;
    numOfCellsInY = ceil(numOfCellsInY_d);
    // std::cout << "Num cells in Y: " << numOfCellsInY << "\n";
    std::cout << "Total number of cells in the grid: " << numOfCellsInX * numOfCellsInY << "\n";

    // Create Cell objects
    int cellID = 0;
    for (int i = 0; i < numOfCellsInY; i++)
    {
        std::vector<Cell*> cells_temp;
        for (int j = 0; j < numOfCellsInX; j++)
        {
            cellID = i * numOfCellsInX + j;
            Cell* cell = new Cell(/*dimension,*/ cellID);
            cell->setIndexX(j);
            cell->setIndexY(i);
            double lat = minLat + i * dimension;
            double lon = minLon + j * dimension;
            GeoPos* pos = new GeoPos(lat, lon);
            cell->setDownLeftPos(pos);
            pos = new GeoPos(lat + dimension, lon + dimension);
            cell->setUpRightPos(pos);
            cells_temp.push_back(cell);
        }
        cells.push_back(cells_temp);
        cells_temp.clear();
    }
}

void Grid::assignLinksToGrid()
{
    auto linkMap = network->getLinks();
    for (auto it = linkMap->begin(); it != linkMap->end(); ++it)
    {
        Link* pLink = it->second;
        Node* startNode = pLink->getStartNode();
        Node* endNode = pLink->getEndNode();
        Cell* sCell = getCellContainingNode(startNode);
        Cell* eCell = getCellContainingNode(endNode);
        if (sCell->getID() == eCell->getID())
        {
            // The link starts and ends within the same cell
            sCell->addLink(pLink);
        }
        else
        {
            sCell->addLink(pLink);
            eCell->addLink(pLink);
            int minX = std::min(sCell->getIndexX(), eCell->getIndexX());
            int minY = std::min(sCell->getIndexY(), eCell->getIndexY());
            int maxX = std::max(sCell->getIndexX(), eCell->getIndexX());
            int maxY = std::max(sCell->getIndexY(), eCell->getIndexY());

            if (sCell->getIndexX() == eCell->getIndexX())
            {
                // The link starts and ends in cells of equal longitude (X)
                for (int i = minY; i <= maxY; i++)
                    cells[i][minX]->addLink(pLink);
            }
            else
            {
                if (sCell->getIndexY() == eCell->getIndexY())
                {
                    // The link starts and ends in cells of equal latitude (Y)
                    for (int j = minX; j <= maxX; j++)
                        cells[minY][j]->addLink(pLink);
                }
                else
                {
                    GeoPos* sPos = startNode->getGeoPos();
                    GeoPos* ePos = endNode->getGeoPos();
                    double LA = (ePos->getLat() - sPos->getLat()) / (ePos->getLon() - sPos->getLon());
                    double LB = sPos->getLat() - LA * sPos->getLon();

                    // The link starts and ends in cells of both different longitude (X) and latitude (Y)
                    for (int j = minX; j <= maxX; j++)
                    {
                        for (int i = minY; i <= maxY; i++)
                        {
                            Cell* cell = getCell(j, i);
                            if (cell->getID() != sCell->getID() && cell->getID() != eCell->getID())
                            {
                                double cellMinX, cellMinY, cellMaxX, cellMaxY;
                                cellMinX = cell->getDownLeftPos()->getLon();
                                cellMaxX = cell->getUpRightPos()->getLon();
                                cellMinY = cell->getDownLeftPos()->getLat();
                                cellMaxY = cell->getUpRightPos()->getLat();

                                // For each one of the cell's sides, check if it intersects with the link
                                double LY = LA * cellMinX + LB;	// Left side, x = minX
                                if ((LY >= cellMinY) && (LY <= cellMaxY))
                                        cell->addLink(pLink);
                                else
                                {
                                    LY = LA * cellMaxX + LB;	// Right side, x = maxX
                                    if ((LY >= cellMinY) && (LY <= cellMaxY))
                                        cell->addLink(pLink);
                                    else
                                    {
                                        double LX = (cellMinY - LB) / LA;	// Down side, y = minY
                                        if ((LX >= cellMinX) && (LX <= cellMaxX))
                                            cell->addLink(pLink);
                                        else
                                        {
                                            LX = (cellMaxY - LB) / LA;	// Up side, y = maxY
                                            if ((LX >= cellMinX) && (LX <= cellMaxX))
                                                cell->addLink(pLink);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

Cell* Grid::getCell(int indexX, int indexY) const
{
    Cell* cell = nullptr;
    if (indexX >= 0 && indexX < numOfCellsInX && indexY >= 0 && indexY < numOfCellsInY)
    {
        cell = cells[indexY][indexX];
    }
    return cell;
}

Cell* Grid::getCellContainingVDS(VDS* vds) const
{
    GeoPos* pos = vds->getGeoPos();
    double lat = pos->getLat();
    double lon = pos->getLon();
    if ((lat >= minLat && lat <= maxLat) && (lon >= minLon && lon <= maxLon))
    {
        double dY = lat - minLat;
        int i = static_cast<int>(dY / dimension);
        double dX = lon - minLon;
        int j = static_cast<int>(dX / dimension);
        //int cellID = i * numOfCellsInX + j;
        return cells[i][j];
    }
    else
    {
        return nullptr;
    }
}

Cell* Grid::getCellContainingNode(Node* node) const
{
    return getCellContainingPos(node->getGeoPos());
}

Cell* Grid::getCellContainingPos(GeoPos* pos) const
{
    double lat = pos->getLat();
    double lon = pos->getLon();
    if ((lat >= minLat && lat <= maxLat) && (lon >= minLon && lon <= maxLon))
    {
        double dY = lat - minLat;
        int i = static_cast<int>(dY / dimension);
        double dX = lon - minLon;
        int j = static_cast<int>(dX / dimension);
        //int cellID = i * numOfCellsInX + j;
        return cells[i][j];
    }
    else
    {
        return nullptr;
    }
}