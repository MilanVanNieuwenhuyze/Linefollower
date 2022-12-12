#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial
#define Baudrate 9600

#define rotationL 7
#define rotationR 8
#define speedM1 9
#define speedM2 10

const byte StartStop = 3;

float iTerm = 0;
float lastErr = 0;

SerialCommand sCmd(SerialPort);
bool debug;
unsigned long previous, calculationTime;
bool run = false;

const int sensor[] = {A0, A1, A2, A3, A4, A5};
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
 } params;

void setup()
{
  SerialPort.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("debug", onDebug);

  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode(13, OUTPUT);
  SerialPort.println("ready");

  pinMode(StartStop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(StartStop), ProgrammaRun , FALLING);
}

void loop()
{
  sCmd.readSerial();

  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    int normalised[6];

    for (int i = 0; i < 6; i++)
    {
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);

    }


    /* interpolatie */
    int index = 0;
    float position;
    for (int i = 1; i < 6; i++) if (normalised[i] < normalised[index]) index = i;

    if (normalised[index] > 700) run = false;

    if (index == 0) position = -30;
    else if (index == 5) position = 30;
    else
    {
      int sNul = normalised[index];
      int sMinEen = normalised [index - 1];
      int sPlusEen = normalised[index + 1];

      float b = sPlusEen - sMinEen;
      b = b / 2;

      float a = sPlusEen - b - sNul;

      position = -b / (2 * a);
      position += index;
      position -= 2.5;

      position *= 9.525;
    }

    /* Proportionele regelaar */
    
    float error = -position;
    float output = error * params.kp;

    /* Integrerend */

    iTerm += params.ki * error;
    iTerm = constrain(iTerm, -510, 510);
    output += iTerm;

    /* Differentiërend */

    output += params.kd * (error - lastErr);
    lastErr = error;

    output = constrain(output, -510, 510);


    if (run)
    {

      int powerLeft = 0;
      int powerRight = 0;

      if (output >= 0)
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

      if (powerRight < 0) digitalWrite(rotationR, LOW);
      if (powerRight > 0) digitalWrite(rotationR, HIGH);
      if (powerLeft < 0) digitalWrite(rotationL, LOW);
      if (powerLeft > 0) digitalWrite(rotationL, HIGH);

      analogWrite(speedM1, abs(powerRight));
      analogWrite(speedM2, abs(powerLeft));
    }
    else  // run = false;
    {

      digitalWrite(rotationR, LOW);
      digitalWrite(rotationL, LOW);

      analogWrite(speedM1, 0);
      analogWrite(speedM2, 0);
    }

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

void ProgrammaRun() {
  run = ! run;
  iTerm = 0;
  lastErr = 0;
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();

  if (strcmp(param, "cycle") == 0)
  {
    params.cycleTime = atol(value);
    long newCycleTime = atol(value); 
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;
    params.kd /= ratio;

    params.cycleTime = newCycleTime;

  }

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
  else if (strcmp(param, "run") == 0) run = atol(value);
  else if (strcmp(param, "power") == 0) params.power = atol(value);
  else if (strcmp(param, "diff") == 0) params.diff = atof(value);
  else if (strcmp(param, "kp") == 0) params.kp = atof(value);

  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);

  SerialPort.print("black: ");
  for (int i = 0; i < 6; i++) {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i = 0; i < 6; i++) {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("kp: ");
  SerialPort.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("Ki: ");
  SerialPort.println(ki);

  float kd = params.kd * cycleTimeInSec;
  SerialPort.print("Kd: ");
  SerialPort.println(kd);

  SerialPort.print("diff: ");
  SerialPort.println(params.diff);

  SerialPort.print("power: ");
  SerialPort.println(params.power);

  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}


void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 6; i++) params.black[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");
    for (int i = 0; i < 6; i++) params.white[i] = analogRead(sensor[i]);
    SerialPort.println("done");
  }

  EEPROM_writeAnything(0, params);
}
