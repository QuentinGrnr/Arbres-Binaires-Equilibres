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
                    noeud* feuille = new noeud(nombre);
                    this->insertionFeuille(feuille, this->root());
                    this->equilibre(this->root());



                    
                } else {
                    cout << "Impossible de lire un nombre." << endl;
                    break;                
                }
            } 
        } else { // insertion en racine
            for (int i = 0; i < nbr_entiers; i++){
                if (flux >> nombre) { // Traitez chaque nombre en tant qu'entier
                    cout << "Nombre : " << nombre << endl;
                    noeud* racine = new noeud(nombre);
                    this->insertionRacine(racine, this->root());
                    this->equilibre(this->root());
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
     if (x == nullptr) {
        return;
    }

    // Afficher la valeur du nœud courant
    cout << x->cle << " ";

    // Parcourir le sous-arbre gauche en récursif
    prefixe(x->fg);

    // Parcourir le sous-arbre droit en récursif
    prefixe(x->fd);
}

int AVL::hauteur(noeud* x)
{
    if (x == nullptr) return -1 ;
    else return x->h ;
}

void desequilibres(noeud* x) {
    if (x == nullptr) {
        return;
    }

    // Calcule le facteur d'équilibre pour le nœud actuel
    x->d = hauteur(x->gauche) - hauteur(x->droite);

    // Appel récursif pour les sous-arbres gauche et droit
    desequilibres(x->gauche);
    desequilibres(x->droite);
}

void insertionFeuille(noeud* x, noeud* y)
{
    if (x == nullptr) return ;
    else if (y->cle < x->cle)
    {
        if (x->fg == nullptr)
        {
            x->fg = y ;
            y->pere = x ;
        }
        else insertionFeuille(x->fg, y) ;
    }
    else
    {
        if (x->fd == nullptr)
        {
            x->fd = y ;
            y->pere = x ;
        }
        else insertionFeuille(x->fd, y) ;
    }

    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));

}

Noeud* partition(Noeud* racine, int k) {
    if (racine == nullptr) {
        return nullptr;
    }

    int t = (racine->gauche != nullptr) ? racine->gauche->N : 0;

    if (k == t + 1) {
        // Trouvé la k-ième plus petite valeur à la racine
        return racine;
    } else if (k <= t) {
        // La k-ième plus petite valeur se trouve dans le sous-arbre gauche
        racine->gauche = partition(racine->gauche, k);
        // Effectuer une rotation droite pour rééquilibrer si nécessaire
        return rotationDroite(racine);
    } else {
        // La k-ième plus petite valeur se trouve dans le sous-arbre droit
        racine->droite = partition(racine->droite, k - t - 1);
        // Effectuer une rotation gauche pour rééquilibrer si nécessaire
        return rotationGauche(racine);
    }
}

Noeud* equilibre(Noeud* x) {
    if (x == nullptr) {
        return x;
    }

    desequilibres(x);
    int equilibre = x->d;

    // Si le nœud est déséquilibré à gauche, équilibrer le sous-arbre gauche
    if (equilibre > 1) {
        // Cas de la rotation simple à droite ou double à droite-gauche
        if (x->gauche->d < 0) {
            x->gauche = rotationGauche(x->gauche);
        }
        // Effectuer la rotation simple à droite
        return rotationDroite(x);
    }

    // Si le nœud est déséquilibré à droite, équilibrer le sous-arbre droit
    if (equilibre < -1) {
        // Cas de la rotation simple à gauche ou double à gauche-droite
        if (x->droite->d > 0) {
            x->droite = rotationDroite(x->droite);
        }
        // Effectuer la rotation simple à gauche
        return rotationGauche(x);
    }

    // Si le nœud est équilibré, pas de modification nécessaire
    return x;
}

Noeud* rotationDroite(Noeud* y) {
    Noeud* x = y->gauche;
    Noeud* T2 = x->droite;

    x->droite = y;
    y->gauche = T2;

    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));
    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));

    return x;
}

Noeud* rotationGauche(Noeud* x) {
    Noeud* y = x->droite;
    Noeud* T2 = y->gauche;

    y->gauche = x;
    x->droite = T2;

    x->hauteur = 1 + max(hauteur(x->gauche), hauteur(x->droite));
    y->hauteur = 1 + max(hauteur(y->gauche), hauteur(y->droite));

    return y;
}




