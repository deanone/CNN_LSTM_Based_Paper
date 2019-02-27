#ifndef ROAD_H
#define ROAD_H

#include "NetworkElement.h"
#include "DataTypes.h"

class Node;
class Link;
class Network;

class Road : public NetworkElement
{
	Node* startNode;
	Node* endNode;	
	LinkMap links;
	double length;
public:
	/*! Default constructor */
	Road();
	/*! Constructor
 	 * @param ID the ID of the road as NetworkElemnt object
 	 */
	Road(int ID);
	/*! Destructor.
 	 */
	~Road();

	/*! Setters - Getters */

	/*! Sets the value of startNode.
 	 * @param node a pointer in an object of type Node.
 	 */
	void setStartNode(Node* startNode);
	
	/*! Returns the value of startNode.
 	 * @return the startNode pointer
 	 */
	Node* getStartNode() const;

	/*! Sets the value of endNode.
 	 * @param node a pointer in an object of type Node.
 	 */
	void setEndNode(Node* _endNode);

	/*! Returns the value of endNode.
 	 * @return the endNode pointer
 	 */
	Node* getEndNode() const;

	/*! Returns the number of links belonging to this road.
 	 * @return the number of links belonging to this road
 	 */
	size_t getNumOfLinks() const;

	/*! Sets the length of the road.
 	 *  @param length the length of the road.
 	 */
	void setLength(const double _length);

	/*! Returns the length of the road.
 	 *  @return the length of the road
 	 */
	double getLength() const;

	LinkMap* getLinks();

	/*! Returns a specific link of the road using an index.
 	 * @param index the index of the link to be returned. The index should a value from 0 to linksPtrVector.size() - 1.
 	 * @return a specific link of the road using index
 	 */
	Link* getLink(const int linkID);
	size_t getNumOfLinks();

	/*! It calculates the distance of the road from a specific point (X, Y).
 	 * @param pointX the longitude of the point.
 	 * @param pointY the latitude of the point.
 	 * @return the distance of the road from the point
 	 */
	double calRoadDistanceFromPoint(double pointX, double pointY);
	void addLink(const int linkID, Link* link);
	void computeLength();
	
};

#endif /*!ROAD_H*/