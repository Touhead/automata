#ifndef MODELE_H
#define MODELE_H

#include <QString>
#include <QStandardItemModel>
#include <QStringListModel>

class Automate;

class Modele
{

private:
    QStandardItemModel* modele;
    Automate* automate;
    QStringListModel *A, *Q, *I, *T;
    QString fichier;

public:

    Modele();
    virtual ~Modele();

    void setAutomate(Automate*);

    QStandardItemModel* getTableTranstion() const;
    QStringListModel* getA() const;
    QStringListModel* getQ() const;
    QStringListModel* getI() const;
    QStringListModel* getT() const;

    void editModele();

    int standardise();
    void standardiser();

    int determinise();
    void determiniser();

    bool complet();
    void completer();

    bool minimiser();

    int lecture(const QString&);

    bool ouvrir(const QString& = "");
    bool sauvegarder(const QString&);

    bool estOuvert();
};

#endif // MODELE_H
