const byte StartStop  = 3;
int LED = 13;
bool State;

void setup(){
 
  analogReference(DEFAULT);
  pinMode(StartStop, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
 
  attachInterrupt(digitalPinToInterrupt(StartStop), ProgrammaRun , RISING);
 
}

void loop(){
 
  if (State == true){
    
   digitalWrite(LED, HIGH);
   delay(1000);
   digitalWrite(LED, LOW);
   delay(1000);
  }
  
  else {
    digitalWrite(LED, LOW);
  }
}

void ProgrammaRun(){
 
  State =! State;
 
}
