#pragma once
#include "Ressource.h"
class Arbre :
	public Ressource
{
public:

	int taille; // taille de l'arbre, 1, 2 ou 3 (selon la grosseur)


	Arbre(void);
	Arbre::Arbre(string nom, Position pos):Ressource(nom,pos);

	~Arbre(void);

		//
	// CONSTANTES
	//

	char getAffich();
	COLORS getCouleur();

};