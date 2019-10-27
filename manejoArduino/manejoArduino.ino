/****************************************** Incluyo las librerias ******************************************/
#include "Adafruit_PWMServoDriver.h"

/****************************************** Declaro e inicializo el PWM ******************************************/
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/****************************************** Defino constantes ******************************************/
#define INCR    5     // "angle-step", from current angle to the destination angle
#define INCRDEL 20 // "time-step", ms delay for each angle-step
#define DELAY_ENTRE_SERVOS 250 

#define HEAD    0   // Head
#define RSHLDRT 1   // Right shoulder articulation with torso
#define RSHLDRA 2   // Right shoulder articulation with arm
#define RELBOW  3   // Right elbow articulation
#define LSHLDRT 4   // Left shoulder articulation with torso
#define LSHLDRA 5   // Left shoulder articulation with arm
#define LELBOW  6   // Left elbow articulation
#define RHIP    7   // Right hip articulation
#define LHIP    8   // Left hip articulation
#define RTHIGH  9   // Right thigh articulation
#define LTHIGH  10  // Left thigh articulation
#define RKNEE   11  // Right knee articulation
#define LKNEE   12  // Left knee articulation
#define RANKLE  13  // Right ankle articulation
#define LANKLE  14  // Left ankle articulation

#define MAX_SERVOS 15

/****************************************** Defino variables ******************************************/
// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// "Minimum" and "Maximum" pulse lengths for each servo. Indexes correspond to constants above
int servoMin[] =  {145, 160,  145,  145,  120,  145,  145,  145,  145,  145,  145,  145,  145,  145,  145 };
int servoMax[] =  {635, 630,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640 };
int anguloMin[] = {25,  65,   5,    70,    5,    15,  80,   60,   85,   5,    75,   50,   85,   70,   35  };
int anguloMax[] = {145, 175,  150,  80,   110,  160,  90,  75,   100,  75,   145,  75,   110,  120,  85  };
int posHome[] =   {85,  75,   5,    70,   85,   160,  90,   75,   85,   45,   110,  75,   85,   95,   60  };
//                 00   01    02    03    04    05    06    07    08    09    10    11    12    13    14

// Last values for each servo/articulation. Needed for "natural movements"
int lastVal[MAX_SERVOS] = {};

//Mods
int funcionPE;
int modo;

//Inputs
int servos[MAX_SERVOS] = {};
int angles[MAX_SERVOS] = {}; 
int posArray;

/****************************************** SETUP ******************************************/
void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(1000); // for parseInt()...
                           //  Serial.println("Go");
  Serial.println("Ingrese :");
  Serial.println("1 SERVO ANGULO");
  Serial.println("2 FUNCION");
  Serial.println("  1 - Saludar");
  Serial.println("  2 - Decir que no");
  Serial.println("  3 - Dar la mano");
  Serial.println("  4 - Caminar");
  Serial.println("  5 - Estabilizar");

  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates
  
  posArray = 0;
  
  estabilizar();
}

/****************************************** LOOP ******************************************/
void loop()
{
  modo = readMonitorSerie();
  if (modo == 1)
  {
    if(posArray != 0 && servos[0]!= -1){
      setAngleParallel();
      Serial.println("Servo: ");
      Serial.println(servos[0]);
      Serial.println("Angulo: ");
      Serial.println(angles[0]);
    }
  }
  else if (modo == 2)
  {
    switch (funcionPE){
      case 1:
        Serial.println("Bhasky saluda");
        saludar();
        break;
      case 2:
        Serial.println("Bhasky dice no");
        no();
        break;
      case 3:
        Serial.println("Bhasky da la mano");
        darLaMano();
        break;
      case 4:
        Serial.println("Bhasky camina");
        caminar();
        break;
      case 5:
        Serial.println("Bhasky se estabiliza");
        estabilizar();
        break;
      case 6:
        Serial.println("Bhasky festeja a lo Pogba");
        dab();
        break;
      case 7:
        Serial.println("Bhasky hace la onda");
        onda();
        break;
      default:
        Serial.println("Bhasky no hace nada");
        break;
    }
  }
}

/****************************************** readMonitorSerie ******************************************/
int readMonitorSerie()
{
  int modoF;
  int servo, angle;
  if (Serial.available())
  {
    modoF = Serial.parseInt();
    if (modoF == 1)
    {
      cleanInputs();
      servo = Serial.parseInt();
      angle = Serial.parseInt();
      if ((servo >= 0) && (servo < MAX_SERVOS)){
        addInput(servo,angle);
      }
    }
    else if (modoF == 2)
    {
      funcionPE = Serial.parseInt();
    }
  }
  return modoF;
}

/****************************************** isAngleValid ******************************************/
boolean isAngleValid(){
  for(int i=0;i<posArray;i++){
     if ((angles[i] < anguloMin[servos[i]]) || (angles[i] > anguloMax[servos[i]])){
       return false;
     }
  }
  return true;
}

/****************************************** setAngleParallel ******************************************/
void setAngleParallel()
{
  int i, pulselen;
  int j=0;
  int cantComplet=0;
  int speed = 2;
  int incV[posArray]={};
  boolean notFinish = true;
  

  if(isAngleValid())
  {   
    for(i=0;i<posArray;i++){
      if (lastVal[servos[i]] > angles[i])
        incV[i] = -1;
      else
        incV[i] = 1; 
      lastVal[servos[i]]+=incV[i];
    }
    while(notFinish){
      if (servos[j] != -1 && j<posArray){
        i = lastVal[servos[j]];
        if ((incV[j] == 1 && i <= angles[j] ) || (incV[j] == -1 && i >= angles[j])){
          pulselen = map(i, 0, 180, servoMin[servos[j]], servoMax[servos[j]]);
          pwm.setPWM(servos[j], 0, pulselen);
          delay(INCRDEL);
          i += incV[j]*speed;
          lastVal[servos[j]] = i;
        } else {
          lastVal[servos[j]] = angles[j];
          if (servos[j] > 0 && servos[j] < 7 )
            limitarBrazos();
          servos[j] = -1;
          cantComplet++;
          if (cantComplet == posArray){
            notFinish = false;
          }
        }
      }else{
        if (j == posArray){
          j=-1;
        }
      }
      j++;
    }
  }
  delay(DELAY_ENTRE_SERVOS);
}

/****************************************** Manejo de Inputs ******************************************/
void cleanInputs(){
  int i;
  for(i=0;i<MAX_SERVOS;i++){
    servos[i]=-1;
    angles[i]=0;
  }
  posArray=0;
}

void addInput(int servo, int angle){
  servos[posArray]=servo;
  angles[posArray]=angle;
  posArray++;
}

/****************************************** Inicializar LastVal ******************************************/
void inicializarLastVal(){
  for(int i=0;i<MAX_SERVOS;i++){
    lastVal[i] = posHome[i] - 1;
  }
  Serial.println("Se Inicializo LastVal");
}

/****************************************** Limitar Brazos ******************************************/
void limitarBrazos(){
  if ( lastVal[RSHLDRT] > 109 || lastVal[RSHLDRA] > 34 ){
    anguloMin[RELBOW] = 5;
    if ( lastVal[RELBOW] < posHome[RELBOW] ){
      if ( lastVal[RSHLDRT] > 109 )
        anguloMin[RSHLDRA] = 5;
      if ( lastVal[RSHLDRA] > 34 )
        anguloMin[RSHLDRT] = 65;
    }
  } else {
    anguloMin[RELBOW] = posHome[RELBOW];
  }
  
  if (lastVal[LSHLDRT] < 46 || lastVal[LSHLDRA] < 131  ){
    anguloMax[LELBOW] = 165;
    if( lastVal[LELBOW] > posHome[LELBOW] ){
      if ( lastVal[LSHLDRT] < 46 )
        anguloMax[LSHLDRA] = 160;
      if ( lastVal[LSHLDRA] < 131 )
        anguloMax[LSHLDRT] = 110; 
    }
  } else {
    anguloMax[LELBOW] = posHome[LELBOW];
  }

  if (lastVal[RELBOW] < posHome[RELBOW]){
    if (lastVal[RSHLDRT] > 109 && lastVal[RSHLDRA] < 36 )
      anguloMin[RSHLDRT] = 110;
    if (lastVal[RSHLDRA] > 34 && lastVal[RSHLDRT] < 111 )
      anguloMin[RSHLDRA] = 35;
  } else {
    anguloMin[RSHLDRT] = 65;
    anguloMin[RSHLDRA] = 5;
  }

  if (lastVal[LELBOW] > posHome[LELBOW]){
    if (lastVal[LSHLDRT] < 46 && lastVal[LSHLDRA] > 129)
      anguloMax[LSHLDRT] = 45;
    if (lastVal[LSHLDRA] < 131 && lastVal[LSHLDRT] > 44)
      anguloMax[LSHLDRA] = 130;
  } else {
    anguloMax[LSHLDRT] = 110;
    anguloMax[LSHLDRA] = 160;
  }
}

/****************************************** Defino las funciones del robot ******************************************/
/****************************************** Saludar ******************************************/
void saludar(){
  cleanInputs();
  addInput(RSHLDRA,35);
  addInput(LSHLDRA,120);
  setAngleParallel();

  cleanInputs();
  addInput(LSHLDRA,65);
  addInput(RELBOW,5);
  setAngleParallel();
  
  cleanInputs();
  addInput(LSHLDRA,25);
  addInput(HEAD,30);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD,130);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD,posHome[HEAD]);
  addInput(LSHLDRA,70);
  setAngleParallel();

  cleanInputs();
  addInput(LSHLDRA,135);
  addInput(RELBOW,posHome[RELBOW]);
  setAngleParallel();

  cleanInputs();
  addInput(LSHLDRA,posHome[LSHLDRA]);
  addInput(RSHLDRA,posHome[RSHLDRA]);
  setAngleParallel();
}

/****************************************** Decir que No ******************************************/
void no()
{
  cleanInputs();
  addInput(RSHLDRA,35);
  addInput(LSHLDRA,130);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW,5);
  addInput(LELBOW,150);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD,85);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD,30);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD,140);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD,posHome[HEAD]);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW,posHome[RELBOW]);
  addInput(LELBOW,posHome[LELBOW]);
  setAngleParallel();
  
  cleanInputs();
  addInput(RSHLDRA,posHome[RSHLDRA]);
  addInput(LSHLDRA,posHome[LSHLDRA]);
  setAngleParallel();
}

/****************************************** Dar La Mano ******************************************/
void darLaMano(){

  cleanInputs();
  addInput(RSHLDRT,150);
  setAngleParallel();

  cleanInputs();
  addInput(RSHLDRT,130);
  setAngleParallel();    


  cleanInputs();
  addInput(RSHLDRT,150);
  setAngleParallel();

  cleanInputs();
  addInput(RSHLDRT,130);
  setAngleParallel(); 
    
  cleanInputs();
  addInput(RSHLDRT,150);
  setAngleParallel();

  cleanInputs();
  addInput(RSHLDRT,posHome[RSHLDRT]);
  setAngleParallel(); 
}
/****************************************** Caminar ******************************************/
void caminar(){
  //Salir de posicion estatica
  cleanInputs();
  addInput(RTHIGH,30);
  addInput(RANKLE,80);
  setAngleParallel();

  for(int i=0;i<3;i++){
    //Paso Izq
    cleanInputs();
    addInput(LTHIGH,130);
    addInput(LANKLE,80);
    addInput(RANKLE,posHome[RANKLE]);
    addInput(RTHIGH,posHome[RTHIGH]);
    addInput(RSHLDRT,85);
    addInput(LSHLDRT,95);
    setAngleParallel();
  
    //Paso Der
    cleanInputs();
    addInput(RTHIGH,25);
    addInput(RANKLE,75);
    addInput(LTHIGH,posHome[LTHIGH]);
    addInput(LANKLE,posHome[LANKLE]);
    addInput(RSHLDRT,65);
    addInput(LSHLDRT,75);
    setAngleParallel();
  }
  
  //Estabilizar
  cleanInputs();
  addInput(RANKLE,posHome[RANKLE]);
  addInput(RTHIGH,posHome[RTHIGH]);
  addInput(LTHIGH,posHome[LTHIGH]);
  addInput(LANKLE,posHome[LANKLE]);
  setAngleParallel();
}

/****************************************** Estabilizar ******************************************/
void estabilizar()
{
  if (lastVal[0] == 0)
    inicializarLastVal();

  cleanInputs();
  addInput(HEAD,posHome[HEAD]);
  setAngleParallel(); 

  cleanInputs();
  addInput(RELBOW,posHome[RELBOW]);
  addInput(LELBOW,posHome[LELBOW]);
  setAngleParallel(); 

  cleanInputs();
  addInput(RSHLDRA,posHome[RSHLDRA]);
  addInput(LSHLDRA,posHome[LSHLDRA]);
  setAngleParallel(); 

  cleanInputs();
  addInput(RSHLDRT,posHome[RSHLDRT]);
  addInput(LSHLDRT,posHome[LSHLDRT]);
  setAngleParallel(); 

  cleanInputs();
  addInput(RHIP,posHome[RHIP]);
  addInput(LHIP,posHome[LHIP]);
  setAngleParallel();

  cleanInputs();
  addInput(RTHIGH,posHome[RTHIGH]);
  addInput(LTHIGH,posHome[LTHIGH]);
  setAngleParallel();

  cleanInputs();
  addInput(RKNEE,posHome[RKNEE]);
  addInput(LKNEE,posHome[LKNEE]);
  setAngleParallel();

  cleanInputs();
  addInput(RANKLE,posHome[RANKLE]);
  addInput(LANKLE,posHome[LANKLE]);
  setAngleParallel(); 
}

/****************************************** Dab ******************************************/
void dab(){
  
  cleanInputs();
  addInput(RSHLDRT, 175);
  addInput(LSHLDRA, 80);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW, 40);
  addInput(LSHLDRA, 35);
  setAngleParallel();

  cleanInputs();
  addInput(HEAD, 40);
  addInput(RTHIGH, 30);
  addInput(RELBOW, 5);
  setAngleParallel();

  delay(1000);

  cleanInputs();
  addInput(RTHIGH, posHome[RTHIGH]);
  addInput(RELBOW, 20);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW, posHome[RELBOW]);
  addInput(LSHLDRA, 80);
  addInput(HEAD, posHome[HEAD]);
  setAngleParallel();

  cleanInputs();
  addInput(RSHLDRT, posHome[RSHLDRT]);
  addInput(LSHLDRA, posHome[LSHLDRA]);
  setAngleParallel();  

}

/****************************************** Onda ******************************************/
void onda(){
  
  cleanInputs();
  addInput(RSHLDRA, 90);
  addInput(RSHLDRT, 65);
  addInput(LSHLDRA, 75);
  addInput(LSHLDRT, 90);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW, 35);
  addInput(RSHLDRA, 110);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW, 70);
  addInput(RSHLDRA, 75);
  addInput(LSHLDRA, 50);
  setAngleParallel();

  cleanInputs();
  addInput(LELBOW, 90);
  addInput(RSHLDRA, 90);
  addInput(LSHLDRA, 70);
  setAngleParallel();

  cleanInputs();
  addInput(LELBOW, 125);
  addInput(LSHLDRA, 60);
  setAngleParallel();

  cleanInputs();
  addInput(LELBOW, 90);
  addInput(LSHLDRA, 75);
  setAngleParallel();

  cleanInputs();
  addInput(LELBOW, 125);
  addInput(LSHLDRA, 60);
  setAngleParallel();
  
  cleanInputs();
  addInput(LELBOW, 90);
  addInput(LSHLDRA, 90);
  addInput(RSHLDRA, 105);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW, 65);
  addInput(LSHLDRA, 75);
  addInput(RSHLDRA, 80);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW, 70);
  addInput(RSHLDRA, 90);
  setAngleParallel();

  cleanInputs();
  addInput(RELBOW,posHome[RELBOW]);
  addInput(LELBOW,posHome[LELBOW]);
  setAngleParallel(); 

  cleanInputs();
  addInput(RSHLDRA,posHome[RSHLDRA]);
  addInput(LSHLDRA,posHome[LSHLDRA]);
  setAngleParallel(); 

  cleanInputs();
  addInput(RSHLDRT,posHome[RSHLDRT]);
  addInput(LSHLDRT,posHome[LSHLDRT]);
  setAngleParallel(); 


}
