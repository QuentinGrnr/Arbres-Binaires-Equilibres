#include <iostream>
#include "avl.h"

int main() {
    cout << "voulez vous effectuer une insertion en racine (0) ou une insertion en feuille (1) ?" << endl << "tappez 0 ou 1" << endl;
    bool choix_insertion;
    cin >> choix_insertion;
    AVL("donnees.txt", choix_insertion);

    return 0;
}
