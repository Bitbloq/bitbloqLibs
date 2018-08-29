/***   Included libraries  ***/
#include <BitbloqSoftwareSerial.h>




/***   Global variables and function definition  ***/
bqSoftwareSerial puerto_serie(0, 1, 9600);

float contador = 0;



/***   Setup  ***/
void setup() {
    puerto_serie.begin(9600);

}


/***   Loop  ***/
void loop() {}
