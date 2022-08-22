#ifndef CAPTEUR_H
#define CAPTEUR_H

#include <Arduino.h>

#define samples 256 //pourra être changé //This value MUST ALWAYS be a power of 2
#define samplingFrequency 1000 //fréquence d'échantillonage en Hz must be less than 10000 due to ADC
#define SOUND_SPEED 0.340 //m/ms

class Capteur
{   
  public:
  
    Capteur(int pinIn, int pinOUTA, int pinOUTB, int T = 0 , int N_capt=0);
    ~Capteur();
    void defPinMod(); //pinMode(pinIN) ; pinMode(pinOUT)
    
    /*FONCTIONS GENERALES*/
    void MaZ(); //Mise a Zero des différentes variables
    void uploadData(int i);           //met a jours le i-ème élément de la liste vReal
    void real2der(int mode = 0);      //Dérive vReal dans vReal_der
    void der2bin();                   //renvoie un signal binaire en fonction de ce a été ou non détecté à partir de la liste dérivée
    int  valid_freq(int Nvalid = 4, int err = 1);    //Vérifie si un signal a bien été détecté à la fréquence voulue
    void ech_a_zero();                //échelonne à 0 à partir de la moyenne définie au bréalable
    void moyenne();                   //fait la moyenne à partir de sum (généré dans uploadData)

    /*EMISSION D'ONDES*/
    void emissionSalveMux(int i);       //Emission en fonction de la position de i (dans le temps)
    void emissionSalve(int i);          //A utiliser dans un circuit sans MUX

    void emissionSimpleMux(int i = 0);  //Emet une fois// On met i=0 par defaut, car il emmet juste avant que l'on lance la captation du son. 
    void emissionSimple(int i = 0);

    /*DETECTION D'ONDES*/
    int  detectionSimple(int i); //détecte une impulsion unique
    void detectionSimple_3();

    /*PROTOCOLES TRAITEMENT DU SIGNAL*/

    int  Prorocole_0();       //Regroupe les 5 fonctions précédentes
    void Prorocole_1(int i);  //programme le plus simple possible pour mesurer les distances. A utiliser avec emissionSimpleMux()
    void Prorocole_2();
    void Prorocole_3();

    /*CASE1 FUNCTION*/
    void BreakProtocole(int i); //Sort de la boucle de mesure si on capte un truc.
    int derivAndBinAuPas(int i); //dérive pas à pas et replis la liste binaire en fonction d'un seuil prédéfini.
    int validationBreak();  //si un signal est détecté, (this->valid) on renvoie 1, pour faire un break.

    /*DISTANCE*/
    void distance(int affiche=0);  //calcul la distance; i = 1: affiche;  i = 0: affiche pas

    /*AFFICHAGE*/
    void afficheReception();  //Affiche pour chaque capteur si il reçoit ou non, ainsi que la distance
    void affiche(); //affiche les données essentielles

    /*ETUDE DES RESUTATS*/
    //Fonction qui va déterminer le nombre de validation pour N boucle. Crée pour évaluer l'influence de la Période T sur la mesure
    void N_detect_freq(int compt_loop);  
    int lst_comparaison_perf[300];

  private:

    /* NOMBRE DE CAPT */
    int N_capt_tot = 0; //nombre de capteur total
    int N_capt = 0;     //n° attitré du capteur

    /* PERIODE */
    int T = 0;  //ms
    int delta = 0; //nombre de pas entre 2 impulsions

    /* PINS */
    int pinIn;    //Réception
    int pinOUTA;  //Emission sur A
    int pinOUTB;  //Emission sur B

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

    int first_em_real_time = 0;
    int first_re_real_time = 0;

    int compt = 0;          //compte le nombre d'émission
    int N_em = 6;           //défini le nb d'émission voulues

    /* Moyennes */
    double sum = 0; //somme de toutes les données d'une liste pour faire la moyenne après
    double moy = 0; //moyenne
    double moy_sans_zero = 0;

    /* Validation de la détection du signal*/
    int valid = 0;
    int seuil = 5000; //seuil de détection dérivée pour Detection simple

    /* Distances */
    double dist = 0;  //distance
    double dist_real_time = 0;  //distance calculé avec le temps réel

    /*ETUDE DES RESUTATS*/
    int N_loop = 100; //nombre de boucle que l'on va faire pour 1 échatillon
    int N_succes = 0; //compte de nombre de validation en sur une étude
    
};

#endif
