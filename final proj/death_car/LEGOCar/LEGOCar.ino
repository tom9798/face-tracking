int irR = 3;
int irL = 2;
int irF = A3;
int fLed = A4;
int engR = 7;
int engL = 6;
int not_engR = 5;
int not_engL = 4;
byte conditions = 0;
int ENR = 10;
int ENL = 11;
int START = 8;
bool running;
int rLed = A2;
int lLed = A1;

void setup() {
  Serial.begin(9600);
  pinMode(irR, INPUT);
  pinMode(irL, INPUT);
  pinMode(engR, OUTPUT);
  pinMode(engL, OUTPUT);
  pinMode(not_engR, OUTPUT);
  pinMode(not_engL, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(ENL, OUTPUT);
  pinMode(rLed, OUTPUT);
  pinMode(lLed, OUTPUT);
  pinMode(fLed, OUTPUT);
  
  }

// Please note we are a pair of funny people, and we decided to give out motors names.
// "motor ranger" is our surname, and since we are Asian, we put the surname first.
// We also named the right motor "2500", and the left motor "2501".
// Please make sure to respect their names and pronouns (it/its).


void loop() {
  conditions = checkConditions(); //checking where are we

  //start button
  if (!running){

    digitalWrite(engR, HIGH);
    digitalWrite(not_engR, LOW);
    digitalWrite(engL, HIGH);
    digitalWrite(not_engL, LOW);
    Vroom("motor ranger 2500", 0);
    Vroom("motor ranger 2501", 0);

    if (digitalRead(START)){
      running = true;
    }
    return;
  }
  
  if ((conditions & 0b100) == 0b000) // death for everybody
  {
    digitalWrite(engR, HIGH);
    digitalWrite(not_engR, LOW);
    digitalWrite(engL, HIGH);
    digitalWrite(not_engL, LOW);
    Vroom("motor ranger 2500", 55);
    Vroom("motor ranger 2501", 55);
  }
  else{
    if ((conditions & 0b011) == 0b011) // Go straight
    {
      digitalWrite(engR, HIGH);
      digitalWrite(not_engR, LOW);
      digitalWrite(engL, HIGH);
      digitalWrite(not_engL, LOW);
      Vroom("motor ranger 2500", 50);
      Vroom("motor ranger 2501", 50);
    }
    
    if ((conditions & 0b011) == 0b010) // Go Right
    {
      digitalWrite(engR, LOW);
      digitalWrite(not_engR, HIGH);
      digitalWrite(engL, HIGH);
      digitalWrite(not_engL, LOW);
      Vroom("motor ranger 2500", 67);
      Vroom("motor ranger 2501", 55);

      //do that for 150mms unless you reach the line, then stop
      delay(70);
      for(int i = 0; i < 30; i++){
        conditions = checkConditions();
        delay(5);
        if ((conditions & 0b011) != 0b011 || (conditions & 0b100) != 0b100){
          return;
        }
      }
    }
    
    if ((conditions & 0b011) == 0b001) // Go Left
    {
      digitalWrite(engR, HIGH);
      digitalWrite(not_engR, LOW);
      digitalWrite(engL, LOW);
      digitalWrite(not_engL, HIGH);
      Vroom("motor ranger 2500", 55);
      Vroom("motor ranger 2501", 67);
      
      //do that for 150mms unless you reach the line, then stop
      delay(70);
      for(int i = 0; i < 30; i++){
        conditions = checkConditions();
        delay(5);
        if ((conditions & 0b011) != 0b011 || (conditions & 0b100) != 0b100){
          return;
        }

      }
    }
  }
  
}


byte checkConditions()
{
  byte cond = 0;
  if(digitalRead(irR) == LOW)
  {
    cond = cond | 0b001;
  }
  if(digitalRead(irL) == LOW)
  {
    cond = cond | 0b010;
  }
  if(digitalRead(irF) == LOW)
  {
    cond = cond | 0b100;
  } 
  digitalWrite(rLed, (cond & 0b001) != 0);
  digitalWrite(lLed, (cond & 0b010) != 0);
  digitalWrite(fLed, (cond & 0b100) != 0);
  return cond;
}

//pwm so we can control the speed of the car, implemented with precentage out of the full speed
void Vroom(String motor, int speed){
  if (motor == "motor ranger 2500"){
    analogWrite(ENR, (speed*255)/100);
  }
  if (motor == "motor ranger 2501"){
    analogWrite(ENL, (speed*255)/100);
  }
}