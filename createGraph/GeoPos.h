#ifndef GEOPOS_H
#define GEOPOS_H

/*! This class represents a 2-dimensional (Geo)graphical (Pos)ition. 
 *  Such a point is uniquely characterized by its latitude and its longitude.
 */
class GeoPos
{
    /*! The latitude of the point */
    double lat; 
    /*! The longitude of the point */
    double lon; 
public:
    /*! Default constructor */
    GeoPos();
    /*! Constructor */
    GeoPos(double _lat, double _lon);
    /*! Destructor */
    ~GeoPos();
    /*! Copy-constructor */
    GeoPos(const GeoPos& pos);
    /*! Copy assignment operator */
    GeoPos& operator=(const GeoPos& pos);
    
    /*! Setters - Getters */

    /*! sets the latitude of the GeoPos object
     *  @param lat the latitude of the GeoPos object
     *  @return nothing
     */
    void setLat(const double _lat);

    /*! Returns the latitude of the GeoPos object
     *  @param nothing
     *  @return the latitude of the GeoPos object
     */
    double getLat() const;

    /*! sets the longitude of the GeoPos object
     *  @param lon the longitude of the GeoPos object
     *  @return nothing
     */
    void setLon(const double _lon);
    
    /*! Returns the longitude of the GeoPos object
     *  @param nothing
     *  @return the longitude of the GeoPos object
     */
    double getLon() const;
    
    /*! Operator overloading */

    /*! The == operator overloading function for objects of the GeoPos class
     *  @param the right GeoPos operand
     *  @return a boolean value indicating if the compared values are equal
     */
    bool operator==(const GeoPos &pos);
    
    /*! The != operator overloading function for objects of the GeoPos class
     *  @param the right GeoPos operand
     *  @return a boolean value indicating if the compared values are not equal
     */
    bool operator!=(const GeoPos &pos);
    
    /*! Friend operator overloading function for the + operator with GeoPos objects
     *  @param pos1 the left operand
     *  @param pos2 the right operand
     *  @return a GeoPos object as the result of the + operator on two GeoPos objects
     */
    friend GeoPos operator+(const GeoPos &pos1, const GeoPos &pos2);
};

#endif  //  GEOPOS_H