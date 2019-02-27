#ifndef NETWORKELEMENT_H
#define NETWORKELEMENT_H

/*! This class is the base for all the elements of a traffic network namely, node, link, road and VDS
 */
class NetworkElement
{
protected:
    /*! The ID of a NetworkElement */
    int ID;
public:
    /*! Default constructor */
    NetworkElement();
    /*! Constructor */
    NetworkElement(int _ID);
    /*! Copy-constructor */
    NetworkElement(const NetworkElement& networkElement);
    /*! Destructor */ 
    virtual ~NetworkElement();
    /*! Copy assignment operator */
    NetworkElement& operator=(const NetworkElement& networkElement);

    /*! Setters - Getters */
    
    /*! Sets the ID of the NetworkElement object
     *  @param ID the NetworkElement object's ID
     *  @return nothing
     */
    void setID(const int _ID);
    
    /*! Returns the ID of the NetworkElement object
     *  @param nothing
     *  @return the ID of the NetworkElement object
     */
    int getID() const;
};

#endif  //  NETWORKELEMENT_H