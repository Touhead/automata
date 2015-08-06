#ifndef AUTOMATE_H_INCLUDED
#define AUTOMATE_H_INCLUDED

#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QStandardItem>
#include <QStringList>
#include <QString>
#include <fstream>

#include "interface.h"

using namespace std;

class Automate{

private:

    // Les etats de l'automate de base ne doivent pas etre une combinaison d'autre etat. Ex: {"0"; "1"; "01"} OK / {"0"; "1"; {"0";"1"}} NON
    // L'automate doit disposer d'au moins 1 etat, 1 entree et 1 sortie
    list<string> _A; // Alphabet
    list<list<string>*> _Q, _I, _T; // Liste des etats, liste des etats initiaux, liste des etats terminaux (chaque etat pouvant etre composé de plusieurs d'etat. Ex : determinisation)
                                    // Un etat est donc une liste de string. Ex : {"0";"1"} avec "0" et "1" deux etats presents dans l'automate de base
    map<list<string>*,list<list<string>*>*> _tab;  // 'Tableau(=map)' de tableau d'etat de taille (nb de caractere dans l'alphabet) indexé par des pointeurs sur des etats

    void deleteA();
    void deleteQ();
    void deleteI();
    void deleteT();
    void deleteTableTranstion();

public:

    Automate();
    virtual ~Automate();

    int standardise();
    void standardisation();

    int determinise();
    void determinisation();

    bool complet();
    void completer();

    bool minimiser();   // Retourne True si l'automate etait deja minimise, False sinon

    int lecture(const string&);

    bool ouvrir(const string&);
    bool sauvegarder(const string&);

    void editTableTranstion(QStandardItemModel*);
    void editA(QStringListModel*);
    void editQ(QStringListModel*);
    void editI(QStringListModel*);
    void editT(QStringListModel*);
};


#endif // AUTOMATE_H_INCLUDED
