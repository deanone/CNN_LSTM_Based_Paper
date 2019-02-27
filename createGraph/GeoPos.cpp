#include "GeoPos.h"

GeoPos::GeoPos() : lat(-1.0), lon(-1.0)
{
}

GeoPos::GeoPos(double _lat, double _lon) : lat(_lat), lon(_lon)
{	
}

GeoPos::~GeoPos()
{	
}

GeoPos::GeoPos(const GeoPos& pos) : lat(pos.lat), lon(pos.lon)
{
}

GeoPos& GeoPos::operator=(const GeoPos& pos)
{
	lat = pos.lat;
	lon = pos.lon;
	return *this;
}

void GeoPos::setLat(const double _lat)
{
	lat = _lat;
}

double GeoPos::getLat() const
{
	return lat;
}

void GeoPos::setLon(const double _lon)
{
	lon = _lon;
}

double GeoPos::getLon() const
{
	return lon;
}

bool GeoPos::operator==(const GeoPos &pos)
{
	return lon == pos.lon && lat == pos.lat;
}

bool GeoPos::operator!=(const GeoPos &pos)
{
	return !(*this == pos);
}

GeoPos operator+(const GeoPos &pos1, const GeoPos &pos2)
{
    GeoPos sum = pos1;
    sum.lat  = pos1.lat  + pos2.lat;
    sum.lon = pos1.lon + pos2.lon;
    return sum;
}