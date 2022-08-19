#include "arduinoFFT.h"
#include "math.h"
#include "Capteur.h"
#include "Fonctions.h"

/*Declaration Pins Analogiques*/
#define InpA0 A0
#define InpA1 A1
#define InpA2 A2
#define InpA3 A3
#define InpA4 A4
#define InpA5 A5
#define InpA6 A6

/*------------------------------------FONCTIONS-------------------------------------*/
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);

/*------------------------------------------------------------------------------------
------------------------------------------MAIN----------------------------------------
--------------------------------------------------------------------------------------*/

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */


unsigned int sampling_period_us;
unsigned long microseconds;

/*----------------------------------------------------------------------------------*/
/*DECLARATION DES CAPTEURS ---------------------------------------------------------*/
/*----------------------------------------------------------------------------------*/

Capteur Cap0(InpA6,0,1,5,0);
Capteur Cap1(InpA5,2,3,7,1);
Capteur Cap2(InpA4,4,5,9,2);
Capteur Cap3(InpA3,6,7,11,3);
Capteur Cap4(InpA0,8,9,13,4);
Capteur Cap5(InpA1,10,11,15,5);
Capteur Cap6(InpA2,12,13,17,6);

//Initialisation du compteur de boucle
int compt_loop = 0; //Compteur de boucle

/*----------------------------------------------------------------------------------*/

double seuil = 10000; //Arbitraire en fonction des meures testés
int erreur = 1;  //erreur acceptable en ms


int detection_mode = 0; 
//0:emission salves simulanées 
//1:emission simples || If this mode is enable, the periode in the declaration is useless
//2:emission salves step by step

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

/*---------------------------------------------------------------------------------------*/
/*------------------------------------------SETUP----------------------------------------*/
/*---------------------------------------------------------------------------------------*/
void setup()
{
  /*DEFINITION PINS*/
  //Réception
  Cap0.defPinMod();
  Cap1.defPinMod();
  Cap2.defPinMod();
  Cap3.defPinMod();
  Cap4.defPinMod();
  Cap5.defPinMod();
  Cap6.defPinMod();
  
  //Gestion fréquence
  sampling_period_us = round(1000000*(1.0/samplingFrequency));
  //Serial
  Serial.begin(115200);
  //Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
  delay(8000);
}

/*---------------------------------------------------------------------------------------*/
/*------------------------------------------LOOP-----------------------------------------*/
/*---------------------------------------------------------------------------------------*/

void loop()
{
  switch (detection_mode)
  {
  /*---------------------------------------------------------------------------------------*/
  /*------------------------------------------CASE 0---------------------------------------*/
  /*---------------------------------------------------------------------------------------*/
  /*
  On emet des salves dès l'instant t0. On mesure tout en même temps, et on fait les verifications à la fin
  Puis on affiche les résultats.
  */
  case 0:
    /*REMISE A ZEROS VARIABLES*/
    Cap0.MaZ();
    Cap1.MaZ();
    Cap2.MaZ();
    Cap3.MaZ();
    Cap4.MaZ();
    Cap5.MaZ();
    Cap6.MaZ();
    
    /*SAMPLING*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
        /*EMISSION*/

        Cap0.emissionSalve(i);
        Cap1.emissionSalve(i);
        Cap2.emissionSalve(i);
        Cap3.emissionSalve(i);
        Cap4.emissionSalve(i);
        Cap5.emissionSalve(i);
        Cap6.emissionSalve(i);

        /*RECEPTION*/
        Cap0.uploadData(i);
        Cap1.uploadData(i);
        Cap2.uploadData(i);
        Cap3.uploadData(i);
        Cap4.uploadData(i);
        Cap5.uploadData(i);
        Cap6.uploadData(i);
        
        while(micros() - microseconds < sampling_period_us){
          //empty loop
        }
        microseconds += sampling_period_us;
    }
    
    /* Traitement du signal */
    Cap0.Prorocole_0();
    Cap1.Prorocole_0();
    Cap2.Prorocole_0();
    Cap3.Prorocole_0();
    Cap4.Prorocole_0();
    Cap5.Prorocole_0();
    Cap6.Prorocole_0();

    /*AFFICHAGE*/
    Serial.println("===================================");
    Serial.println(millis()/1000);  //On affiche le temps en seconde
    Serial.println("===================================\n");

    Cap0.afficheReception();
    Cap1.afficheReception();
    Cap2.afficheReception();
    Cap3.afficheReception();
    Cap4.afficheReception();
    Cap5.afficheReception();
    Cap6.afficheReception();
    
    //Cap0.affiche();

    /*ETUDE INFLUENCE DE LA PERIODE SUR LA MESURE*/ //A commenter ça sinon ça bloque au bout de 20 itération par defaut
    Cap0.N_detect_freq(compt_loop);
    compt_loop++;
    

    /* DELAY */
    //while(1);
    delay(0); /* Repeat after delay */
  break;

  /*---------------------------------------------------------------------------------------*/
  /*------------------------------------------CASE 1---------------------------------------*/
  /*---------------------------------------------------------------------------------------*/
  /*
  On emet des ondes uniques dès l'instant t0. On mesure tout en même temps, et on fait les verifications à la fin
  Puis on affiche les résultats.
  */
  case 1:
    /*REMISE A ZEROS VARIABLES*/
    Cap0.MaZ();
    Cap1.MaZ();
    Cap2.MaZ();
    Cap3.MaZ();
    Cap4.MaZ();
    Cap5.MaZ();
    Cap6.MaZ();
    
    /*EMISSION*/

    Cap0.emissionSimple();
    Cap1.emissionSimple();
    Cap2.emissionSimple();
    Cap3.emissionSimple();
    Cap4.emissionSimple();
    Cap5.emissionSimple();
    Cap6.emissionSimple();

    /*SAMPLING*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
        /*RECEPTION*/
        Cap0.uploadData(i);
        Cap1.uploadData(i);
        Cap2.uploadData(i);
        Cap3.uploadData(i);
        Cap4.uploadData(i);
        Cap5.uploadData(i);
        Cap6.uploadData(i);
        
        while(micros() - microseconds < sampling_period_us){
          //empty loop
        }
        microseconds += sampling_period_us;
    }

    /* Traitement du signal */
    Cap0.Prorocole_3();
    Cap1.Prorocole_3();
    Cap2.Prorocole_3();
    Cap3.Prorocole_3();
    Cap4.Prorocole_3();
    Cap5.Prorocole_3();
    Cap6.Prorocole_3();

    /*AFFICHAGE*/
    Cap0.afficheReception();
    Cap1.afficheReception();
    Cap2.afficheReception();
    Cap3.afficheReception();
    Cap4.afficheReception();
    Cap5.afficheReception();
    Cap6.afficheReception();
    Serial.println("===================================\n");


    /* DELAY */
    //while(1);
    delay(2000); /* Repeat after delay */
  break;

  
  /*---------------------------------------------------------------------------------------*/
  /*------------------------------------------CASE 2---------------------------------------*/
  /*---------------------------------------------------------------------------------------*/
  /*
  Cette partie emet une salve, reçoit et calcule, puis fait un traitement des données pour voir si la fréquence est validée
  Et on fait ça avec chaque capteur.
  C'est le CASE avec plus de vérification en somme. Un mix entre CASE0 et CASE1
  */

  case 2:
    /*REMISE A ZEROS VARIABLES*/
    Cap0.MaZ();
    Cap1.MaZ();
    Cap2.MaZ();
    Cap3.MaZ();
    Cap4.MaZ();
    Cap5.MaZ();
    Cap6.MaZ();
    
    
    /*________________Cap0________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap0.emissionSalve(i);  //Emission si bon timing
      Cap0.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /*Traitement du signal*/
    Cap0.Prorocole_2();

    /*Affichage*/
    Cap0.afficheReception();


    /*________________Cap1________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap1.emissionSalve(i);  //Emission si bon timing
      Cap1.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /* Traitement du signal */
    Cap1.Prorocole_2();

    /*Affichage*/
    Cap1.afficheReception();


    /*________________Cap2________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap2.emissionSalve(i);  //Emission si bon timing
      Cap2.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /* Traitement du signal */
    Cap2.Prorocole_2();

    /*Affichage*/
    Cap2.afficheReception();


    /*________________Cap3________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap3.emissionSalve(i);  //Emission si bon timing
      Cap3.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /* Traitement du signal */
    Cap3.Prorocole_2();

    /*Affichage*/
    Cap3.afficheReception();


    /*________________Cap4________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap4.emissionSalve(i);  //Emission si bon timing
      Cap4.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /* Traitement du signal */
    Cap4.Prorocole_2();

    /*Affichage*/
    Cap4.afficheReception();


    /*________________Cap5________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap5.emissionSalve(i);  //Emission si bon timing
      Cap5.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /* Traitement du signal */
    Cap5.Prorocole_2();

    /*Affichage*/
    Cap5.afficheReception();


    /*________________Cap6________________*/
    microseconds = micros();
    for(int i=0; i<samples; i++)
    {
      /*EMISSION/Reception*/
      Cap6.emissionSalve(i);  //Emission si bon timing
      Cap6.uploadData(i);     //Met a jours vReal
      while(micros() - microseconds < sampling_period_us){
        //empty loop
      }
      microseconds += sampling_period_us;
    }
    /* Traitement du signal */
    Cap6.Prorocole_2();

    /*Affichage*/
    Cap6.afficheReception();

    Serial.println("===================================\n");

  break;


  /*---------------------------------------------------------------------------------------*/
  /*------------------------------------------CASE 3---------------------------------------*/
  /*---------------------------------------------------------------------------------------*/
  /*
  On emet un blip et on regarde le temps qu'il met a revenir. Si on capte un truc, return 1.
  Pas de vérification
  */

  case 3:
    /*REMISE A ZEROS VARIABLES*/
    Cap0.MaZ();
    Cap1.MaZ();
    Cap2.MaZ();
    Cap3.MaZ();
    Cap4.MaZ();
    Cap5.MaZ();
    Cap6.MaZ();

    //Avec les parametres par defaut, une boucle met à peu près 0.2s a se faire si rien n'est capté
    /*Cap0*/
    microseconds = micros();
    Cap0.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap0.Prorocole_1(i);
      if( Cap0.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap1*/
    microseconds = micros();
    Cap1.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap1.Prorocole_1(i);
      if( Cap1.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap2*/
    microseconds = micros();
    Cap2.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap2.Prorocole_1(i);
      if( Cap2.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap3*/
    microseconds = micros();
    Cap3.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap3.Prorocole_1(i);
      if( Cap3.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap4*/
    microseconds = micros();
    Cap4.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap4.Prorocole_1(i);
      if( Cap4.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap5*/
    microseconds = micros();
    Cap5.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap5.Prorocole_1(i);
      if( Cap5.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap6*/
    microseconds = micros();
    Cap6.emissionSimple();
    for(int i=0; i<samples; i++)
    {
      Cap6.Prorocole_1(i);
      if( Cap6.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*AFFICHAGE*/
    Cap0.afficheReception();
    Cap1.afficheReception();
    Cap2.afficheReception();
    Cap3.afficheReception();
    Cap4.afficheReception();
    Cap5.afficheReception();
    Cap6.afficheReception();

    Serial.println("===================================\n");


  break;

  }
}

/*----------------------------------------------------------------------------------*/
/*------------------------------------FONCTIONS-------------------------------------*/
/*----------------------------------------------------------------------------------*/

void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
