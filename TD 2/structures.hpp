/**
* TD 3.
* \file   structures.hpp
* \author Irem et Albert
* \date   5 Mars 2023
* Créé le 25 Fevrier 2023
*/
#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
#include <functional>
using namespace std;
using gsl::span;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const std::string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const std::string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	Film& operator[](int index) const { return *elements[index]; }
	Film* chercherFilmSi(const function<bool(Film&)>& critere) {
		for (auto& film : enSpan()) 
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};
template <typename T>
class Liste {
public:
	Liste() = default;
	Liste(int cap, int nElem) {
		capacite_ = cap;
		nElements_ = nElem;
		elements_ = make_unique<shared_ptr<T>[]>(capacite_);
	}
	Liste(Liste<T>& autre) {
		capacite_ = autre.capacite_;
		nElements_ = autre.nElements_;
		elements_ = make_unique<shared_ptr<T>[]>(capacite_);
		for (int i = 0; i < autre.nElements_; i++)
			elements_[i] = autre.elements_[i];
	}
	void ajouterElement(shared_ptr<T> element) {
		elements_[nElements_] = element;
		nElements_ += 1;
	}
	Liste<T>& operator= (Liste<T>&& liste) = default;

	shared_ptr<T>& operator[] (int i) { 
		return elements_[i];
	}
	span<shared_ptr<T>> enSpan() const {
		return span(elements_.get(), nElements_);
	}

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};

using ListeActeurs = Liste<Acteur>;

struct Film
{
	std::string titre = "", realisateur = ""; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie = 0, recette = 0; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
	Film() = default;
	Film(Film& autre) {
		titre = autre.titre;
		realisateur = autre.realisateur;
		anneeSortie = autre.anneeSortie;
		recette = autre.recette;
		acteurs = Liste<Acteur>(autre.acteurs);
	}
};

struct Acteur
{
	std::string nom = ""; int anneeNaissance = 0; char sexe = ' ';
	//ListeFilms joueDans;
};
