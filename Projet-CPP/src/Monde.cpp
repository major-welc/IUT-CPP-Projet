
#include "Monde.h"
#include "Config.h"
#include "Mobile.h"
#include "Arbre.h"
#include "Sanglier.h"
#include "Gaulois.h"
#include "Gauloise.h"
#include "Village.h"

#include <iostream>
#include <algorithm>

Monde::Monde(void) : vector<Element*>(), compteurCompactage(0)
{
}


Monde::~Monde(void)
{
}

map<Position, unsigned int>& Monde::getCarte() {
	return carte;
}

bool Monde::supprimerElement(Element* e) {

	// On recherche l'index de l'�l�ment
	int indexElement = -1;
	for(unsigned int i = 0; i < this->size(); i++) {
		if((*this)[i] == e) {
			indexElement = i;
			break;
		}
	}

	if(indexElement < 0) {
		return false;
	}

	// On met l'�l�ment � NULL dans le vecteur
	std::replace(this->begin(), this->end(), e, (Element*) NULL);
	// On le supprime de la map
	this->carte.erase(e->getPosition());

	compteurCompactage++;
	
	if(compteurCompactage == Config::frequence_compactage_map) {

		OutputDebugString(L"Compactage !");

		vector<Element *> *nouvelleListe = new vector<Element *>();

		// On cr�er une nouvelle liste
		for(unsigned int i = 0; i < this->size(); i++) {
			if((*this)[i] != NULL) {
				nouvelleListe->push_back((*this)[i]);
			}
		}

		// On met la liste compact�e
		this->swap(*nouvelleListe);
		delete nouvelleListe;

		// On r�cr�er toute la map
		this->carte.clear();
		for(unsigned int i = 0; i < this->size(); i++) {
			Element* t = (*this)[i];
			this->carte[t->getPosition()] = i;
		}

		compteurCompactage = 0;
	}

	return true;
}

void Monde::ajouterElementPositionAleatoireEtVide(Element *e) {

	Position pos(-1, -1);
	int i = 0;
	int max = Config::dimentions.first *  Config::dimentions.second;

	do {
		pos = Position::random(Config::dimentions.first, Config::dimentions.second);
		i++;
	} while (Monde::getInstance()->isCaseLibre(pos) == false && i < max) ;

	// Si la grille est pleine
	if(i >= max) {
		OutputDebugString(L"Grille pleine !");
	}
	// Si la position a �t� trouv�e
	else {
		Monde::getInstance()->ajouterElement(pos, e);
	}
}

// Ajout l'�l�ment au monde
// Doit �tre plac� plus tard
void Monde::ajouterElement(Position pos, Element* e) {
	this->push_back(e);
	int id = getIndexDansListe(e);
	this->carte[pos] = id;
	e->setPosition(pos);
}

void Monde::deplacerElement(Element *e, Position nouvellePos) {
	int id = this->getIndexDansListe(e);

	if(id < 0)
		throw new string("L'element plac� n'existe pas dans le monde.");

	this->getCarte().erase(e->getPosition());
	e->setPosition(nouvellePos);
	this->carte[nouvellePos] = id;
}

int Monde::getIndexDansListe(Element *e) {
	// On r�cup�re la position de l'�l�ment
	int id = -1;
	for(unsigned int i = 0 ; i < this->size() ; i++) {
		if(this->at(i) == e) {
			id=i;
			break;
		}
	}
	return id;
}

bool Monde::isCaseLibre(Position pos) {
		
	// On v�rifie que les coordonn�es sont valides
	if(pos.getX() < 0 || pos.getY() < 0 || pos.getX() > (Config::dimentions.first-1) || pos.getY() > (Config::dimentions.second-1) )
		return false;

	// Si la case est occup�e
	if(this->carte.count(pos) > 0) {
		return false;
	}

	OutputDebugString(L"Case libre ! \n");
	return true;
}

OCCUPANT Monde::getTypeOccupant(Position pos) {

	if(this->isCaseLibre(pos)) {
		return VIDE;
	}
	
	// Si la position est occup�e
	if(this->carte.count(pos) > 0) {

		// On r�cup�re l'�l�ment � l'emplacement
		Element* elem = (*this) [(this->carte[pos])];

		if(typeid(*elem) == typeid(Arbre))
			return ARBRE;
		if(typeid(*elem) == typeid(Sanglier))
			return SANGLIER;
		if(typeid(*elem) == typeid(Gaulois))
			return GAULOIS;
		if(typeid(*elem) == typeid(Gauloise))
			return GAULOISE;

	}

	return VIDE;
}

void Monde::jourSuivant()
{
	for(unsigned int i = 0; i < this->size(); i++)
	{
		try {
			if(this->at(i) == NULL)
				continue;
			if(typeid(*this->at(i)) == typeid(Gaulois))
				dynamic_cast<Gaulois *>(this->at(i))-> Gaulois::agir();
			if(typeid(*this->at(i)) == typeid(Gauloise))
				dynamic_cast<Gauloise *>(this->at(i))-> Gauloise::agir();
			if(typeid(*this->at(i)) == typeid(Sanglier)) {
				Sanglier *sanglier = dynamic_cast<Sanglier *>(this->at(i));
				sanglier->agir();
			}
		
		} catch(const std::logic_error & e) {
			OutputDebugStringA(e.what());
		}
	}
}

// STATIQUE
Monde* Monde::getInstance() {
	if(_instance == NULL) {
		_instance = new Monde();
	}
	return _instance;
}


void Monde::initialiserMonde() {

	// On ajoute les arbres
	for(int i = 0; i < Config::nb_arbres; i++) {
		Position pos = Position::random(Config::dimentions.first, Config::dimentions.second);
		this->ajouterElement(pos, new Arbre("A", pos));
	}

	// On ajoute les sangliers
	for(int i = 0; i < Config::nb_sanglier; i++) {
		Position pos = Position::random(Config::dimentions.first, Config::dimentions.second);
		Element *elem = dynamic_cast<Element *>(new Sanglier("S", pos));
		this->ajouterElement(pos, elem);
	}

	// On ajoute des gauloise
	for(int i = 0; i < Config::nb_gauloise; i++) {
		Position pos = Position::random(Config::dimentions.first, Config::dimentions.second);
		this->ajouterElement(pos, new Gauloise("", pos, 20, 5, 3, 2, 3));
	}

	// On ajoute des gaulois
	for(int i = 0; i < Config::nb_gaulois; i++) {
		Position pos = Position::random(Config::dimentions.first, Config::dimentions.second);
		this->ajouterElement(pos, new Gaulois("", pos, 20, 5, 3, 2, 3));
	}

	Village::nourriture = Config::nourriture;
	Village::bois = Config::bois;
}