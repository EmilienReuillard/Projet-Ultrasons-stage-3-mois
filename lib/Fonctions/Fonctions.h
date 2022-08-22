#ifndef FONCTIONS_H
#define FONCTIONS_H

#include "Capteur.h"

#include <Arduino.h>

static inline void affiche_list(const double *list){
    for (int i = 0; i < samples; i++)
    {
        Serial.println(list[i]);
    }
}

static inline void affiche_validations(Capteur CapA,Capteur CapB,Capteur CapC,Capteur CapD,Capteur CapE,Capteur CapF,Capteur CapG, int N=300){
  Serial.println("===AFFICHAGE DES RECEPTIONS===");
  Serial.println("Cap0  Cap1  Cap2  Cap3  Cap4  Cap5  Cap6  Cap7");
  for(int i = 0 ; i < N ; i++){
    Serial.print(CapA.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(CapB.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(CapC.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(CapD.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(CapE.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(CapF.lst_comparaison_perf[i]); Serial.print(";");
    Serial.println(CapG.lst_comparaison_perf[i]);
  }
  Serial.println("===FIN D'AFFICHAGE===");
}

/*
void defPinModMux(int a,int b,int c,int d){
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
}
*/

#endif