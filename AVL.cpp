#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include "avl.h"
using namespace std ;

/******************** Fonctions noeud : ********************/

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

/******************** Fonctions AVL : ********************/

AVL::AVL(noeud* r)
{
    this->r = r ;
}

AVL::~AVL()
{
    if (r != nullptr) delete r ;
}

noeud* AVL::root()
{
    return r ; 
}

void AVL::prefixe(noeud *x) 
{
    if (x == nullptr) return ;
    else {
        cout << x->cle << " " ;
        prefixe(x->fg) ;
        prefixe(x->fd) ;
    }
}

AVL::AVL(const char* filename, bool option) // option = 0 : insertion en racine, option = 1 : insertion en feuille
{
    r = nullptr;

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
        getline(file, ligne); // Lire la deuxième ligne pour obtenir les entiers en question
        istringstream flux(ligne); // Convertir la ligne en flux d'entrée pour pouvoir lire les entiers
        int nombre;
        flux >> nombre; // Lire le premier entier
        this->r = new noeud(nombre);

        if (option){ // insertion en feuilles
            for (int i = 1 ; i <= nbr_entiers; i++){
                if (flux >> nombre) { 
                    this->insertionFeuille(this->root(), new noeud(nombre));
                    cout << endl << "affichage de l'arbre après insertion de " << nombre << " : " << endl;
                    this->affichageArbre(this->root(), "", 0); 
                }
            } 

        } else { // insertion en racine
            for (int i = 0; i < nbr_entiers; i++){
                if (flux >> nombre) {
                    this->insertionRacine(this->root(), new noeud(nombre)); 
                    cout << endl << "affichage de l'arbre après insertion de " << nombre << " : " << endl;
                    this->affichageArbre(this->root(), "", 0); 
                }
            } 
        }

        //une fois l'inserion terminée, on affiche l'arbre resultant et on l'équilibre
        this->miseajour(this->root());
        cout << endl << "affichage préfixe avant équilibrage : " << endl;
        this->prefixe(this->root()); 
        cout << endl << endl;
        this->equilibre(this->root());
        cout << "voulez vous un affichage graphique en plus de l'affichage préfixe de l'arbre ?" << endl << "1 = oui, 0 = non" << endl;
        this->miseajour(this->root());
        bool affichage;
        cin >> affichage;
        if (affichage) {
            cout << endl << "affichage graphique de l'arbre equilibré : " << endl;
            this->affichageArbre(this->root(), "", 0);
        }
        cout << endl << "affichage préfixe de l'arbre équilibré : " << endl;
        this->prefixe(this->root());
        cout << endl;
    }

    file.close();
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
            x->fd = y ;
            y->pere = x ;
        }
        else {
            insertionFeuille(x->fd, y) ;
        }
    }
}

void AVL::insertionRacine(noeud *x, noeud *y)
{
    if (x == nullptr) x = y;
    else if (x->cle > y->cle){
        x->pere = y;
        y->fd = x;
        this -> r = y;
    } else {
        x->pere = y;
        y->fg = x;
        this -> r = y;
    }
}

int AVL::hauteur(noeud *x)
{
    if (x == nullptr) {
        return 0;
    }
    
    hauteur(x->fg);
    hauteur(x->fd);

    int hg = (x->fg == nullptr) ? -1 : x->fg->h;
    int hd = (x->fd == nullptr) ? -1 : x->fd->h;
    x->h = max(hg, hd) + 1;
    return x->h;
}

void AVL::desequilibres(noeud *x)
{ 
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
    } else if (x->fd == nullptr) {
        x->d = x->fg->h+1;
    } else {
        x->d = x->fg->h - x->fd->h;
    }
}

void AVL::equilibre(noeud* x) {
    if (x != nullptr && x->N > 1){
        int m = (x->N)/2;
        if ((x->N) % 2 != 0) {
        m++; // Ajoute 0,5 si le nombre est impair
        }
        x = partition(x, m);
        miseajour(x); // Met à jour les attributs N, d et h de chaque noeud de l'arbre enraciné par x pour que la rotation fonctionne
        if (x->fg) {  // Si le fils gauche existe on peut equilbrer le coté gauche en premier, puis le coté droit
            equilibre(x->fg);
            equilibre(x->fd);
        } 
        if (x->fd) { // Si le fils droit existe on peut equilbrer le coté droit en premier, puis le coté gauche
            equilibre(x->fd);
            equilibre(x->fg);
        }

    } 
}

void AVL::rotationDroite(noeud* r) {
    if (r == nullptr || r->fg == nullptr) {
        cout << "La rotation droite ne peut pas être effectuée" << endl;
        return;
    }

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
}

void AVL::rotationGauche(noeud* r) {
        if (r == nullptr || r->fd == nullptr) {
        cout << "La rotation gauche ne peut pas être effectuée" << endl;
        return;
    }

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

    this->miseajour(this->root());
}

int AVL::noeuds(noeud *x)
{
    if (x == nullptr) return 0 ;
    else {
        x->N = 1 + noeuds(x->fg) + noeuds(x->fd) ;
        return x->N ;
    }
}

noeud* AVL::partition(noeud* x, int k) {
    noeud *res=nullptr;
    if (x != nullptr){
        int t = 0;
        if (x->fg){
            t = x->fg->N;
        }
        if (t == k-1){
            res = x;
        } else if (t > k-1){
            res = partition(x->fg, k);
            rotationDroite(x);
        } else {
            res = partition(x->fd, k-t-1);
            rotationGauche(x);
        }
    }
    return res;
}

/******************** Fonctions facultatives : ********************/

void AVL::miseajour(noeud *x)
{
    hauteur(x) ;
    noeuds(x) ;
    desequilibres(x) ;
}

void AVL::affichageArbre(noeud *x, const string &prefixe, bool estDernier)
{
    if (x == nullptr) {
        return;
    }

    cout << prefixe;
    cout << "|__";
    cout << x->cle << endl;

    string nouveauPrefixe = prefixe + (estDernier ? "    " : "|   ");
    if (x->fd) {
        this->affichageArbre(x->fd, nouveauPrefixe, x->fg == nullptr);
    }
    if (x->fg) {
        this->affichageArbre(x->fg, nouveauPrefixe, true);
    }

}