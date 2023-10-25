#include <string>
#include <iostream>

using namespace std ;

class noeud
{
friend class AVL ;
    int cle ; // valeur du noeud
    int N ; // nombre de noeuds de l'arbre enraciné par ce noeud
    int d ; // déséquilibre de ce noeud
    int h ; // hauteur de l'arbre enraciné par ce noeud
    noeud * fg ; // fils gauche
    noeud * fd ; // fils droit
    noeud * pere ; // père
public :
    noeud(int x) ;
    ~noeud() ;
    void affiche() ;
} ;

class AVL
{
    noeud * r ;
public :
    AVL(noeud* r) ; // constructeur à partir d'un noeud
    AVL(const char* filename, bool option) ; // constructeur à partir d'un fichier
    ~AVL() ; // destructeur
    noeud* root() ; // retourne le pointeur sur la racine de l'arbre
    void prefixe(noeud *x); // affiche les noeuds de l'arbre enraciné par x selon un parcours préfixe
    int hauteur(noeud* x) ; // met a jour l'attribut h de chaque noeud de l'arbre enraciné par x

    void desequilibres(noeud* x) ; // met à jour l'attribut d de chaque noeud de l'arbre enraciné par x
    int noeuds(noeud* x) ; // met à jour l'attribut N de chaque noeud de l'arbre enraciné par x
    void insertionFeuille(noeud* x, noeud* y) ; // insère y en tant que feuille dans l'arbre enraciné par x
    void insertionRacine(noeud* x, noeud* y) ; // insère y en tant que racine dans l'arbre enraciné par x
    void rotationGauche(noeud* x) ; // effectue une rotation gauche de l'arbre enraciné par x
    void rotationDroite(noeud* x) ; // effectue une rotation droite de l'arbre enraciné par x
    noeud* partition(noeud* x, int k) ; // partitionne l’arbre par rapport a sa k-ieme plus petite valeur et renvoie l’adresse de celle-ci
    void equilibre(noeud* x) ; // équilibre l'arbre enraciné par x en utilisant la procédure "diviser pour régner"

    /*FONCTION FACULTATIVE AJOUTE AFIN DE RENDRE LE CODE PLUS CLAIR*/
    void miseajour(noeud* x) ; // met à jour les attributs N, d et h de chaque noeud de l'arbre enraciné par x
    void affichageArbre(noeud *x, const string &prefixe, bool estDernier); // affiche l'arbre enraciné par x graphiquement
    };
