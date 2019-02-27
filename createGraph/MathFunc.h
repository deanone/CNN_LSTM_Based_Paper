#include "DataTypes.h"

namespace mfnc
{
	/*! This function converts decimal degrees to radians */
	double deg2rad(double deg);
	/*!  This function converts radians to decimal degrees */
	double rad2deg(double rad);
	/*!
	 * Calculates the distance between two points.
	 * @param x1 the X coordinate of the first point.
	 * @param y1 the Y coordinate of the first point.
	 * @param x2 the X coordinate of the second point.
	 * @param y2 the Y coordinate of the second point.
	 */
	double calcPointsDistance(double x1, double y1, double x2, double y2);
}