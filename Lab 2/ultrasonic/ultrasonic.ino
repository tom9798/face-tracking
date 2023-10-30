int triggerPin = 3;
int echoPin = 2;
unsigned long time = 0;
unsigned long duration = 0;

void setup()
{
  pinMode(triggerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(echoPin), calcDist, FALLING);
  // Set the interrupt routine
  Serial.begin(38400);
}

void loop()
{
  time = micros(); // Start the timer

  // Send the trigger pulse
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  delay(250); // Delay so we don't spam the system
}

void calcDist()
{
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = (micros() - time); // Stop the timer

  if (duration > 12000)
  {
    return; // Filter out too far measurements
  }
  Serial.println(duration); // turns out it takes at least 2388 us round trip,
                            // which equates 418760 MHz max
  return;
}
