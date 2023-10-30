#include <Arduino.h>
#include <Servo.h>
// #include <RG.h>

int alarm = 13;
int irLeft = 6;
int irRight = 5;
int irBack = 7;
int PinLedLeft = 4;
int PinLedRight = 5;
int PinLedBack = 3;
int val = 0;
int cntr = 0;
byte conditions = 0;
int force1 = 85;
int force2 = 85;
Servo rightS;
Servo LeftS;

int val1=0;
int val2=0;

int highR = 104;
int lowR = 80;
int midR = 90;
int highL = 98;
int lowL = 80;
int midL = 85;


	void setup()
	{
    Serial.begin(9600);
		pinMode(irRight, INPUT);
    pinMode(irLeft, INPUT);
    pinMode(irBack, INPUT);
    pinMode(alarm, OUTPUT);
    rightS.attach(11);
    LeftS.attach(10);
	}

	void loop()
	{
		conditions = checkConditions();

    digitalWrite(alarm, (conditions & 0b101) == 5);

    // turn left

    if (conditions == 0b111) // Go back, your'e off course
    {
      rightS.write(highR);
      LeftS.write(lowR);
      // delay(500);

    }
    if (conditions == 0b100) // Turn right
    {
      rightS.write(lowR);
      LeftS.write(midL);
      // delay(600);
      // rightS.write(lowR);
      // LeftS.write(highL);
      // delay(1500);
    }
    if (conditions == 0b001) // Turn left
    {
      rightS.write(midR);
      LeftS.write(highL);
      // delay(600);
      // rightS.write(lowR);
      // LeftS.write(highL);
      // delay(1500);
    }
    if (conditions == 0b101) // Go straight
    {
      rightS.write(lowR);
      LeftS.write(highL);
      // delay(500);
    }
    if (conditions == 0b000) // Go wild!
    {
      val1=random(-1,1);
      val2=random(-1,1);
      while(conditions == 0b000){
        rightS.write(85+30*val1);
        LeftS.write(85+30*val2);
        conditions = checkConditions();
      }
    }
    conditions = 0;
	}



byte checkConditions()
{
  byte cond = 0;
  if(digitalRead(irRight) == LOW)
  {
    cond = cond | 0b100;
  }
  if(digitalRead(irLeft) == LOW)
  {
    cond = cond | 0b001;
  }
  if(digitalRead(irBack) == LOW)
  {
    cond = cond | 0b010;
  }
  return cond;
}