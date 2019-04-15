#include "eventheap.h"


#include <BQZUMJunior.h>

#include <BQZUMJuniorPorts.h>


Heap heap;

BQ::ZUMJunior zumJunior;

bool button1PinOn = false;
const uint8_t button1Pin = BQ::ZUMJunior::ports[1][0];
bool button2PinOn = false;
const uint8_t button2Pin = BQ::ZUMJunior::ports[2][0];
const uint8_t led1WhitePin = BQ::ZUMJunior::ports[3][0];
const uint8_t led1ColorPin = BQ::ZUMJunior::ports[3][1];
const uint8_t led2WhitePin = BQ::ZUMJunior::ports[4][0];
const uint8_t led2ColorPin = BQ::ZUMJunior::ports[4][1];

 
 
 
 
 
 
 
 void delay_3813() {
 digitalWrite(led1ColorPin, LOW);
 
 button1PinOn = false;

}
void delay_9387() {
 digitalWrite(led1ColorPin, HIGH);
 heap.delay(1000);
heap.insert(delay_3813);

 
}
void delay_2598() {
 digitalWrite(led1ColorPin, LOW);
 heap.delay(1000);
heap.insert(delay_9387);

 
}
void button1_pressed() {
 if(button1PinOn) return;
button1PinOn = true;
digitalWrite(led1ColorPin, HIGH);
 heap.delay(1000);
heap.insert(delay_2598);

 
}


void setup() {
  zumJunior.setup();

pinMode(button1Pin, INPUT);
pinMode(button2Pin, INPUT);
pinMode(led1WhitePin, OUTPUT);
pinMode(led1ColorPin, OUTPUT);

pinMode(led2WhitePin, OUTPUT);
pinMode(led2ColorPin, OUTPUT);


 
 
 
 
 
 
 
 
}

void loop() {
  heap.eventloop();

  if(digitalRead(button1Pin)) button1_pressed();
}
