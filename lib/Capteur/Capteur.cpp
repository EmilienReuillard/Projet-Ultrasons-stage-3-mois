#include <capteur.h>
#include <Fonctions.h>

Capteur::Capteur(int pinIn_, int pinOUTA_, int pinOUTB_, int T_,int N_capt_){
  this->pinIn = pinIn_;
  this->pinOUTA = pinOUTA_;
  this->pinOUTB = pinOUTB_;
  this->T = T_;
  this->N_capt = N_capt_;
}

Capteur::~Capteur(){

}

void Capteur::defPinMod(){
  pinMode(pinIn,INPUT);
}

void Capteur::MaZ(){
    this->lock_first_em = 0;    //vérou mémoire pour l'émission
    this->first_em = 0;         //mémoire de la première émission
    this->first_em_real_time = 0;
    this->lock_first_re = 0;    //vérou mémoire pour la réception
    this->first_re = 0;         //mémoire de la première la réception
    this->first_re_real_time = 0;
    this->compt = 0;            //compte le nombre d'émission
    this->dist = 0;             //distance
    this->dist_real_time = 0;
    this->valid = 0;
    this->sum = 0;
    this->moy_sans_zero = 0; 
}

/*----------------------------------------------------------------------------------*/
/*------------------------------------CASE 0----------------------------------------*/
/*----------------------------------------------------------------------------------*/

void Capteur::emissionSalveMux(int i){
  //Pins du mux en entrée// 1: car OUT bare
  int S0 = 0; 
  int S1 = 0;
  int S2 = 0;
  int S3 = 0;

  //Capteur sur lequel on veut émettre; ATTENTION, ce n'est le choix de la pin de sortie!
  switch (this->pinOUTA)
  {
    case 0:
      S0 = LOW; S1 = LOW;   S2 = LOW;   S3 = LOW;      
    break;
    
    case 1:
      S0 = LOW; S1 = HIGH;  S2 = LOW;   S3 = LOW;
    break;
    
    case 2:
      S0 = LOW; S1 = LOW;   S2 = HIGH;  S3 = LOW;
    break;
    
    case 3:
      S0 = LOW; S1 = HIGH;  S2 = HIGH;  S3 = LOW;
    break;
    
    case 4:
      S0 = LOW; S1 = LOW;   S2 = LOW;   S3 = HIGH;
    break;
    
    case 5:
      S0 = LOW; S1 = HIGH;  S2 = LOW;   S3 = HIGH;
    break;
    
    case 6: 
      S0 = LOW; S1 = LOW;   S2 = HIGH;  S3 = HIGH;
    break;

    //si on ne veut pas émettre, on emmet sur une pin pas connectée
    case 7:
      S0 = LOW; S1 = HIGH;  S2 = HIGH;  S3 = HIGH;
    break;
  }

  //si bon timing émission
  if(T * (int)compt == i*((int)samplingFrequency/1000) && (int)compt < N_em){ //(freq/1000) si on doit augmanter la fréquence.
    digitalWrite(2,S0);
    digitalWrite(3,S1);
    digitalWrite(4,S2);
    digitalWrite(5,S3);
    if(lock_first_em == 0){
      first_em = i*1000*(1.0/samplingFrequency);  //en ms
      lock_first_em = 1;
    }
    compt++;
  }

  //sinon pas d'émission
  else{
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
    digitalWrite(5,HIGH);
  }
}


//Fonctionne dans une boucle, et i a une dimention temporelle qui détermine si oui ou non on peut émettre
void Capteur::emissionSalve(int i){
  //si bon timing émission
  if(T * (int)compt == i*((int)samplingFrequency/1000) && (int)compt < N_em){ //(freq/1000) si on doit augmanter la fréquence.
    digitalWrite(pinOUTA,HIGH);
    if(lock_first_em == 0){
      first_em = i*1000*(1.0/samplingFrequency);  //en ms
      first_em_real_time = micros();
      lock_first_em = 1;
    }
    digitalWrite(pinOUTA,LOW);
    compt++;
  }
}

void Capteur::uploadData(int i){
    vReal[i] = analogRead(pinIn);
    vImag[i] = 0;
    sum += vReal[i];    //On en profite pour additionner toutes les val de la liste au fur et a mesure pour économiser une boucle plus tard
}

void Capteur::real2der(int mode){
    /* Periode */
    float T = 1.0/samplingFrequency;

    /* Calcul des dérivées */

    switch (mode)
    {
    case 0: //avance
      for (int i = 0; i < samples-1; i++){
          this->vReal_der[i] = (this->vReal[i+1]-this->vReal[i])/(1.0*T);    //Dérivée avent
      }
      this->vReal_der[samples] = 0;  //Permet de garder la même nombre d'element dans la liste
      break;

    case 1: //retard
      this->vReal_der[0] = 0;  //Permet de garder la même nombre d'element dans la liste
      for (int i = 1; i < samples; i++){
          this->vReal_der[i] = ((this->vReal[i]-this->vReal[i-1])*1.0)/(1.0*T);    //Dérivée arrière
      }
      break;

    case 2: //moyenne
      this->vReal_der[0] = 0;    //Permet de garder la même nombre d'element dans la liste
      for (int i = 1; i < samples-1; i++){
          this->vReal_der[i] = ((this->vReal[i+1]-this->vReal[i-1])*1.0)/(1.0*T);    //Dérivée centrée
      }
      this->vReal_der[samples] = 0;  //Permet de garder la même nombre d'element dans la liste
      break;
    
    default:
      break;
    }
}

//Calcul la moyenne à la fin de la prise des mesures
void Capteur::moyenne(){
    moy = sum/(samples*1.0);
}

void Capteur::ech_a_zero(){
    for( int i = 0; i < samples; i++){
    vReal_ech[i] = abs(vReal[i]-moy);
    }

}


//Prend la liste dérivée, et renvoie une liste bianaire
void Capteur::der2bin(){
    double somme = 0; //somme pour le calcul de la moyenne de la list dérivée sans 0
    int Nb = 0;       //Nb le valeurs dfférentes de 0
    for(int i = 0; i < samples; i++){
        if(vReal_der[i] > 0){   //on ne garde que les valeures positives pour détecter uniquement les fronts montants
        somme += vReal_der[i];
        Nb++;
        }
    }
    this->moy_sans_zero = somme/Nb;
    for(int i = 0; i < samples; i++){
        if(vReal_der[i] > moy_sans_zero){
        vReal_bin[i] = 1;
        }
        else{
        vReal_bin[i] = 0;
        }
    }
}

//recherche si la période qu'on entre est présente (on travaille en ms); Te: per recherché; Nvalid: nb de valid nécéssaire pour valider le signal
//recherche aussi la première réception validée
int Capteur::valid_freq(int Nvalid, int err){

    double T1 = (1.0/samplingFrequency)*1000; //pour 1000Hz, T=1
    this->delta = this->T/T1; //Serial.print("delta:"); Serial.println(delta);    //delta: nb de pas recherché entre 2 impultions
    int compteur = 0; //compteur de validation
    this->lock_first_re = 0;
  
  for(int i = 0; i < samples; i++){
    if(vReal_bin[i] == 1){
      
      if(lock_first_re == 0){ //mise en mémoire 1ere réception
        this->first_re = i*1000*(1.0/samplingFrequency);  //en ms
        this->first_re_real_time = micros();
        this->lock_first_re = 1;
      }
      
      for(int j = 0; j < Nvalid; j++){
        if(i + j*delta >= samples){return 0;}
        
        if((vReal_bin[i + j*delta] == 1 || vReal_bin[i + j*delta - err] == 1)){  //Seconde condition pour contrer le bruit blanc
          compteur++;
        }
      }
      
      if(compteur >= Nvalid){ //signal validé
        this->valid = 1;
        this->distance(); //On calcule la distance
        //Serial.print("first reception : ");Serial.println(this->first_re);
        return 1;
      }
      else{ //signal non validé
        this->first_re = 0;  //en ms
        this->lock_first_re = 0;
        this->first_re_real_time = 0;
        compteur = 0;
      }
    }
  }
  //Serial.println("No frequency validation");
  return 0; //si aucun signal n'a été validé, retourne 0;
}

int Capteur::Prorocole_0(){
  this->moyenne();        //On fait la moyenne des données
  this->ech_a_zero();     //On échelonne les donné à cette moyenne
  this->real2der();       //On dérive la liste
  this->der2bin();        //Passage en binaire|| 1 : reception/ 0 : pas de réception
  this->valid_freq(4,1);  //On cherche la fréquence du capteur
}


/*----------------------------------------------------------------------------------*/
/*------------------------------------CASE 1----------------------------------------*/
/*----------------------------------------------------------------------------------*/

void Capteur::Prorocole_1(int i){
    this->uploadData(i);
    this->derivAndBinAuPas(i);
    this->detectionSimple(i);
    this->distance();
}

void Capteur::emissionSimpleMux(int i){
  //Pins du mux en entrée// 1: car OUT bare
  int S0 = 0; 
  int S1 = 0;
  int S2 = 0;
  int S3 = 0;

  //Capteur sur lequel on veut émettre; ATTENTION, ce n'est le choix de la pin de sortie!
  switch (this->pinOUTA)
  {
    case 0:
      S0 = LOW; S1 = LOW;   S2 = LOW;   S3 = LOW;      
    break;
    
    case 1:
      S0 = LOW; S1 = HIGH;  S2 = LOW;   S3 = LOW;
    break;
    
    case 2:
      S0 = LOW; S1 = LOW;   S2 = HIGH;  S3 = LOW;
    break;

    case 3:
      S0 = LOW; S1 = HIGH;  S2 = HIGH;  S3 = LOW;
    break;
    
    case 4:
      S0 = LOW; S1 = LOW;   S2 = LOW;   S3 = HIGH;
    break;
    
    case 5:
      S0 = LOW; S1 = HIGH;  S2 = LOW;   S3 = HIGH;
    break;
    
    case 6: 
      S0 = LOW; S1 = LOW;   S2 = HIGH;  S3 = HIGH;
    break;

    //si on ne veut pas émettre, on emmet sur une pin pas connectée
    case 7:
      S0 = LOW; S1 = HIGH;  S2 = HIGH;  S3 = HIGH;
    break;
  }

  digitalWrite(2,S0);
  digitalWrite(3,S1);
  digitalWrite(4,S2);
  digitalWrite(5,S3);
  
  if(lock_first_em == 0){
    first_em = i*1000*(1.0/samplingFrequency);  //en ms
    first_em_real_time = micros();  //mise en mémoire du temps réel
    lock_first_em = 1;
  }
  //On remet en LOW à la fin
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
}

void Capteur::emissionSimple(int i){
  //si bon timing émission
  digitalWrite(pinOUTA,HIGH);
  first_em_real_time = micros();
  first_em = i*1000*(1.0/samplingFrequency);  //en ms
  digitalWrite(pinOUTA,LOW);
  }

int Capteur::derivAndBinAuPas(int i){
  if(i==samples-1){
    vReal_der[i] = 0;
  }
  if(i>0){
    this->vReal_der[i-1] = (vReal[i]-vReal[i-1])/(1.0*T); //vReal_der[i-1] pour commencer à 0 et éviter un out of range avec vReal lors de l primière étape
  }
  if(vReal_der[i-1]>seuil){
    this->vReal_bin[i-1] = 1;
  }
  else{
    this->vReal_bin[i-1] = 0;
  }
  return 0;
}

int Capteur::detectionSimple(int i){ 
  if(i < samples && vReal_bin[i] == 1 ){
    this->valid = 1;
    if(this->lock_first_re == 0 ){
      this->first_re = i*1000*(1.0/samplingFrequency);  //en ms
      this->first_re_real_time = micros();
      this->lock_first_re = 1;
    }
  }
  else{
    this->valid = 0;
  }
  return 0;
}

int Capteur::validationBreak(){
  if(this->valid == 1){
    return 1;
  }
  else{
    return 0;
  }
}

/*----------------------------------------------------------------------------------*/
/*------------------------------------CASE 2----------------------------------------*/
/*----------------------------------------------------------------------------------*/

//on fait le même travail que lors d'une emission par salve simultané
void Capteur::Prorocole_2(){
  this->moyenne();
  this->ech_a_zero();
  this->real2der();
  this->der2bin();
  this->valid_freq(3,1);
}

/*----------------------------------------------------------------------------------*/
/*------------------------------------CASE 3----------------------------------------*/
/*----------------------------------------------------------------------------------*/

//on fait le même travail que lors d'une emission par salve simultané sauf que l'on recherche uniquement une seul impultion.
void Capteur::Prorocole_3(){
  this->moyenne();
  this->ech_a_zero();
  this->real2der();
  this->der2bin();
  this->detectionSimple_3();
}

void Capteur::detectionSimple_3(){
  for(int i = 0; i < samples ; i++){
    if(vReal_bin[i] == 1){
      this->valid = 1;
      if(lock_first_re == 0){
        first_re = i;
        first_em_real_time = micros();
        lock_first_re = 1;
      }
    }
  }
}

/*----------------------------------------------------------------------------------*/
/*------------------------------------OTHERS----------------------------------------*/
/*----------------------------------------------------------------------------------*/

void Capteur::distance(int affiche){
    dist = (first_re - first_em) * SOUND_SPEED * 0.5;
    dist_real_time = (first_re_real_time - first_em_real_time)*1000*SOUND_SPEED*0.5;  //*1000 car le temps est micros secondes
    if(affiche==1){
        Serial.print("D = ");
        Serial.println(dist);
    }
    if(affiche==2){
      Serial.print("D_real =");
      Serial.println(dist_real_time);
    }
}

void Capteur::afficheReception(){
  Serial.print("Capt");Serial.print(this->N_capt);Serial.print(" ; ");
  Serial.print("Valid = ");Serial.print(this->valid);Serial.print(" ; ");
  Serial.print("D = ");Serial.print(this->dist);Serial.print(" ; ");
  Serial.print("Moy = ");Serial.println(this->moy_sans_zero);
}

void Capteur::affiche(){
  Serial.print("\nmoy: "); 
  Serial.println(moy);
  Serial.print("\nMoySans0: ");
  Serial.println(this->moy_sans_zero);
  Serial.print("valid: ");
  Serial.println(this->valid);
  Serial.print("Delta: ");
  Serial.println(this->delta);
  Serial.println("\nData:\n");
  affiche_list(vReal);
  Serial.println("\nEchelonné:\n");
  affiche_list(vReal_ech);
  Serial.println("\nlist_dérivée:\n");
  affiche_list(vReal_der);
  Serial.println("\nlist_binaire:\n");
  affiche_list(vReal_bin);

  Serial.print("\nvalid: ");
  Serial.println(this->valid);
}

void Capteur::N_detect_freq(int compt_loop){
  if(compt_loop < N_loop && this->valid == 1){
    N_succes++;
  }
  lst_comparaison_perf[compt_loop] = N_succes;
  if(compt_loop >= N_loop){
    Serial.print("Capteur "); Serial.print(N_capt); Serial.print(" ; ");
    Serial.print("N_succès = "); Serial.println(N_succes);

    //Serial.print("=====FIN DU PROGRAMME=====");
    //while(1);
  }
}