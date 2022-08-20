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

static inline void affiche_validations(Capteur Cap0,Capteur Cap1,Capteur Cap2,Capteur Cap3,Capteur Cap4,Capteur Cap5,Capteur Cap6, int N=300){
  Serial.println("===AFFICHAGE DES RECEPTIONS===");
  Serial.println("Cap0  Cap1  Cap2  Cap3  Cap4  Cap5  Cap6  Cap7");
  for(int i = 0 ; i < N ; i++){
    Serial.print(Cap0.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(Cap1.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(Cap2.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(Cap3.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(Cap4.lst_comparaison_perf[i]); Serial.print(";");
    Serial.print(Cap5.lst_comparaison_perf[i]); Serial.print(";");
    Serial.println(Cap6.lst_comparaison_perf[i]);
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