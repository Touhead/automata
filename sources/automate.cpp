#include <iostream>
#include "automate.h"

Automate::Automate(){

}

Automate::~Automate(){

    deleteTableTranstion();
    deleteQ();
    deleteI();
    deleteT();
}

void Automate::deleteA(){

    _A.clear();
}

void Automate::deleteQ(){

    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++)
        delete *iQ;

    _Q.clear();
}

void Automate::deleteI(){

    for(list<list<string>*>::iterator iI=_I.begin(); iI!=_I.end(); iI++)
        delete *iI;

    _I.clear();
}

void Automate::deleteT(){

    for(list<list<string>*>::iterator iT=_T.begin(); iT!=_T.end(); iT++)
        delete *iT;

    _T.clear();
}

void Automate::deleteTableTranstion(){

    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

        for(unsigned int cpt=0; cpt<_A.size(); cpt++){

            for(list<list<string>*>::iterator iTab=_tab[*iQ][cpt].begin(); iTab!=_tab[*iQ][cpt].end(); iTab++)
                delete *iTab;

            _tab[*iQ][cpt].clear();
        }

        delete[] _tab[*iQ];
    }
}

int Automate::standardise(){

    /* Retourne 0 si l'automate est standardise
     * Retourne 1 s'il y a plus d'un etat initial
     * Retourne 2 si une transition transition mene vers l'etat initial
     * */

    // S'il y a plus d'un etat initial
    if (_I.size() > 1)
        return 1;

    // Si une transition va vers une des entrees
    list<list<string>*>::iterator iI=_I.begin();

    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

        for(unsigned int cpt=0; cpt<_A.size(); cpt++){
			for(list<list<string>*>::iterator iTab=_tab[*iQ][cpt].begin(); iTab!=_tab[*iQ][cpt].end(); iTab++){
				if(((*iTab)->size() != 0)&&(((*iTab)->size() == (*iI)->size())&&(equal((*iTab)->begin(), (*iTab)->end(), (*iI)->begin()))))
                    return 2;
			}
        }
    }
    return 0;
}

void Automate::standardisation(){

    /* Standardise l'automate en ajoutant l'etat 'i'
     * */

    list<list<string>*>::iterator iQ;

    // Insere l'etat i dans la liste des etats
    _Q.push_front(new list<string>);
    iQ = _Q.begin();
    (*iQ)->push_back("i");
    _tab[*iQ] = new list<list<string>*>[_A.size()];

    // Insere les transitions de l'etat 'i' (regroupant les transitions des etats initiaux)
    for(iQ=++_Q.begin(); iQ!=_Q.end(); iQ++){

        for(list<list<string>*>::iterator iI=_I.begin(); iI!=_I.end(); iI++){

            if(((*iQ)->size() == (*iI)->size())&&(equal((*iQ)->begin(), ((*iQ)->end()), (*iI)->begin()))){

                list<list<string>*>::iterator iiTab;

                for(unsigned int cpt=0; cpt<_A.size(); cpt++){
					for(list<list<string>*>::iterator iTab=_tab[*iQ][cpt].begin(); iTab!=_tab[*iQ][cpt].end(); iTab++){
						_tab[*_Q.begin()][cpt].push_back(new list<string>);
                        iiTab =-- _tab[*_Q.begin()][cpt].end();
                        (*iiTab)->insert((*iiTab)->begin(),(*iTab)->begin(),(*iTab)->end());
					}
                }
            }
        }
    }

    // Tri les transitions de l'etat 'i'
    iQ = _Q.begin();
    for(unsigned int cpt=0; cpt<_A.size(); cpt++){
		for(list<list<string>*>::iterator iTab=_tab[*iQ][cpt].begin(); iTab!=_tab[*iQ][cpt].end(); iTab++){

			(*iTab)->sort();
			(*iTab)->unique();
		}
    }
\
    // Si un des etats initiaux est terminal
    int temp = 0;
    list<list<string>*>::iterator iI;
    list<list<string>*>::iterator iT;

    iI=_I.begin();
    while ((iI != _I.end())&&(temp == 0)){

        iT=_T.begin();
        while ((iT != _T.end())&&(temp == 0)){

            if (((*iI)->size() == (*iT)->size())&&(equal((*iI)->begin(),(*iI)->end(),(*iT)->begin())))
                temp = 1;
            iT++;
        }
        iI++;
    }

    // Insere 'i' dans les etats terminaux
    if (temp == 1){

        _T.push_front(new list<string>);
        (*_T.begin())->push_back("i");
    }

    // Remplace les etats initiaux par 'i'
    deleteI();
    _I.push_front(new list<string>);
    (*_I.begin())->push_back("i");
}

int Automate::determinise(){

    /* Retourne 1 si l'automate n'est pas deterministe <=> L'automate a plus d'un etat initial
     * Retourne 2 si l'automate n'est pas deterministe <=> Au moins un etat peut mener vers 2 etats en lisant le meme caractere
     * Retourne 0 si l'automate est deterministe
     * */

    // S'il y a plus de 2 entrees
    if (_I.size() > 1)
        return 1;

    list<list<string>*>::iterator iiQ;

    // Si une transition est composés de 2 etats (c.a.d une transition n'est egale a aucun des etats de la liste des etats)
    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

        for(unsigned int cpt=0; cpt<_A.size(); cpt++){

            if(_tab[*iQ][cpt].size()  > 1)
                return 2;
        }
    }

    return 0;
}

void Automate::determinisation(){

    /* Determinise l'automate
     * */

    list<list<string>*> Qdet;
    list<list<string>*> Tdet;

    // Creation de l'etat initial (= premier etat de la liste des etats)
    Qdet.push_back(new list<string>);
    list<list<string>*>::iterator iQdet = Qdet.begin();
    _tab[*iQdet] = new list<list<string>*>[_A.size()];

    for(list<list<string>*>::iterator iI=_I.begin(); iI!=_I.end(); iI++)
        (*iQdet)->insert((*iQdet)->begin(),(*iI)->begin(),(*iI)->end());


    (*iQdet)->sort();
    (*iQdet)->unique();

    // Remplace les etats initiaux pour le nouvel etat initial
    _I.clear();
    _I.push_back(new list<string>);
    (*_I.begin())->assign((*iQdet)->begin(),(*iQdet)->end());

    // Creer la nouvelle table de transition et la nouvelle liste des etats
    list<list<string>*>::iterator iQ;
    list<string>::iterator iiQdet;
    unsigned int cpt;

    // Tant qu'il y a des etats dans la nouvelle liste des etats
    while(iQdet != Qdet.end()){

        for(cpt=0; cpt<_A.size(); cpt++){
            _tab[*iQdet][cpt].push_back(new list<string>);
        }

        // Creation des nouvelles transitions pour l'etat concerne
        for(iQ=_Q.begin(); iQ!=_Q.end(); iQ++){
            for(iiQdet=(*iQdet)->begin(); iiQdet!=(*iQdet)->end(); iiQdet++){

                if (find((*iQ)->begin(),(*iQ)->end(),*iiQdet) != (*iQ)->end()){

                    for(cpt=0; cpt<_A.size(); cpt++){

                        list<list<string>*>::iterator iiTab;

                        iiTab = _tab[*iQdet][cpt].begin();
                        for(list<list<string>*>::iterator iTab=_tab[*iQ][cpt].begin(); iTab!=_tab[*iQ][cpt].end(); iTab++)
                            (*iiTab)->insert(--(*iiTab)->end(),(*iTab)->begin(),(*iTab)->end());
                    }
                }
            }
        }

        int temp;
        list<list<string>*>::iterator iTab;

        // Tri des nouvelles transitions pour l'etat concerne, et creation d'un nouvel etat a partir des nouvelles transition (s'il n'existait pas deja)
        for(cpt=0; cpt<_A.size(); cpt++){

            iTab=_tab[*iQdet][cpt].begin();
            while((!_tab[*iQdet][cpt].empty())&&(iTab!=_tab[*iQdet][cpt].end())){

                if((*iTab)->empty()){
                    _tab[*iQdet][cpt].erase(iTab);
                    iTab--;
                }
                else{
                    (*iTab)->sort();
                    (*iTab)->unique();
                }

                iTab++;
            }

            temp = 0;
            iTab = _tab[*iQdet][cpt].begin();

            // Creation d'un nouvel etat (s'il n'existait pas deja)
            for(iQ=Qdet.begin(); iQ!=Qdet.end(); iQ++){

                if((_tab[*iQdet][cpt].empty())||(((*iTab)->size() == (*iQ)->size())&&(equal((*iTab)->begin(),(*iTab)->end(),(*iQ)->begin()))))
                    temp = 1;
            }
            if (temp == 0){

                Qdet.push_back(new list<string>);
                _tab[*(--Qdet.end())] = new list<list<string>*>[_A.size()];
                (*(--Qdet.end()))->assign((*iTab)->begin(),(*iTab)->end());
            }
        }

        iQdet++;
    }

    // Suppression de l'ancienne table de transition
    deleteTableTranstion();

    // Remplace l'ancienne liste des etats par la nouvelle
    deleteQ();
    _Q = Qdet;

    // Creation de la nouvelle liste des etats teminaux (tout etat contenant un ancien etat terminal est terminal)
    for(list<list<string>*>::iterator iT=_T.begin(); iT!=_T.end(); iT++){
        for(iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

            if(search((*iQ)->begin(),(*iQ)->end(),(*iT)->begin(),(*iT)->end()) != (*iQ)->end()){

                int temp = 0;
                list<list<string>*>::iterator iTdet;

                iTdet = Tdet.begin();
                while ((iTdet != Tdet.end())&&(temp == 0)){

                    if (((*iQ)->size() == (*iTdet)->size())&&(equal((*iQ)->begin(),(*iQ)->end(),(*iTdet)->begin())))
                        temp = 1;
                    iTdet++;
                }

                if (temp == 0){
                    Tdet.push_back(new list<string>);
                    (*(--Tdet.end()))->assign((*iQ)->begin(),(*iQ)->end());
                }
            }
        }
    }

    // Remplace l'ancienne liste des etats terminaux par la nouvelle
    deleteT();
    _T = Tdet;

}

bool Automate::complet(){

    /* Retourne false si l'automate n'est pas complet <=> Au moins un etat n'a pas de transition pour un caractere de l'alphabet
     * Retourne true si l'automate est complet
     * */

    //On parcours l'automate et ses transitions

    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

        for(unsigned int cpt=0; cpt<_A.size(); cpt++){

            // Si une des transition ne mène vers rien

            if(_tab[*iQ][cpt].empty()){

                return false;
            }
        }
    }

    return true;
}

void Automate::completer(){

    /* Complete l'automate en ajoutant l'etat 'P'
     * */

    list<list<string>*>::iterator iQ;

    // Insere l'etat P dans la liste des etats

    _Q.push_back(new list<string>);
    iQ = --_Q.end();
    (*iQ)->push_back("P");
    _tab[*iQ] = new list<list<string>*>[_A.size()];


    //Parcours de l'automate et ses transitions

    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

        for(unsigned int cpt=0; cpt<_A.size(); cpt++){

            // S'il n'existe aucune transition pour un caractere de l'alphabet

            if(_tab[*iQ][cpt].empty()){

                _tab[*iQ][cpt].push_back(new list<string>);
                (*_tab[*iQ][cpt].begin())->push_back("P");
            }
        }
    }
}

bool Automate::minimiser(){

    /* Minimise l'automate
     * Retourne true si l'automate etait deja minimal <=> Le nombre de groupe cree lors de la minimisation est egal au nombre d'etat de l'automate
     * Retourne false si l'automate n'etait pas minimal
     * */

    list<string> groupes; // La liste des groupes Ex : {NT, T}
    list<string> tempgroupes; // Une liste des groupes temporaires
    string  groupeString; // Le groupe auquel appartient un etat
    map<string, list<list<string>*>*> tabG; // Un tableau indexe par les groupes contenant la liste des etats dans un groupe

    //Initialisation de tempGroupes et de tabG
    tempgroupes.push_back("NT");
    tempgroupes.push_back("T");

    tabG["NT"] = new list<list<string>*>;
    tabG["T"] = new list<list<string>*>;

    for(list<list<string>*>::iterator iQ=_T.begin(); iQ!=_T.end(); iQ++)
        tabG["T"]->push_back(*iQ);

    for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

        int temp = 0;

        for(list<list<string>*>::iterator iT=_T.begin(); iT!=_T.end(); iT++){

             if( ((*iT)->size() == (*iQ)->size()) && (equal(  (*iQ)->begin(), (*iQ)->end(), (*iT)->begin())  )){
                 temp = 1;
             }
        }

        if(temp == 0)
           tabG["NT"]->push_back(*iQ);
    }

    list<string>::iterator iG;
    QStringList liste;
    QString tempS;
    int cptG;

    // Tant que le nombre de groupe n'est pas egal a celui de la derniere iteration
    while (groupes.size() != tempgroupes.size()) {

        // Initialisation de groupes et reinitialisation de tempgroupes
        groupes = tempgroupes;
        tempgroupes.clear();

        cptG = 1;

        for(list<string>::iterator i = groupes.begin(); i != groupes.end(); i++){

            tempS.clear();

            tempS += "Group " + QString::number(cptG) + " : ";
            for(list<list<string>*>::iterator ii = tabG[*i]->begin(); ii != tabG[*i]->end(); ii++){

                if (ii != tabG[*i]->begin())
                    tempS += ", ";

                tempS += "{";

                for(list<string>::iterator iii = (*ii)->begin(); iii != (*ii)->end(); iii++){

                    if(iii != (*ii)->begin())
                        tempS += ", ";

                    tempS += QString::fromStdString(*iii);
                }

                tempS += "}";
            }

            liste << tempS;
            cptG++;
        }

        liste << "";


        // Pour chaque etat
        for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

            // Reinitialisation de groupeString
            groupeString.clear();

            iG = groupes.begin();

            int temp;

            // Faire tant que nous ne trouvons pas dans quel groupe est cet etat
            temp = 1;
            do {

                for(list<list<string>*>::iterator iiG = tabG[*iG]->begin(); iiG != tabG[*iG]->end(); iiG++){

                    // Si les etats sont egaux
                    if (((*iiG)->size() == (*iQ)->size())&&(equal((*iiG)->begin(), (*iiG)->end(), (*iQ)->begin())))
                        temp = 0;
                }

                iG++;

            }while(temp == 1);

            iG--;

            // Insertion de ce groupe dans groupeString
            groupeString = groupeString + *iG;

            for (unsigned int cpt = 0; cpt != _A.size(); cpt++){

                int temp;
                iG = groupes.begin();

                temp = 1;
                 do{

                     list<list<string>*>::iterator iTab = _tab[*iQ][cpt].begin();

                     for(list<list<string>*>::iterator iiG = tabG[*iG]->begin(); iiG != tabG[*iG]->end(); iiG++){

                         if (((*iiG)->size() == (*iTab)->size())&&(equal((*iiG)->begin(), (*iiG)->end(), (*iTab)->begin()))) // Si les etats ne sont pas egaux
                             temp = 0;
                     }

                     iG++;

                 }while(temp == 1);

                 iG--;

                 // Insertion de ce groupe dans groupeString
                 groupeString = groupeString + *iG;
             }

             if(tabG[groupeString] == NULL){
                 tabG[groupeString] = new list<list<string>*>;
             }

             // Insertion de cet etat dans son groupe
             tabG[groupeString]->push_back(*iQ);

             tempgroupes.push_back(groupeString);

             tempgroupes.sort();
             tempgroupes.unique();
        }

        // Suppression des anciens groupes
        for (list<string>::iterator it=groupes.begin(); it != groupes.end(); it++)
            tabG[*it]->clear();
    }

    Interface::messageMinimiser(&liste);

    groupes = tempgroupes;
    tempgroupes.clear();

    if(groupes.size() == _Q.size())
        return true;

    list<list<string>*> Qmin, Tmin, Imin;

    list<string>::iterator iGG;
    list<list<string>*>::iterator iTabG;
    list<list<string>*>::iterator iTab;

    int temp;

    list<list<string>*>::iterator iI;
    list<list<string>*>::iterator iIm;

    iI = _I.begin();
    iG=groupes.begin();
    temp = 1;
    do {

        for(list<list<string>*>::iterator iiG = tabG[*iG]->begin(); iiG != tabG[*iG]->end(); iiG++){

            if (((*iiG)->size() == (*iI)->size())&&(equal((*iiG)->begin(), (*iiG)->end(), (*iI)->begin()))) // Si les etats ne sont pas egaux
                temp = 0;
        }

        iG++;

    }while(temp == 1);

    iG--;

    Imin.push_back(new list<string>);
    iIm =-- Imin.end();

    for(list<list<string>*>::iterator iTabGG = tabG[*iG]->begin(); iTabGG != tabG[*iG]->end(); iTabGG++)
        (*iIm)->insert(--(*iIm)->end(), (*iTabGG)->begin(), (*iTabGG)->end());

    deleteI();
    _I = Imin;

    list<list<string>*>::iterator iTmin;

    for(list<string>::iterator iG = groupes.begin(); iG != groupes.end(); iG++){

        if ((*iG).at(0) == 'T'){

            Tmin.push_back(new list<string>);
            iTmin =-- Tmin.end();

            for(list<list<string>*>::iterator iTabG = tabG[*iG]->begin(); iTabG != tabG[*iG]->end(); iTabG++){

                (*iTmin)->insert(--(*iTmin)->end(), (*iTabG)->begin(), (*iTabG)->end());
            }
        }
    }

    deleteT();
    _T = Tmin;

    list<list<string>*>::iterator iQmin;
    list<list<string>*>::iterator iTabmin;

    for(list<string>::iterator iG=groupes.begin(); iG != groupes.end(); iG++){

        // Creation des états
        Qmin.push_back(new list<string>);
        iQmin = --Qmin.end();

        for(iTabG = tabG[*iG]->begin(); iTabG != tabG[*iG]->end(); iTabG++){

            (*iQmin)->insert(--(*iQmin)->end(), (*iTabG)->begin(), (*iTabG)->end());
        }

        // Creation des transitions associes a chaque etat
        _tab[*(--Qmin.end())] = new list<list<string>*>[_A.size()];

        // Pour chaque transition
        for(unsigned int cpt = 0; cpt < _A.size(); cpt++){

            // Pour un etat dans le groupe
            iTabG = tabG[*iG]->begin();

            // Pour une transition de de cet etat dans le groupe
            iTab = _tab[*iTabG][cpt].begin();

            int temp;
            iGG = groupes.begin();

            temp = 1;

            // Faire tant que nous ne trouvons pas dans quel groupe est cet etat
            do {

                for(list<list<string>*>::iterator iiG = tabG[*iGG]->begin(); iiG != tabG[*iGG]->end(); iiG++){

                    if (((*iiG)->size() == (*iTab)->size())&&(equal((*iiG)->begin(), (*iiG)->end(), (*iTab)->begin())))
                        temp = 0;
                    }

                    iGG++;

            }while(temp == 1);

            iGG--;

            _tab[*(--Qmin.end())][cpt].push_back(new list<string>);

            iTabmin =-- _tab[*(--Qmin.end())][cpt].end();

            for(list<list<string>*>::iterator iTabGG = tabG[*iGG]->begin(); iTabGG != tabG[*iGG]->end(); iTabGG++)
                (*iTabmin)->insert(--(*iTabmin)->end(), (*iTabGG)->begin(), (*iTabGG)->end());
        }
    }

    deleteQ();
    _Q = Qmin;

    return false;
}

int Automate::lecture(const string& mot){

    /* Retourne 1 si le mot n'est pas reconnu par l'automate <=> Le caractere lu n'appartient pas a l'alphabet
     * Retourne 2 si le mot n'est pas reconnu par l'automate <=> L'etat sur lequel nous arrivons en lisant le mot n'est pas terminal
     * Retourne r < 0 si le mot n'est pas reconnu par l'automate <=> Il n'existe pas de transition dans cet etat pour le caractere
     * Retourne 0 si le mot est reconnu par l'automate
     * */

    list<string>* etatCourant = NULL;

    list<list<string>*>::iterator iI;
    list<list<string>*>::iterator iQ;

    iI=_I.begin();
    iQ=_Q.begin();

    while ((iQ != _Q.end())&&(etatCourant == NULL)){

        if (((*iQ)->size() == (*iI)->size())&&(equal((*iQ)->begin(),(*iQ)->end(),(*iI)->begin())))
            etatCourant = *iQ;
        iQ++;
    }

    int cpt;
    list<string>::iterator iA;
    list<string>* etatCourantTemp;

    cpt = 0;
    while (mot[cpt] != 0){

        iA = find(_A.begin(),_A.end(),mot.substr(cpt,1));

        if (iA == _A.end())
            return 1;

        if (_tab[etatCourant][distance(_A.begin(),iA)].empty())
            return (-cpt)-1;
        etatCourant = *_tab[etatCourant][distance(_A.begin(),iA)].begin();

        iQ=_Q.begin();
        etatCourantTemp = etatCourant;

        while ((iQ != _Q.end())&&(etatCourant == etatCourantTemp)){

            if (((*iQ)->size() == etatCourant->size())&&(equal((*iQ)->begin(),(*iQ)->end(),etatCourant->begin())))
                etatCourant = *iQ;
            iQ++;
        }

        cpt++;
    }

    int temp;
    list<list<string>*>::iterator iT;

    temp = 0;
    iT=_T.begin();
    while ((iT != _T.end())&&(temp == 0)){

        if (((*iT)->size() == etatCourant->size())&&(equal((*iT)->begin(),(*iT)->end(),etatCourant->begin())))
            temp = 1;
        iT++;
    }

    if (temp == 0)
        return 2;

    return 0;
}

bool Automate::ouvrir(const string& nomFichier){

    deleteTableTranstion();
    deleteA();
    deleteQ();
    deleteI();
    deleteT();

    ifstream fichier(nomFichier.c_str(), ios::in);
    if (fichier.is_open()){

        string charA;
        string charQ;
        string charI;
        string charT;
        string charTDT;
        string temp;

        getline(fichier,charA);

        for(unsigned int i = 0; i < charA.size(); i++)
        {
            temp.clear();
            while ((i < charA.size())&&(charA[i] == ' '))
                i++;
            while((i < charA.size())&&(charA[i] != ' ')){
                temp += charA.substr(i,1);
                i++;
            }
            _A.push_back(temp);
        }

        getline(fichier,charQ);

        for(unsigned int i = 0; i < charQ.size(); i++)
        {
            temp.clear();
            while ((i < charQ.size())&&(charQ[i] == ' '))
                i++;
            while((i < charQ.size())&&(charQ[i] != ' ')){
                temp += charQ.substr(i,1);
                i++;
            }
            _Q.push_back(new list<string>);
            (*--_Q.end())->push_back(temp);
        }

        getline(fichier,charI);

        for(unsigned int i = 0; i < charI.size(); i++)
        {
            temp.clear();
            while ((i < charI.size())&&(charI[i] == ' '))
                i++;
            while((i < charI.size())&&(charI[i] != ' ')){
                temp += charI.substr(i,1);
                i++;
            }
            _I.push_back(new list<string>);
            (*--_I.end())->push_back(temp);
        }

        getline(fichier,charT);

        for(unsigned int i = 0; i < charT.size(); i++)
        {
            temp.clear();
            while ((i < charT.size())&&(charT[i] == ' '))
                i++;
            while((i < charT.size())&&(charT[i] != ' ')){
                temp += charT.substr(i,1);
                i++;
            }
            _T.push_back(new list<string>);
            (*--_T.end())->push_back(temp);
        }

        list<list<string>*>::iterator iQ;

        for(iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

            _tab[*iQ]=new list<list<string>*>[_A.size()];
        }

        getline(fichier,charTDT);
        iQ = _Q.begin();

        unsigned int col;
        unsigned int start;
        while ((!fichier.eof())&&(iQ != _Q.end())){

            getline(fichier,charTDT);
            col = 0;
            start = 0;
            while ((start < charTDT.size())&&(charTDT[start] == ' '))
                start++;
            while(charTDT[start] != ' ')
                start++;

            for(unsigned int i=start; i < charTDT.size(); i++){

                temp.clear();
                while ((i < charTDT.size())&&(charTDT[i] == ' '))
                    i++;
                while((i < charTDT.size())&&(charTDT[i] != ',')&&(charTDT[i] != ' ')){
                    temp += charTDT.substr(i,1);
                    i++;
                }

                if (temp != "-"){
                    _tab[*iQ][col].push_back(new list<string>);
                    (*--_tab[*iQ][col].end())->push_back(temp);
                }

                if (charTDT[i] == ','){
                    i++;
                }
                else{
                    col++;
                }
            }
            iQ++;
        }

        return true;
    }

    return false;
}

bool Automate::sauvegarder(const string& nomFichier){

    ofstream fichier(nomFichier.c_str(), ios::out | ios::trunc);

    if (fichier.is_open()){
        // on ecrit le fichier avec les chevrons << dans le flux fichier

        for(list<string>::iterator iA=_A.begin(); iA!=_A.end(); iA++){ // le -- avant le end est ultra important car sinan il rajoute une space à ala fin de la ligne et je suis pas persuadé que ça ne fera pas bugger à la relecture :)

            if (iA != _A.begin())
                fichier << " ";
            fichier << *iA;
        }
        fichier << endl; // on passe à la ligne suivante

        for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

            if (iQ != _Q.begin())
                fichier << " ";

            for(list<string>::iterator iiQ=(*iQ)->begin(); iiQ!=(*iQ)->end(); iiQ++)
                fichier << *iiQ;

        }
        fichier << endl;

        for(list<list<string>*>::iterator iI=_I.begin(); iI!=_I.end(); iI++){

            if (iI != _I.begin())
                fichier << " ";

            for(list<string>::iterator iiI=(*iI)->begin(); iiI!=(*iI)->end(); iiI++)
                fichier << *iiI;
        }
        fichier << endl;

        for(list<list<string>*>::iterator iT=_T.begin(); iT!=_T.end(); iT++){

            if (iT != _T.begin())
                fichier << " ";

            for(list<string>::iterator iiT=(*iT)->begin(); iiT!=(*iT)->end(); iiT++)
                fichier << *iiT;
        }
        fichier << endl;

        for(list<list<string>*>::iterator iQ=_Q.begin(); iQ!=_Q.end(); iQ++){

            fichier << endl;

            for(list<string>::iterator iiQ=(*iQ)->begin(); iiQ!=(*iQ)->end(); iiQ++)
                fichier << *iiQ;

            for(unsigned int cpt=0; cpt<_A.size(); cpt++){

                fichier << " ";

                if (_tab[*iQ][cpt].empty())
                    fichier << "-";

                for(list<list<string>*>::iterator iTDT=_tab[*iQ][cpt].begin(); iTDT!=_tab[*iQ][cpt].end(); iTDT++){

                    if (iTDT != _tab[*iQ][cpt].begin())
                        fichier << ",";

                    for(list<string>::iterator iiTDT=(*iTDT)->begin(); iiTDT!=(*iTDT)->end(); iiTDT++)
                        fichier << *iiTDT;
                }
            }
        }

        return true;
    }

    return false;
}

void Automate::editTableTranstion(QStandardItemModel* modele){

    int ligne = 0, colonne;
    list<string>::iterator iA = _A.begin();
    QStringList label;
    QString etat;

    modele->clear();

    label << "State";
    for(list<string>::iterator iA=_A.begin(); iA!=_A.end(); iA++){

        label << (*iA).c_str();
    }

    modele->setHorizontalHeaderLabels(label);

    for(list<list<string>*>::iterator i=_Q.begin(); i!=_Q.end(); i++){

        colonne = 0;
        etat.clear();

        etat = "{";

        for(list<string>::iterator ii=(*i)->begin(); ii!=(*i)->end(); ii++){

            if(ii!=(*i)->begin())
                etat += ", ";

            etat += QString::fromStdString(*ii);
        }

        etat += "}";

        modele->setItem(ligne,colonne,new QStandardItem(etat));
        colonne++;

        for(unsigned int cpt=0; cpt<_A.size(); cpt++){

            etat.clear();

            if (_tab[*i][cpt].size() == 0) etat = "{ø}";

            for(list<list<string>*>::iterator ii=_tab[*i][cpt].begin(); ii!=_tab[*i][cpt].end(); ii++){

                if(ii != _tab[*i][cpt].begin())
                    etat += ", ";

                etat += "{";

                for(list<string>::iterator iii=(*ii)->begin(); iii!=(*ii)->end(); iii++){

                    if(iii != (*ii)->begin())
                        etat += ", ";

                    etat += QString::fromStdString(*iii);
                }

                etat += "}";
            }

            modele->setItem(ligne,colonne,new QStandardItem(etat));
            colonne++;
        }
        ligne++;
    }
}

void Automate::editA(QStringListModel* modele){

    QStringList liste;

    for(list<string>::iterator i=_A.begin(); i!=_A.end(); i++){

        liste << QString::fromStdString(*i);
    }

    modele->setStringList(liste);
}

void Automate::editQ(QStringListModel* modele){

    QStringList liste;
    QString temp;

    for(list<list<string>*>::iterator i=_Q.begin(); i!=_Q.end(); i++){

        temp.clear();

        for(list<string>::iterator ii=(*i)->begin(); ii!=(*i)->end(); ii++){

            temp += QString::fromStdString(*ii);
        }

        liste << temp;
    }

    modele->setStringList(liste);
}

void Automate::editI(QStringListModel* modele){

    QStringList liste;
    QString temp;

    for(list<list<string>*>::iterator i=_I.begin(); i!=_I.end(); i++){

        temp.clear();

        for(list<string>::iterator ii=(*i)->begin(); ii!=(*i)->end(); ii++){

            temp += QString::fromStdString(*ii);
        }

        liste << temp;
    }

    modele->setStringList(liste);
}

void Automate::editT(QStringListModel* modele){

    QStringList liste;
    QString temp;

    for(list<list<string>*>::iterator i=_T.begin(); i!=_T.end(); i++){

        temp.clear();

        for(list<string>::iterator ii=(*i)->begin(); ii!=(*i)->end(); ii++){

            temp += QString::fromStdString(*ii);
        }

        liste << temp;
    }

    modele->setStringList(liste);
}
