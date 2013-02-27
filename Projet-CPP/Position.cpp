#include "Position.h"


Position::Position(void)
{
	this->x = 0;
	this->y = 0;
}


Position::~Position(void)
{
}


bool Position::isCoordonneesValide(int x, int y)
{
	return x%2 == y%2;
}


int Position::getX(void)
{
	return this->x;
}


void Position::setX(int x)
{
	if(this->isCoordonneesValide(x,this->getY())) {
		this->x = x;
	} else {
		leverErreurMauvaiseCoordonnees(x, this->getY());
	}
}


int Position::getY(void)
{
	return this->y;
}


void Position::setY(int Y)
{
	if(this->isCoordonneesValide(y,this->getX())) {
		this->y = y;
	} else {
		leverErreurMauvaiseCoordonnees(this->getX(), y);
	}
}


void Position::setCoordonnees(int x, int y)
{
	if(this->isCoordonneesValide(x,y)) {
		this->x = x;
		this->y = y;
	} else {
		leverErreurMauvaiseCoordonnees(x, y);
	}
}



void Position::leverErreurMauvaiseCoordonnees(int x, int y)
{
	string msg ="Erreur : Les coordonnees ne serait plus correctes x=[" + x;
	msg += "] y=[" + y ;
	msg += "]";

	throw logic_error(msg);
}