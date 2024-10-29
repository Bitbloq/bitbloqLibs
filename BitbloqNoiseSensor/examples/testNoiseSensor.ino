/***   Included libraries  ***/
#include <BitbloqNoiseSensor.h>

NoiseSensor sensor(A0);

/***   Setup  ***/
void setup() {
    Serial.begin(9600);
}

/***   Loop  ***/
void loop() {
    Serial.println(sensor.readNoise());
}
