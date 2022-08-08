#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <Arduino.h>

#define samples 256 //pourra être changé //This value MUST ALWAYS be a power of 2
#define samplingFrequency 1000 //fréquence d'échantillonage en Hz must be less than 10000 due to ADC
#define SOUND_SPEED 0.340 //m/ms

class Capteur
{   
  public:
  
    Capteur(int pinIn, int pinOUT, int T);
    ~Capteur();

    void defPinMod(); //pinMode(pinIN) ; pinMode(pinOUT)

    void MaZ(); //Mise a Zero des différentes variables

    void emissionMux(int i);  //Emission en fonction de la position de i (dans le temps)
    void emissionSimpleMux();

    void uploadData(int i);  //met a jours le i-ème élément de la liste vReal

    void deriv_list(int mode = 0); //Dérive vReal dans vReal_der
    void moyenne();
    void ech_a_zero();  //échelonne à 0 à partir de la moyenne
    void detection();  //renvoie un signal binaire en fonction de ce a été ou non détecté
    int detectionImpulsion(); //détecte une impulsion unique
    int  valid_freq(int Nvalid = 3, int err = 1);    //Vérifie si un signal a bien été détecté

    int Prorocole_detection();  //Regroupe les 5 fonctions précédentes
    void Prorocole_alternance_capt(); //programme le plus simple possible pour mesurer les distances. A utiliser avec emissionSimpleMux()

    void distance(int affiche=0);  //calcul la distance; i = 1: affiche;  i = 0: affiche pas

    void affiche(); //affiche les données essentielles



  private:

    /* NOMBRE DE CAPT */
    int N_capt_tot = 0; //nombre de capteur total

    /* PERIODE */
    int T = 0;  //ms

    /* PINS */
    int pinIn;  //Réception
    int pinOUT; //Emission

    /* ARRAY */
    double vReal[samples];      //Données Brutes
    double vReal_ech[samples];  //Echelloné
    double vReal_der[samples];  //Dérivée
    double vReal_bin[samples];  //Binaire
    double vImag[samples];      //Imaginaire

    /*  VERROUS/MEMOIRES */
    int lock_first_em = 0;  //vérou mémoire pour l'émission
    int first_em = 0;       //mémoire de la première émission

    int lock_first_re = 0;  //vérou mémoire pour la réception
    int first_re = 0;       //mémoire de la première la réception

    int compt = 0;          //compte le nombre d'émission
    int N_em = 5;           //défini le nb d'émission voulues

    /* Moyennes */
    double sum = 0; //somme de toutes les données d'une liste pour faire la moyenne après
    double moy = 0; //moyenne

    /* Validation de la détection du signal*/
    int valid = 0;

    /* Emission */
    int compt = 0;  //compte de nombre de plip qui ont été émis

    /* Distances */
    double dist = 0;  //distance
    
};

void affiche_list(const double *list){
    for (int i = 0; i < samples; i++)
    {
        Serial.println(list[i]);
    }
}

void defPinModMux(int a,int b,int c,int d){
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
}
#endif
