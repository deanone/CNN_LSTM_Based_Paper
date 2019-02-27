#include "VDS.h"
#include "GeoPos.h"

VDS::VDS() : NetworkElement(-1), pos(new GeoPos())
{
}

VDS::VDS(int ID, double lat, double lon) : NetworkElement(ID), pos(new GeoPos(lat, lon))
{
}

VDS::~VDS()
{
	if (pos != nullptr)
	{
		delete pos;
	}
}

void VDS::setGeoPos(GeoPos* _pos)
{
	pos = _pos;
}

GeoPos* VDS::getGeoPos() const
{
	return pos;
}

double VDS::getLat() const
{
	return pos->getLat();
}

double VDS::getLon() const
{
	return pos->getLon();
}