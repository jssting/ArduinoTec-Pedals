#include <Joystick.h>
#include "confOptions.h"
#define Throttle          A0
#define Brake             A1
#define Clutch            A2

#define Throttle_I2       A5 //second input line (brown wire)
#define Brake_I2          A6 //second input line (brown wire)
#define Clutch_I2         A8 //second input line (brown wire)

#define BrakeResistance   A3
#define VibrationMotor    3

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
                   false, false, false, true, false, false,
                   false, true, false, true, false);

const int MaxRange = 1023;
const int MinRange = 0;

const int AxisMax = 1024;

int blBrk = 0;
int blThr = 0;
int blCth = 0;
int maxBrk = 0; //max value for Brake
int maxThr = 0; //max value for Throttle
int maxCth = 0; //max value for Clutch
bool aveBrkSet = false;
bool aveThrSet = false;
bool aveCthSet = false;

bool debug = false;
bool dbg_PrintThr = false;
bool dbg_PrintBrk = false;
bool dbg_PrintCTH = false;

void setup() {
  delay(2000); //set a delay for everything to settle
      
  debug = Enable_Debug;
  dbg_PrintThr = Debug_Thr;
  dbg_PrintBrk = Debug_Brk;
  dbg_PrintCTH = Debug_Cth;

  //setting ranges
  Joystick.setRxAxisRange(MinRange, MaxRange);
  Joystick.setThrottleRange(MinRange, MaxRange);
  Joystick.setBrakeRange(MinRange, MaxRange);

  Joystick.begin(true);

  // Setting Pin Modes
  pinMode(Throttle, INPUT); //Throttle  
  pinMode(Brake, INPUT); //Brake
  pinMode(Clutch, INPUT); //Clutch
  
  //Setting optional pins states
  if (use_Dual_Thr) pinMode(Throttle_I2, INPUT); //Throttle input 2
  if (use_Dual_Cl) pinMode(Clutch_I2, INPUT); //Clutch input 2
  if (use_Dual_Brk) pinMode(Brake_I2, INPUT); //Brake input 2
  
  pinMode(BrakeResistance, INPUT); //BrakeResistance;
  pinMode(VibrationMotor, OUTPUT); // Vibration Motor Control this is paired with a transistor to control the motor
  pinMode(13, OUTPUT); // LED output
  digitalWrite(VibrationMotor, HIGH); // set the pin as High, Low will result in the motor spinning

  // get the baseline registering values
  blBrk = get_baseline(Brake, 25);
  blThr = get_baseline(Throttle, 25);
  blCth = get_baseline(Clutch, 25);
}

void loop() {
  //reading the relevant values from the pedals
  int valThr = 0;
  int valBrk = 0;
  int valCth = 0;
  
  if (use_Dual_Thr) valThr = abs((analogRead(Throttle)+analogRead(Throttle_I2))/2);
  else valThr = analogRead(Throttle);
  
  if (use_Dual_Cl) valCth = abs((analogRead(Clutch)+analogRead(Clutch_I2))/2);
  else valCth = analogRead(Clutch);
  
  if (use_Dual_Brk) valBrk = abs((analogRead(Brake)+analogRead(Brake_I2))/2);
  else valBrk = analogRead(Brake);
  
  /* Original read statements
  int valThr = analogRead(Throttle);
  int valBrk = analogRead(Brake);
  int valCth = analogRead(Clutch); */
  
  int valRestBrk = analogRead(BrakeResistance);
  double pers = get_Persentage(BrakeResistance); //get the resistance persentage to apply against the brake pedal which then halved

  //All values below are normalised and converted to absolute to ensure a positive value
  int actCthVal = abs(valCth - blCth);//normalised clutch value 
  int actThrVal = abs(valThr - blThr);//normalised Throttle value
  int actBrkVal = abs(valBrk - blBrk);//normalised brake value
  int PersBrkVal = int(((valBrk - blBrk) * pers)) ; //normalised brake value with the resistance added

  // set/update the max value based on pedal progressions
  if (maxCth < actCthVal) maxCth = actCthVal;
  if (maxThr < actThrVal) maxThr = actThrVal;  
  if (maxBrk < actBrkVal) maxBrk = actBrkVal;

  if (debug) {
    if(dbg_PrintThr){      
      String values = "Throttle - Baseline: " + String(blThr) + ", Actual Value: " + String(valThr) + ", Normalised Value: " + String(actThrVal) + ", max value: " + String(maxThr);
      Serial.println(values);
      delay(100);}
 
    if(dbg_PrintBrk){
       String values = "Brakes - Baseline: " + String(blBrk) + ", Actual Value: " + String(valBrk) + ", Normalised Value: " + String(actBrkVal) + ", Normalised With Resistance:" + String(PersBrkVal) + ", Pers:" + String(pers) + ", Resistance: " + String(valRestBrk) + ", max value: " + String(maxBrk);
       Serial.println(values);
       delay(100);}
    
    if(dbg_PrintCTH){
      String values = "Clutch - Baseline: " + String(blCth) + ", Actual Value: " + String(valCth) + ", Normalised Value: " + String(actCthVal) + ", Max Value: " + String(maxCth);                      
      Serial.println(values);
      delay(100);}  
    Serial.println("");//create an empty line   
  }

  if (actBrkVal > (maxBrk * 0.80)) digitalWrite(VibrationMotor, LOW); //enable the motor to spin when the pedal reaches 80% of the pedal pressure
  else digitalWrite(VibrationMotor, HIGH); //stop the motor when it falls below 80%

  //Limit upperbound Noise
  if (actCthVal  > (maxCth - clutch_U_DZ)  ) {actCthVal  = (maxCth - clutch_U_DZ);}
  if (actThrVal  > (maxThr - throttle_U_DZ)) {actThrVal  = (maxThr - throttle_U_DZ);}
  if (PersBrkVal > (maxBrk - brake_U_DZ)   ) {PersBrkVal = (maxBrk - brake_U_DZ);}
 
  //set the values applying base deadzone
  if (valCth >0) 
  {
    if (actCthVal > clutch_L_DZ) Joystick.setRxAxis(actCthVal);
    else Joystick.setRxAxis(0);
  }
  if (valThr >0) 
  {
    if (actThrVal > throttle_L_DZ) Joystick.setThrottle(actThrVal);
    else Joystick.setThrottle(0);
  }
  if (valBrk >0) 
  {
    if (actBrkVal > brake_L_DZ) Joystick.setBrake(PersBrkVal);
    else Joystick.setBrake(0);
  }
}

int get_baseline(int pin, int count) { //get the pedal baseline
  int maxVal = 0;
  int newVal = 0;
  for (int i = 0; i < count; i++) {
    newVal = analogRead(pin);
    if (maxVal < newVal) maxVal = newVal;
  }
  return maxVal;
}

double get_Persentage(int pinToRead) {
  return (1 - (double(analogRead(pinToRead)) / AxisMax)); //1024 is the maximum
}
