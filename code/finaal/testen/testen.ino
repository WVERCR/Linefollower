#include "Arduino.h"
#include "SerialCommand.h"
#include "EEPROMAnything.h"
//#include <SoftwareSerial.h>

#define SerialPort Serial1
#define Baudrate 9600

#define MotorRightForward 5
#define MotorRightBackward 6
#define MotorLeftForward 11
#define MotorLeftBackward 3
#define buttonInterrupt 2
#define ledRun 13

SerialCommand sCmd(SerialPort);     
 
bool debug;
bool run ;
bool stop;
unsigned long previous, calculationTime;

const int sensor[] = {A5, A4, A3, A2, A1, A0};

struct param_t
{
  unsigned long cycleTime;
  int black[6];
  int white[6];
  int power;
  float diff;
  float kp;
  float ki;
  float kd;
  float iTerm;
} params;

int normalised[6];
float debugPosition;
volatile bool buttonPressed = false;
unsigned long laatsteInterrupt = 0; 

void onUnknownCommand(char*command);
void onSet();
void onDebug();
void onCalibrate();
void onRunInterrupt();
void onStop();

void setup()
{
 SerialPort.begin(Baudrate);
 Serial.begin(9600); 

 sCmd.addCommand("set", onSet);
 sCmd.addCommand("debug", onDebug);
 sCmd.addCommand("calibrate", onCalibrate);
 sCmd.addCommand("run", onRun);
 sCmd.addCommand("stop", onStop);
 sCmd.setDefaultHandler(onUnknownCommand);

 EEPROM_readAnything(0, params);

  pinMode(ledRun, OUTPUT);
  pinMode(buttonInterrupt, INPUT_PULLUP);  //
  attachInterrupt(digitalPinToInterrupt(buttonInterrupt), onRun, FALLING);

  SerialPort.println("ready");

  pinMode(MotorLeftForward,OUTPUT);  // 
  pinMode(MotorLeftBackward,OUTPUT);  //
  pinMode(MotorRightForward,OUTPUT);  //
  pinMode(MotorRightBackward,OUTPUT);  //
}

void loop()
{
 sCmd.readSerial();

if (debug) {
  while (Serial1.available()) {
    char inChar = Serial1.read();
    Serial.write(inChar);  // Dit echo't naar USB-serieel
  }
}
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;



    for (int i = 0; i < 6; i++)
    {
       normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
    }


    float position = 0;
    int index = 0;
    for (int i = 1; i < 6; i++) if (normalised[i] < normalised[index]) index = i;

    //if (normalised[index] < 300) run = false;

    if (index == 0) position = -30;
    else if (index == 5) position = 30;
    else
    {
      int sNul = normalised[index];
      int sMinEen = normalised[index-1];
      int sPlusEen = normalised[index+1];

      float b = sPlusEen - sMinEen;
      b = b / 2;

      float a = sPlusEen - b - sNul;

      position = -b / (2 * a);
      position += index;
      position -= 2.5;

      position *= 15;
    }
    debugPosition = position;

    float error = -position;
    float output = error * params.kp;

    params.iTerm += params.ki*error;
    params.iTerm = constrain(params.iTerm, -510,510);
    output += params.iTerm; 

    float lastErr;
    output += params.kd*(error - lastErr);
    lastErr = error; 

    output = constrain(output, -510,510);

    int powerLeft = 0;
    int powerRight = 0;

   if(run) if (output >= 0)
  {
    powerLeft = constrain(params.power + params.diff * output, -255, 255);
    powerRight = constrain(powerLeft - output, -255, 255);
    powerLeft = powerRight + output;
  }
  else
  {
    powerRight = constrain(params.power - params.diff * output, -255, 255);
    powerLeft = constrain(powerRight + output, -255, 255);
    powerRight = powerLeft - output;
  }

  analogWrite(MotorLeftForward, powerLeft > 0 ? powerLeft : 0);
  analogWrite(MotorLeftBackward, powerLeft < 0 ? -powerLeft : 0);
  analogWrite(MotorRightForward, powerRight > 0 ? powerRight : 0);
  analogWrite(MotorRightBackward, powerRight < 0 ? -powerRight : 0);

  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}


void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "debug") == 0) debug = (strcmp(value, "on") == 0);
  else if (strcmp(param, "cycle") == 0)
  {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;

    params.cycleTime = newCycleTime;
    
  }
  else if (strcmp(param, "power") == 0) params.power = atol(value);
  else if (strcmp(param, "diff") == 0) params.diff = atof(value);
  else if (strcmp(param, "kp") == 0) params.kp = atof(value);
  else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
  }
  else if (strcmp(param, "kd") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.kd = atof(value) / cycleTimeInSec;
  }
  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  
  EEPROM_writeAnything(0, params);
  
}

void onDebug()
{
   SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
 
  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
    SerialPort.println(" ");

  SerialPort.print("normalised: ");
  for (int i = 0; i < 6; i++)
  {
    SerialPort.print(normalised[i]);
    SerialPort.print(" ");
  }
    SerialPort.println(" ");

  SerialPort.print("position: ");
  SerialPort.println(debugPosition);

  SerialPort.print("power: ");
  SerialPort.println(params.power);
  SerialPort.print("diff: ");
  SerialPort.println(params.diff);
  SerialPort.print("kp: ");
  SerialPort.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("Ki: ");
  SerialPort.println(ki);

  float kd = params.kd * cycleTimeInSec;
  SerialPort.print("Kd: ");
  SerialPort.println(kd);


  SerialPort.print("calculationTime: ");
  SerialPort.println(calculationTime);
  calculationTime = 0; 

   SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
}
void onCalibrate ()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for(int i = 0; i < 6; i++) params.black[i]=analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");    
    for (int i = 0; i < 6; i++) params.white[i]=analogRead(sensor[i]);  
    SerialPort.println("done");      
  }

  EEPROM_writeAnything(0, params);
}

void onRun()
{
    unsigned long huidigeTijd = millis();
  if (huidigeTijd - laatsteInterrupt > 200) {
  laatsteInterrupt = huidigeTijd;
  run = !run;
  params.iTerm = 0;
  }
  if (run == HIGH)
  {
    digitalWrite(ledRun, HIGH);
  }
  else
  {
    digitalWrite(ledRun, LOW);
  } 
}

void onStop() {
  run = false;
  digitalWrite(ledRun, LOW);

  // Extra zekerheid: motoren volledig uitschakelen
  analogWrite(MotorLeftForward, 0);
  analogWrite(MotorLeftBackward, 0);
  analogWrite(MotorRightForward, 0);
  analogWrite(MotorRightBackward, 0);
}