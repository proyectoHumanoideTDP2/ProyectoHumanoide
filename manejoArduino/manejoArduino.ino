/*************************************************** 
01) Moving the Robot Head.
  
02) Handle angles instead of "setPWM", i.e. wrap setPWM with angle function.
  Besides, movements are made in small angle increments and delays, in order
  to show some "natural/non-digital" behavior...
  Head = Servo 0...

03) Adding the right arm movement starting in v03 
   Take advantage of previous PWM to Angle experiments.
   Right Shoulder = Servo 1
   
04) Adding the right arm+ movement starting in v04 - shoulder to arm 
   Take advantage of previous PWM to Angle experiments.
   Right Shoulder "down" to arm articulation = Servo 2

05) Adding the right arm++ movement starting in v05 - elbow 
   Take advantage of previous PWM to Angle experiments.
   Right elbow articulation = Servo 3

06) Adding the left arm movement  - shoulder to torso 
   Take advantage of previous PWM to Angle experiments.
   Left shoulder to torso articulation = Servo 4

07) Adding the left arm+ movement - shoulder to arm 
   Take advantage of previous PWM to Angle experiments.
   Left Shoulder "down" to arm articulation = Servo 5

 08) Adding the left arm+ movement - elbow 
   Take advantage of previous PWM to Angle experiments.
   Left Shoulder "down" to arm articulation = Servo 5

09) Just moving arm...

10) Head & Arms to the "initial" state

11) Right hip

12) Left hip

13) Right thigh 

14) Left thigh 

15) Right knee

16) Left knee

17) Right ankle

18) Reset right and left thigs for "more vertical" legs standing position

19) Left ankle

20) Like v09, moving the left arm, but now with a controlled "standing 
    position". There are a few changes in the "standing position", 
    maybe due to provide 7.4v power through an adapted PC power source

21) Receive commands from serial...

22) Read by pairs servo-angle

  Adapted fom Adafruit 16-channel PWM & Servo driver example
  
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.
 ****************************************************/

/****************************************** Incluyo las librerias ******************************************/
#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

/****************************************** Declaro e inicializo el PWM ******************************************/
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/****************************************** Defino constantes ******************************************/
#define INCR 5     // "angle-step", from current angle to the destination angle
#define INCRDEL 20 // "time-step", ms delay for each angle-step
#define DELAY_ENTRE_SERVOS 250 

#define HEAD 0
#define RSHLDRT 1 // Right shoulder articulation with torso
#define RSHLDRA 2 // Right shoulder articulation with arm
#define RELBOW 3  // Right elbow articulation
#define LSHLDRT 4 // Left shoulder articulation with torso
#define LSHLDRA 5 // Left shoulder articulation with arm
#define LELBOW 6  // Left elbow articulation
#define RHIP 7    // Right hip articulation
#define LHIP 8    // Left hip articulation
#define RTHIGH 9  // Right thigh articulation
#define LTHIGH 10 // Left thigh articulation
#define RKNEE 11  // Right knee articulation
#define LKNEE 12  // Left knee articulation
#define RANKLE 13 // Right ankle articulation
#define LANKLE 14 // Left ankle articulation

#define MAX_SERVOS 15

/****************************************** Defino variables ******************************************/
// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// "Minimum" and "Maximum" pulse lengths for each servo. Indexes correspond to constants above
int servoMin[] =  {145, 160,  145,  145,  120,  145,  145,  145,  145,  145,  145,  145,  145,  145,  145 };
int servoMax[] =  {635, 630,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640 };
int anguloMin[] = {25,  65,   5,    5,    5,    15,   90,   60,   85,   5,    75,   50,   85,   70,   35  };
int anguloMax[] = {145, 175,  150,  80,   110,  160,  165,  75,   100,  75,   145,  75,   110,  120,  85  };
int posHome[] =   {85,  75,   5,    70,   85,   160,  90,   75,   85,   45,   110,  75,   85,   95,   60  };
//                 00   01    02    03    04    05    06    07    08    09    10    11    12    13    14


// Last values for each servo/articulation. Needed for "natural movements"
int lastVal[MAX_SERVOS] = {};

// Values read from Serial to set servo angles...
int i, readVal[MAX_SERVOS] = {};

//Mods
int funcionPE;
int modo;

//Inputs
int servos[MAX_SERVOS]={};
int angles[MAX_SERVOS]={}; 
int posArray=0;

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
  Serial.println("  4 - Estabilizar");

  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates

  estabilizar();
  

}

/****************************************** LOOP ******************************************/
void loop()
{
  //readnumbers();
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
    if ( funcionPE == 1)
    {
      Serial.println("Bhasky saluda");
      saludarP();
    }
    if ( funcionPE == 2)
    {
      Serial.println("Bhasky dice no");
      noP();
    }
    if ( funcionPE == 3)
    {
      Serial.println("Bhasky da la mano");
      darLaManoP();
    }
    if ( funcionPE == 4)
    {
      Serial.println("Bhasky camina");
      caminar();
    }
    if ( funcionPE == 5)
    {
      Serial.println("Bhasky se estabiliza");
      estabilizar();
    }
  }
}

/****************************************** setAngle ******************************************/
void setAngle(int servonum, int angle)
{
  int i, pulselen;
  int speed = 2;
  if ((angle >= anguloMin[servonum]) && (angle <= anguloMax[servonum]))
  {
    int initV, endV, incV;
    if (lastVal[servonum] > angle)
      incV = -1;
    else
      incV = 1;
    i = lastVal[servonum];
    do
    {
      pulselen = map(i, 0, 180, servoMin[servonum], servoMax[servonum]);
      pwm.setPWM(servonum, 0, pulselen);
      delay(INCRDEL);
      if (i != angle)
        i += incV*speed;
    } while ((incV == 1 && i < angle ) || ((incV == -1 && i > angle )));
    lastVal[servonum] = angle;
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

/****************************************** Defino las funciones del robot ******************************************/
/****************************************** saludar ******************************************/
void saludar()
{
  readVal[RSHLDRA] = 35;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);
  
  readVal[RELBOW] = 5;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[LSHLDRA] = 25;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[HEAD] = 30;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);
    
  readVal[HEAD] = 130;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);
    
  readVal[HEAD] = 85;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);
    
  readVal[LSHLDRA] = 160;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);
    
  readVal[RELBOW] = 65;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(DELAY_ENTRE_SERVOS);
 
  readVal[RSHLDRA] = 5;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);
}

void saludarP(){
  cleanInputs();
  addInput(RSHLDRA,35);
  addInput(LSHLDRA,130);
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
/****************************************** no ******************************************/
void no()
{
  readVal[RSHLDRA] = 35;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);
  
  readVal[RELBOW] = 5;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[LSHLDRA] = 130;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);
  
  readVal[LELBOW] = 150;
  setAngle(LELBOW, readVal[LELBOW]);
  delay(DELAY_ENTRE_SERVOS);
  
  readVal[HEAD] = 85;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[HEAD] = 30;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[HEAD] = 140;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[HEAD] = 85;
  setAngle(HEAD, readVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[RELBOW] = 65;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(DELAY_ENTRE_SERVOS);
 
  readVal[RSHLDRA] = 5;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[LELBOW] = 90;
  setAngle(LELBOW, readVal[LELBOW]);
  delay(DELAY_ENTRE_SERVOS);
  
  readVal[LSHLDRA] = 160;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);
 
}

void noP()
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

/****************************************** darLaMano ******************************************/
void darLaMano(){
  
  readVal[RSHLDRT] = 150;
  setAngle(RSHLDRT, readVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[RSHLDRT] = 130;
  setAngle(RSHLDRT, readVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[RSHLDRT] = 150;
  setAngle(RSHLDRT, readVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[RSHLDRT] = 130;
  setAngle(RSHLDRT, readVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[RSHLDRT] = 150;
  setAngle(RSHLDRT, readVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  readVal[RSHLDRT] = 75;
  setAngle(RSHLDRT, readVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);
  
}

void darLaManoP(){

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
/****************************************** caminar ******************************************/
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


/****************************************** estabilizar ******************************************/
void estabilizar()
{
  // Head to front
  lastVal[HEAD] = posHome[HEAD];
  setAngle(HEAD, lastVal[HEAD]);
  delay(DELAY_ENTRE_SERVOS);

  // Right arm parallel to torso - elbow 75
  lastVal[RELBOW] = posHome[RELBOW];
  setAngle(RELBOW, lastVal[RELBOW]);
  delay(DELAY_ENTRE_SERVOS);

  // Right arm parallel to torso - shoulder/arm 5
  lastVal[RSHLDRA] =  posHome[RSHLDRA];
  setAngle(RSHLDRA, lastVal[RSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);

  // Right arm parallel to torso - shoulder/torso 70
  lastVal[RSHLDRT] =  posHome[RSHLDRT];
  setAngle(RSHLDRT, lastVal[RSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  // Left arm parallel to torso - shouder/arm 85
  lastVal[LELBOW] =  posHome[LELBOW];
  setAngle(LELBOW, lastVal[LELBOW]);
  delay(DELAY_ENTRE_SERVOS);

  // Left arm parallel to torso - shouder/arm 160
  lastVal[LSHLDRA] =  posHome[LSHLDRA];
  setAngle(LSHLDRA, lastVal[LSHLDRA]);
  delay(DELAY_ENTRE_SERVOS);

  // Left arm parallel to torso - shouder/torso 90
  lastVal[LSHLDRT] =  posHome[LSHLDRT];
  setAngle(LSHLDRT, lastVal[LSHLDRT]);
  delay(DELAY_ENTRE_SERVOS);

  // Right leg - standing position 75
  lastVal[RHIP] =  posHome[RHIP];
  setAngle(RHIP, lastVal[RHIP]);
  delay(DELAY_ENTRE_SERVOS);

  // Left leg - standing position 85
  lastVal[LHIP] =  posHome[LHIP];
  setAngle(LHIP, lastVal[LHIP]);
  delay(DELAY_ENTRE_SERVOS);

  // Right leg - standing position 45
  lastVal[RTHIGH] =  posHome[RTHIGH];
  setAngle(RTHIGH, lastVal[RTHIGH]);
  delay(DELAY_ENTRE_SERVOS);

  // Left leg - standing position 110
  lastVal[LTHIGH] =  posHome[LTHIGH];
  setAngle(LTHIGH, lastVal[LTHIGH]);
  delay(DELAY_ENTRE_SERVOS);

  // Right leg - standing position 75
  lastVal[RKNEE] =  posHome[RKNEE];
  setAngle(RKNEE, lastVal[RKNEE]);
  delay(DELAY_ENTRE_SERVOS);

  // Left leg - standing position 85
  lastVal[LKNEE] =  posHome[LKNEE];
  setAngle(LKNEE, lastVal[LKNEE]);
  delay(DELAY_ENTRE_SERVOS);

  // Right leg - standing position 95
  lastVal[RANKLE] =  posHome[RANKLE];
  setAngle(RANKLE, lastVal[RANKLE]);
  delay(DELAY_ENTRE_SERVOS);

  // Left leg - standing position 95
  lastVal[LANKLE] =  posHome[LANKLE];
  setAngle(LANKLE, lastVal[LANKLE]);
  delay(DELAY_ENTRE_SERVOS);
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
  boolean notFinish = true;
  int incV[posArray]={};

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
