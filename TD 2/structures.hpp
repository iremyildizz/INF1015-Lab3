#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
using gsl::span;
using namespace std;

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

	template <typename Func>
	Film* chercherFilmlamda(Func critere) const
	{
		for (int i = 0; i < nElements; i++)
		{
			if (critere(*elements[i]))
			{
				return elements[i];
			}
		}
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};


template<typename T>
class Liste {

public:
	int capacite, nElements;
	unique_ptr<shared_ptr<T>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.

	Liste()
	{
		capacite = 0;
		nElements = 0;
		elements = unique_ptr<shared_ptr<T>[]>{ new shared_ptr<T>[nElements] };
	}

	Liste(int nombreElements)
	{
		nElements = nombreElements;
		capacite = nombreElements;
		elements = unique_ptr<shared_ptr<T>[]>{ new shared_ptr <T>[nombreElements] };
	}

	Liste<T>& operator=(const Liste<T>& other)
	{
		if (this != &other)
		{
			nElements = other.nElements;
			capacite = other.capacite;
			elements = unique_ptr<shared_ptr<T>[]>{ new shared_ptr<T>[capacite] };
			for (int i = 0; i < nElements; i++)
			{
				elements[i] = other.elements[i];
			}
		}
		return *this;
	}

	Liste(const Liste<T>& other)
	{
		nElements = other.nElements;
		capacite = other.capacite;
		elements = unique_ptr<shared_ptr<T>[]>{ new shared_ptr<T>[capacite] };
		for (int i = 0; i < nElements; i++)
		{
			elements[i] = other.elements[i];
		}
	}
};

using ListeActeurs = Liste<Acteur>;

struct Film
{
	std::string titre = " ", realisateur = " "; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie = 0, recette = 0; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
	friend ostream& operator<<(ostream& os, const Film& film);

	Film() = default;

	Film(const Film& other)
	{
		titre = other.titre;
		realisateur = other.realisateur;
		anneeSortie = other.anneeSortie;
		recette = other.recette;
		acteurs = other.acteurs;
	}
};

struct Acteur
{
	std::string nom = " "; int anneeNaissance = 0; char sexe = ' ';
	//ListeFilms joueDans;
};