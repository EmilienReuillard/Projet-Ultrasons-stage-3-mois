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

Capteur Cap0(InpA0,0,5);
Capteur Cap1(InpA1,1,7);
Capteur Cap2(InpA2,2,9);
Capteur Cap3(InpA3,3,11);
Capteur Cap4(InpA4,4,13);
Capteur Cap5(InpA5,5,15);
Capteur Cap6(InpA6,6,17);

/*----------------------------------------------------------------------------------*/

double seuil = 10000; //Arbitraire en fonction des meures testés
int erreur = 1;  //erreur acceptable en ms
int detection_mode = 0; 
//0:salves 
//1:emission simples || If this mode is enable, the periode in the declaration is useless

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
  
  //Emission
  defPinModMux(2,3,4,5);

  //Gestion fréquence
  sampling_period_us = round(1000000*(1.0/samplingFrequency));

  //Serial
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");
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

        Cap0.emissionSalveSimulMux(i);
        Cap1.emissionSalveSimulMux(i);
        Cap2.emissionSalveSimulMux(i);
        Cap3.emissionSalveSimulMux(i);
        Cap4.emissionSalveSimulMux(i);
        Cap5.emissionSalveSimulMux(i);
        Cap6.emissionSalveSimulMux(i);

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
    Cap0.Prorocole_detection();
    Cap1.Prorocole_detection();
    Cap2.Prorocole_detection();
    Cap3.Prorocole_detection();
    Cap4.Prorocole_detection();
    Cap5.Prorocole_detection();
    Cap6.Prorocole_detection();

    /*AFFICHAGE*/
    Cap0.afficheReception();
    Cap1.afficheReception();
    Cap2.afficheReception();
    Cap3.afficheReception();
    Cap4.afficheReception();
    Cap5.afficheReception();
    Cap6.afficheReception();

    /* DELAY */
    //while(1);
    delay(2000); /* Repeat after delay */
  break;
  
  /*---------------------------------------------------------------------------------------*/
  /*------------------------------------------CASE 1---------------------------------------*/
  /*---------------------------------------------------------------------------------------*/

  case 1:
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
    Cap0.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap0.Prorocole_alternance_capt(i);
      if( Cap0.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap1*/
    microseconds = micros();
    Cap1.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap1.Prorocole_alternance_capt(i);
      if( Cap1.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap2*/
    microseconds = micros();
    Cap2.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap2.Prorocole_alternance_capt(i);
      if( Cap2.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap3*/
    microseconds = micros();
    Cap3.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap3.Prorocole_alternance_capt(i);
      if( Cap3.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap4*/
    microseconds = micros();
    Cap4.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap4.Prorocole_alternance_capt(i);
      if( Cap4.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap5*/
    microseconds = micros();
    Cap5.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap5.Prorocole_alternance_capt(i);
      if( Cap5.validationBreak() == 1){break;}
      while(micros() - microseconds < sampling_period_us){
          //empty loop
      }
      microseconds += sampling_period_us;
    }

    /*Cap6*/
    microseconds = micros();
    Cap6.emissionSimpleMux();
    for(int i=0; i<samples; i++)
    {
      Cap6.Prorocole_alternance_capt(i);
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
  break;

  /*---------------------------------------------------------------------------------------*/
  /*------------------------------------------CASE 2---------------------------------------*/
  /*---------------------------------------------------------------------------------------*/
  case 2:

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
