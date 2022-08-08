/*
Librairie mère de Capteur
Elle a pour but de regrouper les différents capteurs pour simplifier le code
*/

#ifndef CARTE_H
#define CARTE_H

#include <Capteur.h>

class Carte
{
    public:
        Carte(int N);
        ~Carte();

    private:
        int N_capt = 7; //nombre de capteurs

        int firt_pinIN = 0; //première pin ana que l'on va utiliser
        int firt_pinOUT= 0; //première pin dig que l'on va utiliser

        Capteur lst_capt[];

};

#endif