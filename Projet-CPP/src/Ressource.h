#pragma once
#include "element.h"
class Ressource :
	public Element
{
public:

	//
	// CONSTANTES
	//

	char getAffich();
	COLORS getCouleur();


	Ressource(void);
	Ressource(string nom, Position pos);
	~Ressource(void);

};