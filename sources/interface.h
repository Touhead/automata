#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include "modele.h"
#include <QGridLayout>
#include <QListView>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QLineEdit>

class Interface : public QWidget
{
    Q_OBJECT

public:
    Interface(QWidget *parent = 0);
    virtual ~Interface();
    void setModele();
    static void messageMinimiser(QStringList* liste);

public slots:
    void editModele();
    void standardise();
    void standardiser();
    void determinise();
    void determiniser();
    void completer();
    void minimiser();
    void lecture();
    void ouvrir();
    void sauvegarder();

private:

    QGridLayout* gridLayout;
    QGridLayout* layoutSecondaire;
    QTabWidget* onglet;
    QWidget* tabInfo;
    QWidget* verticalLayoutWidget;
    QVBoxLayout* layoutTabInfo;
    QLabel* labelA;
    QListView* listViewA;
    QLabel* labelQ;
    QListView* listViewQ;
    QLabel* labelI;
    QListView* listViewI;
    QLabel* labelT;
    QListView* listViewT;
    QWidget* tabTransition;
    QTableView* vueTableTransition;
    QPushButton* boutonDeterminise;
    QPushButton* boutonMinimiser;
    QSpacerItem* horizontalSpacer;
    QPushButton* boutonOuvrir;
    QPushButton* boutonLecture;
    QPushButton* boutonDeterminiser;
    QLineEdit* saisieMots;
    QPushButton* boutonEnregistrer;
    QPushButton* boutonCompleter;
    QPushButton* boutonStandardiser;
    QSpacerItem* verticalSpacer;
    QMessageBox* msg;

    Modele* modele;

    bool estAutomateBase;
};

#endif // INTERFACE_H
