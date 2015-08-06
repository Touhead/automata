#include "interface.h"

Interface::Interface(QWidget *parent) :
    QWidget(parent)
{
    this->resize(789, 519);

    gridLayout = new QGridLayout(this);
    layoutSecondaire = new QGridLayout();
    onglet = new QTabWidget(this);
    tabInfo = new QWidget();
    verticalLayoutWidget = new QWidget(tabInfo);
    verticalLayoutWidget->setGeometry(QRect(10, 10, 661, 421));
    layoutTabInfo = new QVBoxLayout(verticalLayoutWidget);

    labelA = new QLabel(verticalLayoutWidget);
    labelA->setText("Alphabet");

    layoutTabInfo->addWidget(labelA);

    listViewA = new QListView(verticalLayoutWidget);
    listViewA->setFlow(QListView::LeftToRight);

    layoutTabInfo->addWidget(listViewA);

    labelQ = new QLabel(verticalLayoutWidget);
    labelQ->setText("State(s)");

    layoutTabInfo->addWidget(labelQ);

    listViewQ = new QListView(verticalLayoutWidget);
    listViewQ->setFlow(QListView::LeftToRight);

    layoutTabInfo->addWidget(listViewQ);

    labelI = new QLabel(verticalLayoutWidget);
    labelI->setText("Initial State(s)");

    layoutTabInfo->addWidget(labelI);

    listViewI = new QListView(verticalLayoutWidget);
    listViewI->setFlow(QListView::LeftToRight);

    layoutTabInfo->addWidget(listViewI);

    labelT = new QLabel(verticalLayoutWidget);
    labelT->setText("Final State(s)");

    layoutTabInfo->addWidget(labelT);

    listViewT = new QListView(verticalLayoutWidget);
    listViewT->setFlow(QListView::LeftToRight);

    layoutTabInfo->addWidget(listViewT);

    onglet->addTab(tabInfo,"Info");

    tabTransition = new QWidget();
    vueTableTransition = new QTableView(tabTransition);
    vueTableTransition->setGeometry(QRect(10, 20, 661, 411));

    onglet->addTab(tabTransition,"State Transition Table");
    onglet->setCurrentIndex(0);

    layoutSecondaire->addWidget(onglet, 0, 0, 1, 1);

    gridLayout->addLayout(layoutSecondaire, 2, 0, 8, 4);

    boutonDeterminise = new QPushButton(this);
    boutonDeterminise->setText("Is a DFA ?");

    gridLayout->addWidget(boutonDeterminise, 6, 4, 1, 1);

    boutonMinimiser = new QPushButton(this);
    boutonMinimiser->setText("Minimize");

    gridLayout->addWidget(boutonMinimiser, 9, 4, 1, 1);

    boutonOuvrir = new QPushButton(this);
    boutonOuvrir->setText("Open");

    gridLayout->addWidget(boutonOuvrir, 1, 0, 1, 1);

    boutonLecture = new QPushButton(this);
    boutonLecture->setText("Reading");

    gridLayout->addWidget(boutonLecture, 1, 4, 1, 1);

    boutonDeterminiser = new QPushButton(this);
    boutonDeterminiser->setText("DFA");

    gridLayout->addWidget(boutonDeterminiser, 7, 4, 1, 1);

    saisieMots = new QLineEdit(this);

    gridLayout->addWidget(saisieMots, 1, 2, 1, 1);

    boutonEnregistrer = new QPushButton(this);
    boutonEnregistrer->setText("Save");

    gridLayout->addWidget(boutonEnregistrer, 1, 1, 1, 1);

    boutonCompleter = new QPushButton(this);
    boutonCompleter->setText("Completion");

    gridLayout->addWidget(boutonCompleter, 8, 4, 1, 1);

    boutonStandardiser = new QPushButton(this);
    boutonStandardiser->setText("Standardize");

    gridLayout->addWidget(boutonStandardiser, 5, 4, 1, 1);

    msg = new QMessageBox(this);
    msg->setDefaultButton(QMessageBox::Close);

    modele = new Modele();

    estAutomateBase = true;

    connect(boutonStandardiser,SIGNAL(clicked()),this,SLOT(standardise()));
    connect(boutonDeterminiser,SIGNAL(clicked()),this,SLOT(determiniser()));
    connect(boutonCompleter,SIGNAL(clicked()),this,SLOT(completer()));
    connect(boutonMinimiser,SIGNAL(clicked()),this,SLOT(minimiser()));
    connect(boutonDeterminise,SIGNAL(clicked()),this,SLOT(determinise()));
    connect(boutonLecture,SIGNAL(clicked()),this,SLOT(lecture()));
    connect(boutonOuvrir,SIGNAL(clicked()),this,SLOT(ouvrir()));
    connect(boutonEnregistrer,SIGNAL(clicked()),this,SLOT(sauvegarder()));
    connect(msg,SIGNAL(accepted()),msg,SLOT(close()));
}

Interface::~Interface()
{
    delete modele;
}

void Interface::setModele(){

    vueTableTransition->setModel(modele->getTableTranstion());
    vueTableTransition->setSortingEnabled(true);
    vueTableTransition->setEditTriggers(QAbstractItemView::NoEditTriggers);

    listViewA->setModel(modele->getA());
    listViewA->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listViewQ->setModel(modele->getQ());
    listViewQ->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listViewI->setModel(modele->getI());
    listViewI->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listViewT->setModel(modele->getT());
    listViewT->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Interface::messageMinimiser(QStringList* liste){

    QWidget* fenetre = new QWidget();
    QGridLayout* layout = new QGridLayout;
    QTableView* view = new QTableView;
    QStringListModel* model = new QStringListModel;

    model->setStringList(*liste);

    view->setModel(model);
    layout->addWidget(view);
    fenetre->setLayout(layout);

    fenetre->show();
}

void Interface::editModele(){

    if (modele == NULL)
        return;

    modele->editModele();
}

void Interface::standardise(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    if (!estAutomateBase){
        estAutomateBase = true;
        modele->ouvrir();
    }

    int retour = modele->standardise();

    if(retour == 0){

        msg->setIcon(QMessageBox::Critical);
        msg->setText("FSM was already standardized");
        msg->open();
        return;
    }

    QMessageBox* demande = new QMessageBox(this);

    demande->setIcon(QMessageBox::Information);
    demande->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    if(retour == 1)
        demande->setText("Standardization : This FSM has more than 1 initial state\nDo you want to standardize it ?");

    else if(retour == 2)
        demande->setText("Standardization :  This FSM has at least 1 transition to the initial state\nDo you want to standardize it ?");

    demande->open();

    connect(demande->button(QMessageBox::Yes),SIGNAL(clicked()),this,SLOT(standardiser()));
    connect(demande->button(QMessageBox::Yes),SIGNAL(clicked()),demande,SLOT(close()));
    connect(demande->button(QMessageBox::No),SIGNAL(clicked()),demande,SLOT(close()));
}

void Interface::standardiser(){

    if (modele == NULL)
        return;

    estAutomateBase = false;

    modele->standardiser();
    msg->setIcon(QMessageBox::Information);
    msg->setText("FSM standardized");
    msg->open();
}

void Interface::determinise(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    if (!estAutomateBase){
        estAutomateBase = true;
        modele->ouvrir();
    }

    int retour = modele->determinise();

    if(retour == 0){

        msg->setIcon(QMessageBox::Information);
        msg->setText("FSM is a DFA");
        msg->open();
        return;
    }

    msg->setIcon(QMessageBox::Information);

    if(retour == 1)
        msg->setText("NFA : this FSM has more than 1 initial state");
    else if(retour == 2)
        msg->setText("NFA : This FSM has any transition for at least 1 symbol in a state");

    msg->open();
}

void Interface::determiniser(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    if (!estAutomateBase){
        modele->ouvrir();
    }

    estAutomateBase = false;

    if(modele->determinise() == 0){

        msg->setIcon(QMessageBox::Critical);
        msg->setText("FSM is already a DFA");
        msg->open();
        return;
    }

    modele->determiniser();
    msg->setIcon(QMessageBox::Information);
    msg->setText("FSM has become a DFA");
    msg->open();
}

void Interface::completer(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    if(modele->determinise() != 0){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("FSM is not a DFA");
        msg->open();
        return;
    }

    if(modele->complet()){

        msg->setIcon(QMessageBox::Critical);
        msg->setText("FSM is already completed");
        msg->open();
        return;
    }

    modele->completer();
    msg->setIcon(QMessageBox::Information);
    msg->setText("Completion : A state has any transition for at least 1 symbol");
    msg->open();
}

void Interface::minimiser(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    if(!modele->complet()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("FSM is not completed");
        msg->open();
        return;
    }

    if(modele->determinise() != 0){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("FSM is not a DFA");
        msg->open();
        return;
    }

    if(modele->minimiser()){

        msg->setIcon(QMessageBox::Critical);
        msg->setText("FSM is already minimized");
        msg->open();
        return;
    }

    msg->setIcon(QMessageBox::Information);
    msg->setText("FSM minimized");
    msg->open();
}

void Interface::lecture(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    if(modele->determinise() != 0){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("FSM is not a DFA");
        msg->open();
        return;
    }

    QString mot = saisieMots->text();
    int retour = modele->lecture(mot);

    if (retour == 0){

        msg->setIcon(QMessageBox::Information);
        if (mot.isEmpty()){
            msg->setText("The word 'ø' is recognized");
        }
        else{
            msg->setText("The word '" + mot + "' is recognized");
        }
        msg->open();
        return;
    }

    QString text;

    msg->setIcon(QMessageBox::Critical);

    if (mot.isEmpty()){
        text = "The word 'ø' is not recognized";
    }
    else{
        text = "The word '" + mot + "' is not recognized";
    }

    if (retour == 1)
        text += " : Symbol doesn't belong to the alphabet";

    else if (retour == 2)
        text += " : The word didn't finish on a final state";

    else if (retour < 0)
        text += " : Any transition for symbol " + QString::number(-retour);

    msg->setText(text);
    msg->open();
}

void Interface::ouvrir(){

    if (modele == NULL)
        return;

    QString nomFichier = QFileDialog::getOpenFileName();

    if ((!nomFichier.isEmpty())&&(modele->ouvrir(nomFichier))){

        setModele();

        msg->setIcon(QMessageBox::Information);
        msg->setText("Loaded");
        msg->open();
        return;
    }

    msg->setIcon(QMessageBox::Critical);
    msg->setText("Failure");
    msg->open();
}

void Interface::sauvegarder(){

    if (modele == NULL)
        return;

    if (!modele->estOuvert()){

        msg->setIcon(QMessageBox::Warning);
        msg->setText("Any FSM is loaded");
        msg->open();
        return;
    }

    QString nomFichier = QFileDialog::getSaveFileName();

    if (modele->sauvegarder(nomFichier)){

        msg->setIcon(QMessageBox::Information);
        msg->setText("Saved");
        msg->open();
        return;
    }

    msg->setIcon(QMessageBox::Critical);
    msg->setText("Failure");
    msg->open();
}
