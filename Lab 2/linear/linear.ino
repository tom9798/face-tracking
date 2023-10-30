#include <HCSR04.h>
#include <Servo.h>

Servo arm;
int triggerPin = 3;
int echoPin = 4;
int servoPin = 9;
float dists[5];

int delta = 7;
int center = 91;

UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

void setup()
{
    arm.attach(servoPin);
    Serial.begin(38400);
}

// Linear approach

void loop()
{
    float dist = distanceSensor.measureDistanceCm();
    if (dist < 0) // Discard too far / close measurements
    {
        return;
    }
    float avg = 0;
    dists[0] = dist;
    for (int i = 0; i < 5; i++) // Average 5 results for extra consistency
    {
        avg += dists[i];
    }
    avg /= 5;

    int val = map(avg, 5, 20, center - delta, center + delta); // The LERP magic
    arm.write(val);

    for (int i = 5; i > 0; i--) // Update the measurements
    {
        dists[i] = dists[i - 1];
    }
    delay(10); // Delay to not spam the system
}