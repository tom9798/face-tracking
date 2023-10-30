#include <Servo.h> // Imports the servo library

Servo xServo;
Servo yServo;

String data;
long x = 90;
long y = 90;
int currentX = 90;
int currentY = 90;
int temp = 0;

// Tom really likes the phrase "Here be dragons" so I'm including it here.
// Here be dragons.

void setup(){
  Serial.begin(38400);
  Serial.setTimeout(1);
  xServo.attach(10);
  yServo.attach(11);
}

void loop(){
  while (Serial.peek() != 65 && Serial.available() > 0){
    Serial.read();
  }

  if (Serial.available() >= 7){
    if (Serial.peek() == 65){

      data = Serial.readString();
      x = data.substring(1, 4).toInt();
      y = data.substring(4, 7).toInt();
      

      // Debug prints:
      // Serial.print("Data: ");
      // Serial.println(data);
      // Serial.print(x);
      // Serial.print(", ");
      // Serial.println(y);
    }
  }

  // Write to the servos
  if (currentX != x){
    if (currentX < x){
      currentX++;
    }else{
      currentX--;
    }
    xServo.write(currentX);
  }
  if (currentY != y){
    if (currentY < y){
      currentY++;
    }
    else{
      currentY--;
    }
    yServo.write(currentY);
  }
  delay(50);
}
