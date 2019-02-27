#ifndef CELL_H
#define CELL_H

#include "DataTypes.h"

class GeoPos;
class Link;

/*! This class represents a Cell, which is a rectangle.
 *  The Cell is the structural element of the Grid. 
 */
class Cell
{
    // double dimension;
    /*! The ID of a Cell object */
    int ID;
    /*! A pointer to a GeoPos object representing the down left point of the cell */
    GeoPos* downLeft;
    /*! A pointer to a GeoPos object representing the up right point of the cell */
    GeoPos* upRight;
    std::vector<Link*> linksOfCell;
    /*! The index that runs on x-axis corresponding to the index of the columns of the Grid */
    int indexX;
    /*! The index that runs on y-axis corresponding to the index of the rows of the Grid */
    int indexY;	
public:
    /*! Default constructor */
    Cell();
    /*! Constructor */
    Cell(/*double ddimension, */int _ID);
    /*! Destructor */
    ~Cell();
    
    /*! Setters - Getters */
    // void SetDimension(const double dimension) {this->dimension = dimension;}
    // double GetDimension() const {return dimension;}
    
    /*! Sets the ID of the Cell object
     *  @param ID the Cell object's ID
     *  @return nothing
     */
    void setID(const int _ID);
    
    /*! Returns the ID of the Cell object
     *  @param nothing
     *  @return the ID of the Cell object
     */
    int getID() const;
    
    /*! Returns a pointer to the vector of pointers to Link objects of the Cell object
     *  @param nothing
     *  @return a pointer to the vector of pointers to Link objects of the Cell object
     */
    std::vector<Link*>* getLinksOfCell();

    /*! Sets the indexX of the Cell object
     *  @param indexX the value of the indexX
     *  @return nothing
     */
    void setIndexX(const int _indexX);
    
    /*! Returns the indexX of the Cell object
     *  @param nothing
     *  @return the value of the indexX
     */
    int getIndexX() const;
    
    /*! Sets the indexY of the Cell object
     *  @param indexY the value of the indexY
     *  @return nothing
     */
    void setIndexY(const int _indexY);
    
    /*! Returns the indexY of the Cell object
     *  @param nothing
     *  @return the value of the indexY
     */
    int getIndexY() const;
    
    /*! Sets the pointer to the down left GeoPos object of the Cell object
     *  @param downLeft a pointer to the down left GeoPos object
     *  @return nothing
     */
    void setDownLeftPos(GeoPos* _downLeft);
    
    /*! Returns the pointer to the down left GeoPos object of the Cell object 
     *  @param nothing
     *  @return the pointer to the down left GeoPos object of the Cell object
     */
    GeoPos* getDownLeftPos();
    
    /*! Sets the pointer to the up right GeoPos object of the Cell object
     *  @param upRight a pointer to the up right GeoPos object
     *  @return nothing
     */
    void setUpRightPos(GeoPos* _upRight);
    
    /*! Returns the pointer to the up right GeoPos object of the Cell object 
     *  @param nothing
     *  @return the pointer to the up right GeoPos object of the Cell object
     */
    GeoPos* getUpRightPos();
    
    /*! Adds a pointer of a Link object to the vector of pointers of Link objects of the Cell object
     *  @param pLink pointer to a Link object
     *  @return nothing
     */
    void addLink(Link* pLink);
};

#endif  //  CELL_H