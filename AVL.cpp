#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "avl.h"
using namespace std ;

noeud::noeud(int x)
{
    cle = x ;
    N = 1 ;
    d = 0 ;
    h = 0 ;
    fg = nullptr ;
    fd = nullptr ;
    pere = nullptr ;
}

noeud::~noeud()
{
    if (fg != nullptr) delete fg ;
    if (fd != nullptr) delete fd ;
}

void noeud::affiche()
{
    cout << "valeur : "<< cle << ", nombre de noeud de l'arbre enraciné par ce noeud : " << N << ", déséquilibre de ce noeud : " << d << ", hauteur de l'arbre enraciné par ce noeud : " << h << endl ;
}

AVL::AVL(noeud* r)
{
    this->r = r ;
}

AVL::AVL(const char* filename, bool option)
{
    r = nullptr; // Initialiser l'arbre AVL

    cout << "Lecture du fichier " << filename << endl;
    ifstream file;
    file.open(filename);
    
    if (!file)
    {
        cout << "Erreur : Impossible d'ouvrir le fichier " << filename << endl;
        return;
    }

    string ligne;
    if (file.is_open()){
        getline(file, ligne); // Lire la première ligne pour obtenir le nombre d'entiers
        int nbr_entiers = stoi(ligne);
        cout << "nombre d'entier(s) : " << nbr_entiers << endl;
        getline(file, ligne); // Lire la deuxième ligne pour obtenir les entiers
        istringstream flux(ligne);
        int nombre;
        if (option){ // insertion en feuilles
            for (int i = 0; i < nbr_entiers; i++){
                if (flux >> nombre) { // Traitez chaque nombre en tant qu'entier
                    cout << "Nombre : " << nombre << endl;
                    this->insertionFeuille(nombre);




                    
                } else {
                    cout << "Impossible de lire un nombre." << endl;
                    break;                
                }
            } 
        } else { // insertion en racine
            for (int i = 0; i < nbr_entiers; i++){
                if (flux >> nombre) { // Traitez chaque nombre en tant qu'entier
                    cout << "Nombre : " << nombre << endl;
                    this->insertionRacine(nombre);




                } else {
                    cout << "Impossible de lire un nombre." << endl;
                    break;                
                }
            } 
        }
    }

    file.close();
}

AVL::~AVL()
{
    if (r != nullptr) delete r ;
}

noeud* AVL::root()
{
    return r ;
}

void AVL::prefixe(noeud* x)
{
    if (x != nullptr)
    {
        x->affiche() ;
        prefixe(x->fg) ;
        prefixe(x->fd) ;
    }
}

int AVL::hauteur(noeud* x)
{
    if (x == nullptr) return -1 ;
    else return x->h ;
}

