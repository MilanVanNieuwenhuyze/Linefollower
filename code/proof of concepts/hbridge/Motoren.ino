#define RotatieLinks 7
#define RotatieRechts 8
#define SnelheidMotor1 9
#define SnelheidMotor2 10

void setup()  {
  
}

void loop() {
  digitalWrite(RotatieLinks, HIGH);
  analogWrite(SnelheidMotor1, 100);

  digitalWrite(RotatieRechts, HIGH);
  analogWrite(SnelheidMotor2, 100);
}
