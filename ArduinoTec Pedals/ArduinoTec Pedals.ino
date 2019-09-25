#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
  false, false, false, true, false, false,
  false, true, false, true, false);

const int MaxRange = 1023;
const int MinRange = 0;

const int AxisMax = 1024;

int blBrk=0;
int blThr=0;
int blCth=0;
int maxBrk=0;//max value for Brake
int maxThr=0; //max value for Throttle
int maxCth=0; //max value for Clutch
bool aveBrkSet =false;
bool aveThrSet =false;
bool aveCthSet =false;
bool debug = true;

void setup() {
  delay(2000); //set a delay for everything to settle

  //setting ranges
  Joystick.setRxAxisRange(MinRange,MaxRange);
  Joystick.setThrottleRange(MinRange, MaxRange);
  Joystick.setBrakeRange(MinRange, MaxRange);

  Joystick.begin(true);

  // Setting Pin Modes
  pinMode(A2, INPUT); //Clutch
  pinMode(A1, INPUT); //Brake
  pinMode(A0, INPUT); //Throttle
  pinMode(A3, INPUT); //BrakeResistance;
  pinMode(3, OUTPUT); // Vibration Motor Control this is paired with a transistor to control the motor 
  pinMode(13, OUTPUT); // LED output
  digitalWrite(3,HIGH); // set the pin as High, Low will result in the motor spinning
  
  // get the baseline registering values
  blBrk = get_baseline(A1,25)+10;//adding a small amount of initial deadzone after normalising the value
  blThr= get_baseline(A0,25)+5;//adding a small amount of initial deadzone after normalising the value
  blCth= get_baseline(A2,25)+5;//adding a small amount of initial deadzone after normalising the value}
}

void loop() {
  //reading the relevant values from the pedals
  int valBrk = analogRead(A1); 
  int valRestBrk = analogRead(A3); 
  int valThr = analogRead(A2); 
  int valCth = analogRead(A0); 
  double pers = get_Persentage(A3); //get the resistance persentage to apply against the brake pedal which then halved
  
  int actThrVal = valCth-blCth;
  int actCthVal = valThr-blThr;
  int actBrkVal = (valBrk-blBrk) ; //normalised brake value
  int PersBrkVal = int(((valBrk-blBrk) * pers)) ; //normalised brake value with the resistance added
  
  if (maxCth < actCthVal) maxCth = actCthVal; // set/update the max value based on pedal progressions
  if (maxThr < actThrVal) maxThr = actThrVal; // set/update the max value based on pedal progressions
  if (maxBrk < actBrkVal) maxBrk = actBrkVal; // set/update the max value based on pedal progressions
    
  if (debug) {
  
    String values = "Brake Values: " + String(PersBrkVal)  + ", max value: " + String(maxBrk) + ", Actual With Resistance:" + String(actBrkVal) + ", Pers:" + String(pers) + ", Resistance: " + String(valRestBrk);;  //+ ", Brake Values: " + String(valBrk) + ", Baseline: " + String(blBrk) + ", MaxVal: " + String(maxBrk);// + ", 
  //  String values1 = "Throttle Values: " + String(actThrVal) + ", max value: " + String(maxThr);
  //  String values2 = "Clutch Values: " + String(actCthVal) + ", Baseline: " + String(maxCth);

    Serial.println(values);
 //   Serial.println(values1);
//    Serial.println(values2);
  //  delay(500);
  }
  
  if (actBrkVal > (maxBrk*0.80)) digitalWrite(3,LOW);  //enable the motor to spin when the pedal reaches 80% of the pedal pressure
  else digitalWrite(3,HIGH); //stop the motor when it falls below 85%

  //Limit upperbound Noise
  if (actCthVal > (maxCth-10)) actCthVal = (maxCth-10);
  if (actThrVal > (maxThr-15)) actThrVal = (maxThr-10);
  if (PersBrkVal > (maxBrk-20)) actBrkVal = (maxBrk-20);
  
  //set the values
  Joystick.setRxAxis(actCthVal);
  Joystick.setThrottle(actThrVal);
  Joystick.setBrake(PersBrkVal);
}

  /*
  // System Disabled
  if (digitalRead(A0) != 0)
  {
    // Turn indicator light off.
    digitalWrite(13, 0);
    return;
  }

  // Turn indicator light on.
  digitalWrite(13, 1);
  */
//}

int get_baseline(int pin, int count){ //get the pedal baseline
  int maxVal = 0;
  int newVal = 0;
  for (int i = 0; i < count; i++){
    newVal = analogRead(pin);
    if (maxVal < newVal) maxVal = newVal; 
    }
  return maxVal;
}

 double get_Persentage(int pinToRead){
  return (1-(double(analogRead(pinToRead)) / AxisMax)); //1024 is the maximum  
}
