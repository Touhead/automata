#include "modele.h"
#include "automate.h"

Modele::Modele()
{
    modele = new QStandardItemModel;
    A = new QStringListModel;
    Q = new QStringListModel;
    I = new QStringListModel;
    T = new QStringListModel;

    automate = 0;
}

Modele::~Modele(){

    delete automate;
    delete modele;
    delete A;
    delete Q;
    delete I;
    delete T;
}

void Modele::setAutomate(Automate* a){

    automate = a;
    automate->editA(A);
}

QStandardItemModel* Modele::getTableTranstion() const{

    return modele;
}

QStringListModel* Modele::getA() const{

    return A;
}
QStringListModel* Modele::getQ() const{

    return Q;
}
QStringListModel* Modele::getI() const{

    return I;
}
QStringListModel* Modele::getT() const{

    return T;
}

void Modele::editModele(){

    if(automate == 0)
        return;

    automate->editTableTranstion(modele);
    automate->editQ(Q);
    automate->editI(I);
    automate->editT(T);
}

int Modele::standardise(){

    if(automate == 0)
        return 255;

    return automate->standardise();
}

void Modele::standardiser(){

    if(automate == 0)
        return;

    automate->standardisation();
    editModele();
}

int Modele::determinise(){

    if(automate == 0)
        return 255;

    return automate->determinise();
}

void Modele::determiniser(){

    if(automate == 0)
        return;

    automate->determinisation();
    editModele();
}


bool Modele::complet(){

    if(automate == 0)
        return false;

    return automate->complet();
}

void Modele::completer(){

    if(automate == 0)
        return;

    automate->completer();
    editModele();
}

bool Modele::minimiser(){

    if(automate == 0)
        return false;

    bool estMinimise = automate->minimiser();
    editModele();
    return estMinimise;
}

int Modele::lecture(const QString& mot){

    if(automate == 0)
        return 255;

    return automate->lecture(mot.toStdString());
}

bool Modele::ouvrir(const QString& nomFichier){

    if (automate == 0)
        automate = new Automate();

    if (nomFichier.isEmpty()){

        if (fichier.isEmpty()){

            delete automate;
            automate = 0;
            return false;
        }

        if(automate->ouvrir(fichier.toStdString())){
            automate->editA(A);
            editModele();
            return true;
        }

        delete automate;
        automate = 0;
        return false;
    }

    if(automate->ouvrir(nomFichier.toStdString())){
        fichier = nomFichier;
        automate->editA(A);
        editModele();
        return true;
    }

    delete automate;
    automate = 0;
    return false;
}

bool Modele::sauvegarder(const QString& nomFichier){

    if(automate == 0)
        return false;

    return automate->sauvegarder(nomFichier.toStdString());
}

bool Modele::estOuvert(){

    return !(automate == 0);
}
