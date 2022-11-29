char Analog_Pins[] = {A0,A1,A2,A3,A4,A5};
int i;
int LedPin = 8;

void setup() 
{
  for(i=0; i<=5; i++) {
    pinMode(Analog_Pins[i], INPUT); 
  } 
  pinMode(LedPin, OUTPUT);
  digitalWrite(LedPin, HIGH);
 Serial.begin(9600); 
}

void loop() 
{
  for(i=0; i<=5; i++) {
 Serial.print(i+2);
 Serial.print(" is: ");
 Serial.println(analogRead(Analog_Pins[i]));
 Serial.println();
  }

 Serial.println();
 Serial.println();
 delay(1000); 
}
