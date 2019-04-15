#include "eventheap.h"

#include <BQZUMJunior.h>
#include <BQZUMJuniorPorts.h>


Heap heap;

BQ::ZUMJunior zumJunior;


const uint8_t button1Pin = BQ::ZUMJunior::ports[1][0];
const uint8_t button2Pin = BQ::ZUMJunior::ports[2][0];
const uint8_t ledDoble1WhitePin = BQ::ZUMJunior::ports[3][0];
const uint8_t ledDoble1ColorPin = BQ::ZUMJunior::ports[3][1];
const uint8_t ledDoble2WhitePin = BQ::ZUMJunior::ports[4][0];
const uint8_t ledDoble2ColorPin = BQ::ZUMJunior::ports[4][1];

//flags to avoid button events triggers when actions are in course
bool button1PinOn = false;
bool button2PinOn = false;
/////////////////////// end flags ///////////////////////////////////


//ACTIONS TRIGGERED WHEN BUTTON 1 IS PRESSED /////////////////////////////////////


void button1action2(){
  zumJunior.play(BQ::ZUMJunior::SI,50);
  button1PinOn = false;
}

void button1_a_action3(){
  digitalWrite(ledDoble2ColorPin,HIGH);

  //THIS WOULD BE AFTER IF
  heap.insert(button1action2);
}

void button1_a_action2(){
  heap.delay(1000);
  heap.insert(button1_a_action3);
}

void button1_a_action1(){
  digitalWrite(ledDoble2ColorPin,LOW);
  heap.insert(button1_a_action2);

}

void button1_b_action3(){
  digitalWrite(ledDoble1ColorPin,HIGH);

  //THIS WOULD BE AFTER IF
  heap.insert(button1action2);
}

void button1_b_action2(){
  heap.delay(500);
  heap.insert(button1_b_action3);
}

void button1_b_action1(){
  digitalWrite(ledDoble1ColorPin,LOW);
  heap.insert(button1_b_action2);

}

void button1action1(){

  //EXAMPLE OF IF CONDITION

  if(digitalRead(button2Pin)){
    heap.insert(button1_a_action1);
  }else{
    heap.insert(button1_b_action1);
  }

}

/////////////////////////////// END ACTIONS BUTTON 1 //////////////////////////////////


void triggerButton1Actions(){
  if(button1PinOn) return;

  zumJunior.play(BQ::ZUMJunior::DO,50);
  button1PinOn = true;
  heap.insert(button1action1);
}

void setup(){
  zumJunior.setup();
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(ledDoble1WhitePin, OUTPUT);
  pinMode(ledDoble1ColorPin, OUTPUT);
  pinMode(ledDoble2WhitePin, OUTPUT);
  pinMode(ledDoble2ColorPin, OUTPUT);
  digitalWrite(ledDoble1WhitePin, LOW);
  digitalWrite(ledDoble1ColorPin, LOW);
  digitalWrite(ledDoble2WhitePin, LOW);
  digitalWrite(ledDoble2ColorPin, LOW);

  delay(1000);

  digitalWrite(ledDoble1WhitePin, HIGH);
  digitalWrite(ledDoble1ColorPin, HIGH);
  digitalWrite(ledDoble2WhitePin, HIGH);
  digitalWrite(ledDoble2ColorPin, HIGH);

}

void loop(){


  //loop over all the items and run operations if required
  heap.eventloop();


  //triggers
  if(digitalRead(button1Pin)) triggerButton1Actions();
}
