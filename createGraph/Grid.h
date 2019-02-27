#ifndef GRID_H
#define GRID_H

#include "DataTypes.h"

class Cell;
class GeoPos;
class VDS;
class Node;
class Network;

class Grid
{
    Network* network;
    std::vector< std::vector<Cell*> > cells;
    double dimension;
    double minLat;
    double maxLat;
    double minLon;
    double maxLon;
    int numOfCellsInX;
    int numOfCellsInY;
public:
    /*! Default constructor */
    Grid();
    /*! Constructor */
    Grid(double _dimension, Network* _network);
    /*! Destructor */
    ~Grid();

    /*! Setters - Getters */
    Cell* getCell(int indexX, int indexY) const;
    Cell* getCellContainingPos(GeoPos* pos) const;
    Cell* getCellContainingVDS(VDS* vds) const;
    Cell* getCellContainingNode(Node* node) const;
    
    /*! Other functions */
    void build();
    void assignLinksToGrid();
    void assignVDSToGrid();
};

#endif  //  GRID_H