#include <Servo.h>

Servo arm;
int triggerPin = 3;
int echoPin = 2;
int timeIn = 5;
int timeOut = 6;
int servoPin = 9;
int high = 105;
int low = 87;

int cooldown = 0;
unsigned long counter = 0;
bool looping = false;

float dists[5];

void setup()
{
    pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);     // Sets the echoPin as an Input
    pinMode(timeOut, OUTPUT);
    pinMode(timeIn, INPUT);
    arm.attach(servoPin);
    Serial.begin(38400);
}

// PID-esque APPROACH

void loop()
{
    float dist = measureDist();
    // Serial.println(dist);
    if (dist < 0 || dist > 35) // Discard too far / close measurements
    {
        Serial.println("LOL I'M STUPID");
        return;
    }
    float avg = 0;
    dists[0] = dist;
    for (int i = 0; i < 5; i++) // Average 5 results for extra consistency
    {
        avg += dists[i];
    }
    avg /= 5;

    float vel = 0.5 * ((dists[0] + dists[1]) - (dists[3] + dists[4])); // Calculate velocity

    // Print everthing beautifully
    Serial.print("Push values: ");
    Serial.print(high);
    Serial.print(", ");
    Serial.print(low);
    Serial.print(". Velocity: ");
    Serial.print(vel);
    Serial.print(". Distance: ");
    Serial.println(avg);

    if (cooldown == 0) // Check if the velocity is too high and correct for it
    {
        if (vel < -2)
        {
            high--;
            cooldown = 100;
        }
        if (vel > 2)
        {
            low++;
            cooldown = 100;
        }
    }

    // This part includes a bit of black magic and
    // a lot of spaghetti code. It's not pretty, but it works.
    // Here be dragons. --Signed, Tom and Shraga.

    if (abs(vel) < 1) // Check if the ball is stuck in one side
    {
        if (!looping)
        {
            counter = 0; // Reset the counter
        }
        if (counter > 150) // If it was stuck for too long, increase the power
        {
            if (avg < 9)
            {
                low--;
            }

            if (avg > 22)
            {
                high++;
            }
            looping = false; // Reset the looping variable
        }
        else
        {
            looping = true; // Keep the looping variable so the timer doesn't reset
        }
    }
    else
    {
        looping = false; // Reset the looping variable if it doesnt enter the loop
    }

    if (avg < 8) // Check if the ball is at the near edge
    {
        arm.write(low);
    }
    else if (avg > 22) // Or at the far edge
    {
        arm.write(high);
    }

    else // Or somewhere in the middle
    {

        if (vel < -0.6) // Check if the ball is moving away from the sensor
        {
            arm.write(low);
        }
        else if (vel > 0.6) // Or towards the sensor
        {
            arm.write(high);
        }
        else // Or if its stationary in the center
        {
            arm.write(91);
        }
    }

    for (int i = 5; i > 0; i--) // Update the measurements
    {
        dists[i] = dists[i - 1];
    }

    counter++; // Increment the counter
    if (cooldown > 0)
    {
        cooldown--; // Decrement the cooldown
                    // to make sure we dont overcorrect the velocity
    }
    delay(25); // Delay to not spam the system and add a measurable delay for the velocity calculations
               // (and avoid rounding errors)
}

float measureDist() // Function to measure the distance
{
    // Clears the triggerPin
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sends a 10us pulse to triggerPin
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);
    // Calculate the distance
    return duration * 0.034 / 2;
}
