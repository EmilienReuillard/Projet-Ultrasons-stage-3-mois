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

/*
void defPinModMux(int a,int b,int c,int d){
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
}
*/

#endif