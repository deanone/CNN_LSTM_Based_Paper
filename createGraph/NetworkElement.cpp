#include "NetworkElement.h"

NetworkElement::NetworkElement() : ID(-1)
{
}

NetworkElement::NetworkElement(int _ID) : ID(_ID)
{
}

NetworkElement::NetworkElement(const NetworkElement& networkElement)
{
	ID = networkElement.ID;
}

NetworkElement::~NetworkElement()
{
}

NetworkElement& NetworkElement::operator=(const NetworkElement& networkElement)
{
	ID = networkElement.ID; 
	return *this;
}

void NetworkElement::setID(const int _ID)
{
	ID = _ID;
}

int NetworkElement::getID() const
{
	return ID;
}