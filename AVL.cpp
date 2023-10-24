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
        flux >> nombre; // Lire le premier entier
        this->r = new noeud(nombre);
        if (option){ // insertion en feuilles
            for (int i = 1 ; i < nbr_entiers; i++){
                if (flux >> nombre) { // Traitez chaque nombre en tant qu'entier
                    cout << "Nombre : " << nombre << endl;
                    this->insertionFeuille(this->root(), new noeud(nombre));
                    this->desequilibres(this->root());
                    this->prefixe(this->root());


                    
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


void AVL::desequilibres(noeud* x) { // met à jour l'attribut d de chaque noeud de l'arbre enraciné par x
    if (x == nullptr) {
        return;
    }
    
    if (x->fg != nullptr) desequilibres(x->fg);
    if (x->fd != nullptr) desequilibres(x->fd); 

    // Calcule le facteur d'équilibre pour le nœud actuel
    if (x->fg == nullptr && x->fd == nullptr) {
        x->d = 0;
    } else if (x->fg == nullptr) {
        x->d = -1 - x->fd->h;
        cout << "desequilibre de "<< x->cle << " : " << x->fd->cle << "/"<< x->fd->h << " - 1 = " << x->d << endl;
    } else if (x->fd == nullptr) {
        x->d = x->fg->h+1;
        cout << "desequilibre de "<< x->cle << " : " << x->fg->cle << "/"<< x->fg->h << " + 1 = " << x->d << endl;
    } else {
        x->d = x->fg->h - x->fd->h;
        cout << "desequilibre de "<< x->cle << " : " << x->fg->cle << "/"<<x->fg->h << " - "  << x->fd->cle << "/" << x->fd->h << " = " << x->d << endl;
    }
    equilibre(x);
}

void AVL::insertionFeuille(noeud* x, noeud* y)
{
    if (x == nullptr) return ;
    else if (y->cle < x->cle)
    {
        x->N++ ;
        x->h++ ;
        if (x->fg == nullptr)
        {
            cout << "insertion de " << y->cle << " en tant que fils gauche de " << x->cle << endl ;
            x->fg = y ;
            y->pere = x ;
        }   
        else {
            insertionFeuille(x->fg, y) ;
        }
    }
    else
    {
        x->N++ ;
        x->h++ ;
        if (x->fd == nullptr)
        {
            cout << "insertion de " << y->cle << " en tant que fils droit de " << x->cle << endl ;
            x->fd = y ;
            y->pere = x ;
        }
        else {
            insertionFeuille(x->fd, y) ;
        }
    }
    cout << "hauteur de "<< x->cle << " : " << x->h << endl;
}

void AVL::equilibre(noeud* x) {
    if (x == nullptr) {
        return;
    }
    int equilibre = x->d;
    // Si le nœud est déséquilibré à gauche, équilibrer le sous-arbre gauche
    if (equilibre == 2) {
        // Cas de la rotation simple à droite ou double à droite-gauche
        if (x->fg->d == -1) {
           rotationGauche(x->fg);
        }
        // Effectuer la rotation simple à droite
        return rotationDroite(x);
    }

    // Si le nœud est déséquilibré à droite, équilibrer le sous-arbre droit
    if (equilibre == -2) {
        // Cas de la rotation simple à gauche ou double à gauche-droite
        if (x->fd->d == 1) {
            rotationDroite(x->fd);
        }
        // Effectuer la rotation simple à gauche
        return rotationGauche(x);
    }

    // Si le nœud est équilibré, pas de modification nécessaire
    return;
}

void AVL::rotationDroite(noeud* r) {
    if (r == nullptr || r->fg == nullptr) {
        cout << "La rotation droite ne peut pas être effectuée" << endl;
        return;
    }

    cout << "---------Rotation droite de " << r->cle << endl;
    noeud* pere = r->pere;
    noeud* G = r->fg;

    // Mettre à jour les liens entre les nœuds
    G->pere = pere;
    r->fg = G->fd;

    if (G->fd != nullptr) G->fd->pere = r;
    G->fd = r;
    r->pere = G;

    // Mettre à jour le lien du père du nœud r
    if (pere == nullptr)
        this->r = G;
    else {
        if (pere->fg == r)
            pere->fg = G;
        else
            pere->fd = G;
    }

    // Mettre à jour la hauteur après la rotation
    majHauteurs(this->root());
}

void AVL::rotationGauche(noeud* r) {
        if (r == nullptr || r->fd == nullptr) {
        cout << "La rotation gauche ne peut pas être effectuée" << endl;
        return;
    }

    cout << "---------Rotation gauche de " << r->cle << endl;
    noeud* pere = r->pere;
    noeud* D = r->fd;

    // Mettre à jour les liens entre les nœuds
    D->pere = pere;
    r->fd = D->fg;

    if (D->fg != nullptr) D->fg->pere = r;

    D->fg = r;
    r->pere = D;

    // Mettre à jour le lien du père du nœud r
    if (pere == nullptr)
        this->r = D;
    else {
        if (pere->fd == r)
            pere->fd = D;
        else
            pere->fg = D;
    }

    // Mettre à jour la hauteur après la rotation
    majHauteurs(this->root());
}

void AVL::majHauteurs(noeud* x) {
    if (x == nullptr) {
        return;
    }

    // Appel récursif pour les sous-arbres gauche et droit
    majHauteurs(x->fg);
    majHauteurs(x->fd);

    // Calcul de la hauteur du noeud x
    int hg = (x->fg == nullptr) ? -1 : x->fg->h;
    int hd = (x->fd == nullptr) ? -1 : x->fd->h;
    x->h = max(hg, hd) + 1;
}


int AVL::noeuds(noeud *x)
{
    return 0;
}

void AVL::insertionRacine(noeud *x, noeud *y)
{
}

noeud* AVL::partition(noeud* x, int k) {
return nullptr;
}