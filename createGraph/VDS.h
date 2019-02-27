#ifndef VDS_H
#define VDS_H

#include "NetworkElement.h"

class GeoPos;

/*! This class represents a VDS, i.e. Vehicle Detection Station. 
 *  A VDS is the measurement point of traffic data in the Caltrans system
 */
class VDS : public NetworkElement
{
    /*! The pointer of the GeoPos object wrapped by the VDS object */
    GeoPos* pos;
public:
    /*! Constructor */
    VDS();
    /*! Default constructor */
    VDS(int ID, double lat, double lon);
    /*! Destructor */
    ~VDS();
    
    /*! Setters - Getters */
    
    /*! sets the pointer to the GeoPos object of the VDS
     *  @param pos a pointer to the GeoPos object of the VDS
     *  @return nothing
     */
    void setGeoPos(GeoPos* _pos);
    
    /*! Returns the pointer to the GeoPos object of the VDS 
     *  @param nothing
     *  @return the pointer to the GeoPos object of the VDS
     */
    GeoPos* getGeoPos() const;
    
    /*! Returns the latitude of the GeoPos object of the VDS
     *  @param nothing
     *  @return the latitude of the GeoPos object of the VDS
     */
    double getLat() const;

    /*! Returns the longitude of the GeoPos object of the VDS
     *  @param nothing
     *  @return the longitude of the GeoPos object of the VDS
     */
    double getLon() const;
};

#endif  //  VDS_H