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

#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define INCR 5     // "angle-step", from current angle to the destination angle
#define INCRDEL 20 // "time-step", ms delay for each angle-step

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

#define NUMOFANGLES 15

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// "Minimum" and "Maximum" pulse lengths for each servo. Indexes correspond to constants above
int servomin[] =  {145, 160,  145,  145,  120,  145,  145,  145,  145,  145,  145,  145,  145,  145,  145 };
int servomax[] =  {635, 630,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640,  640 };
int anguloMin[] = {25,  65,   5,    5,    5,    15,   90,   60,   85,   5,    75,   50,   85,   70,   35  };
int anguloMax[] = {145, 175,  150,  80,   110,  160,  165,  75,   100,  75,   145,  75,   110,  120,  85  };
int posHome[] =   {85,  75,   5,    70,   85,   160,  90,   75,   85,   45,   110,  75,   85,   95,   60  }
//                 00   01    02    03    04    05    06    07    08    09    10    11    12    13    14


// Last values for each servo/articulation. Needed for "natural movements"
int lastVal[NUMOFANGLES] = {};

// Values read from Serial to set servo angles...
int i, readVal[NUMOFANGLES] = {};

//DECLARADOS POR MARTIN
int funcionPE;
int modo;

/***********************************************************************/
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
  Serial.println("  3 - Estabilizar");

  pwm.begin();
  pwm.setPWMFreq(60); // Analog servos run at ~60 Hz updates

  estabilizar();
  

}

/***********************************************************************/
void loop()
{
  //readnumbers();
  modo = readMonitorSerie();
  if (modo == 1)
  {
    for (i = 0; i < NUMOFANGLES; i++)
    {
      if (readVal[i] != 0)
      {
        setAngle(i, readVal[i]);
        Serial.println("Servo: ");
        Serial.println(i);
        Serial.println("Angulo: ");
        Serial.println(readVal[i]);
      }
    }
  }
  else if (modo == 2)
  {
    if ( funcionPE == 1)
    {
      Serial.println("Bhasky saluda");
      saludar();
    }
    if ( funcionPE == 2)
    {
      Serial.println("Bhasky dice no");
      no();
    }
    if ( funcionPE == 3)
    {
      Serial.println("Bhasky se estabiliza");
      estabilizar();
    }
  }
}

/***********************************************************************/
void readnumbers()
{
  int servo, angle;

  for (i = 0; i < NUMOFANGLES; i++)
    readVal[i] = 0;

  if (Serial.available())
  {
    servo = Serial.parseInt() - 1;
    angle = Serial.parseInt();

    //    Serial.print("Read data: ");
    //    Serial.print(servo);
    //    Serial.print(" ");
    //    Serial.println(angle);

    if ((servo >= 0) && (servo < NUMOFANGLES))
      readVal[servo] = angle;
  }
}
/***********************************************************************/

/***********************************************************************/
void setAngle(int servonum, int angle)
{
  int i, pulselen;
  if ((angle >= 5) && (angle <= 175))
  {
    int initV, endV, incV;
    if (lastVal[servonum] > angle)
      incV = -1;
    else
      incV = 1;
    i = lastVal[servonum];
    do
    {
      pulselen = map(i, 0, 180, servomin[servonum], servomax[servonum]);
      pwm.setPWM(servonum, 0, pulselen);
      delay(INCRDEL);
      if (i != angle)
        i += incV;
    } while (i != angle);
    lastVal[servonum] = angle;
  }
}

int readMonitorSerie()
{
  int modoF;
  int servo, angle;
  if (Serial.available())
  {
    modoF = Serial.parseInt();
    if (modoF == 1)
    {
      for (i = 0; i < NUMOFANGLES; i++)
        readVal[i] = 0;
        
        servo = Serial.parseInt() - 1;
        angle = Serial.parseInt();
  
        if ((servo >= 0) && (servo < NUMOFANGLES))
          readVal[servo] = angle;
    }
    else if (modoF == 2)
    {
      funcionPE = Serial.parseInt();
    }
  }
  return modoF;
}

void saludar()
{
  readVal[RSHLDRA] = 30;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(500);
  
  readVal[RELBOW] = 5;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(500);

  readVal[LSHLDRA] = 25;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(500);

  readVal[HEAD] = 30;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);
    
  readVal[HEAD] = 130;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);
    
  readVal[HEAD] = 90;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);
    
  readVal[LSHLDRA] = 160;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(500);
    
  readVal[RELBOW] = 65;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(500);
 
  readVal[RSHLDRA] = 5;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(500);
}

void no()
{
  readVal[RSHLDRA] = 30;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(500);
  
  readVal[RELBOW] = 5;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(500);

  readVal[LSHLDRA] = 135;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(500);
  
  readVal[LELBOW] = 150;
  setAngle(LELBOW, readVal[LELBOW]);
  delay(500);
  
  readVal[HEAD] = 90;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);

  readVal[HEAD] = 30;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);

  readVal[HEAD] = 140;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);

  readVal[HEAD] = 90;
  setAngle(HEAD, readVal[HEAD]);
  delay(500);

  readVal[RELBOW] = 65;
  setAngle(RELBOW, readVal[RELBOW]);
  delay(500);
 
  readVal[RSHLDRA] = 5;
  setAngle(RSHLDRA, readVal[RSHLDRA]);
  delay(500);

  readVal[LELBOW] = 90;
  setAngle(LELBOW, readVal[LELBOW]);
  delay(500);
  
  readVal[LSHLDRA] = 160;
  setAngle(LSHLDRA, readVal[LSHLDRA]);
  delay(500);
 
}

void estabilizar()
{
  // Head to front
  lastVal[HEAD] = 85;
  setAngle(HEAD, lastVal[HEAD]);
  delay(500);

  // Right arm parallel to torso - elbow 75
  lastVal[RELBOW] = 75;
  setAngle(RELBOW, lastVal[RELBOW]);
  delay(500);

  // Right arm parallel to torso - shoulder/arm 5
  lastVal[RSHLDRA] = 5;
  setAngle(RSHLDRA, lastVal[RSHLDRA]);
  delay(500);

  // Right arm parallel to torso - shoulder/torso 70
  lastVal[RSHLDRT] = 70;
  setAngle(RSHLDRT, lastVal[RSHLDRT]);
  delay(500);

  // Left arm parallel to torso - shouder/arm 85
  lastVal[LELBOW] = 85;
  setAngle(LELBOW, lastVal[LELBOW]);
  delay(500);

  // Left arm parallel to torso - shouder/arm 160
  lastVal[LSHLDRA] = 160;
  setAngle(LSHLDRA, lastVal[LSHLDRA]);
  delay(500);

  // Left arm parallel to torso - shouder/torso 90
  lastVal[LSHLDRT] = 90;
  setAngle(LSHLDRT, lastVal[LSHLDRT]);
  delay(500);

  // Right leg - standing position 75
  lastVal[RHIP] = 75;
  setAngle(RHIP, lastVal[RHIP]);
  delay(500);

  // Left leg - standing position 85
  lastVal[LHIP] = 85;
  setAngle(LHIP, lastVal[LHIP]);
  delay(500);

  // Right leg - standing position 45
  lastVal[RTHIGH] = 45;
  setAngle(RTHIGH, lastVal[RTHIGH]);
  delay(500);

  // Left leg - standing position 110
  lastVal[LTHIGH] = 110;
  setAngle(LTHIGH, lastVal[LTHIGH]);
  delay(500);

  // Right leg - standing position 75
  lastVal[RKNEE] = 75;
  setAngle(RKNEE, lastVal[RKNEE]);
  delay(500);

  // Left leg - standing position 85
  lastVal[LKNEE] = 85;
  setAngle(LKNEE, lastVal[LKNEE]);
  delay(500);

  // Right leg - standing position 95
  lastVal[RANKLE] = 95;
  setAngle(RANKLE, lastVal[RANKLE]);
  delay(500);

  // Left leg - standing position 95
  lastVal[LANKLE] = 60;
  setAngle(LANKLE, lastVal[LANKLE]);
  delay(500);
}
