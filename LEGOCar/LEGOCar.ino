int irR = 11;
int irL = 10;
int engR = 7;
int engL = 6;
int not_engR = 5;
int not_engL = 4;
byte conditions = 0;
int turn = 0;
void setup() {
  Serial.begin(9600);
  pinMode(irR, INPUT);
  pinMode(irL, INPUT);
  pinMode(engR, OUTPUT);
  pinMode(engL, OUTPUT);
  pinMode(not_engR, OUTPUT);
  pinMode(not_engL, OUTPUT);
    // rightS.attach(11);
    // LeftS.attach(10);
  }

void loop() {
  conditions = checkConditions();
  if (conditions == 0b10) // Go Right
  {
    if (turn == 0) {
      //turn right
      turn = 1;
      digitalWrite(engR, LOW);
      digitalWrite(not_engR, HIGH);
      digitalWrite(engL, HIGH);
      digitalWrite(not_engL, LOW);
      while (conditions != 0b01){
        conditions = checkConditions();
      }
    }else if(turn == 1){
      //fix right
      turn = 0;
      digitalWrite(engR, LOW);
      digitalWrite(not_engR, HIGH);
      digitalWrite(engL, HIGH);
      digitalWrite(not_engL, LOW);
      delay(100);
    }
  }
  // }else if(conditions == 0b10 && turn == 1){
  //   // fix left
  //   // turn = 0;
  //   digitalWrite(engR, HIGH);
  //   digitalWrite(not_engR, LOW);
  //   digitalWrite(engL, LOW);
  //   digitalWrite(not_engL, HIGH);
  //   delay(100);
  // }

  if (conditions == 0b01) // Go Left
  {
    if (turn ==0){
      //turn left
      turn = 1;
      digitalWrite(engR, HIGH);
      digitalWrite(not_engR, LOW);
      digitalWrite(engL, LOW);
      digitalWrite(not_engL, HIGH);
      while (conditions != 0b10){
        conditions = checkConditions();
      }
    }else if (turn == 1){
      // fix left
      turn = 0;
      digitalWrite(engR, HIGH);
      digitalWrite(not_engR, LOW);
      digitalWrite(engL, LOW);
      digitalWrite(not_engL, HIGH);
      delay(100);
    }
  }
  // }else if (conditions == 0b01 && turn == 1){
  //   //fix right
  //   // turn = 0;
  //   digitalWrite(engR, LOW);
  //   digitalWrite(not_engR, HIGH);
  //   digitalWrite(engL, HIGH);
  //   digitalWrite(not_engL, LOW);
  //   delay(100);
  // }
  if (conditions == 0b11) // Go straight
  {
    digitalWrite(engR, HIGH);
    digitalWrite(not_engR, LOW);
    digitalWrite(engL, HIGH);
    digitalWrite(not_engL, LOW);
    // delay(500);
  }
  if (conditions == 0b00) // Reverse
  {
    digitalWrite(engR, LOW);
    digitalWrite(not_engR, HIGH);
    digitalWrite(engL, LOW);
    digitalWrite(not_engL, HIGH);
    // delay(500);
  }
  conditions = 0;
}

byte checkConditions()
{
  byte cond = 0;
  if(digitalRead(irR) == LOW)
  {
    cond = cond | 0b01;
  }
  if(digitalRead(irL) == LOW)
  {
    cond = cond | 0b10;
  }
  return cond;
}