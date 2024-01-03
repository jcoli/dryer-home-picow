/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/

#include <Arduino.h>
#include <ArduinoHA.h> //MQTT
#include <Bounce2.h>

#include "defines.h"
#include "io_defines.h"
#include "display.h"
#include "sounds.h"
#include "program.h"
#include "mqtt_controll.h"


void io_input_begin();
void check_door();
void io_input_controll();
void choice_item_select(int chg);


extern bool wifi_connected;

extern float tempIn;
extern float tempOut;
extern float deltaTemp;
extern bool dryerState;
extern bool heaterState;
extern bool heaterEnable;
extern bool fanState;
extern bool fanEnable;
extern bool dryerOn;
extern bool doorState;
extern bool lastDoorState;
extern int dryerCicle;
extern int dryerProgram;
extern bool cooling;
extern bool sleeping;

extern unsigned long loopDelay_cooling;
extern unsigned long loopDelay_door;
extern unsigned long loopDelay_start;
extern unsigned long loopDelay_on;
extern unsigned long loopDelay_sleep;

bool button0 = false;
bool button1 = false;
bool button2 = false;

#define NUM_BUTTONS 3
const uint8_t BUTTON_PINS[NUM_BUTTONS] = {BT_POWER, BT_PROGRAM, BT_CYCLE};
Bounce * buttons = new Bounce[NUM_BUTTONS];

void io_input_begin(){
    // pinMode(BT_POWER, INPUT_PULLUP);
    // pinMode(BT_PROGRAM, INPUT_PULLUP);
    // pinMode(BT_CYCLE, INPUT_PULLUP);
    Serial.println("input begin");  
    for (int i = 0; i < NUM_BUTTONS; i++) {
      // Serial.println(i);
      buttons[i].attach( BUTTON_PINS[i] , INPUT_PULLUP  );       //setup the bounce instance for the current button
      buttons[i].interval(50);              // interval in ms
      
    }
    Serial.println("input begin 1");  
    pinMode(BT_DOOR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BT_DOOR), check_door, CHANGE);
    

}

void io_input_controll(){
    if (lastDoorState != (digitalRead(BT_DOOR))){
      if (digitalRead(BT_DOOR)){
        doorState = false;
      }else{
        doorState = true;
      }  
      sleeping = false;
      digitalWrite(TFT_BACKLIGHT, HIGH);
      delay(50);
      door_check();

      loopDelay_sleep = millis();
      change_door(); 
      lastDoorState = (digitalRead(BT_DOOR));
      Serial.println("check_door"); 

    }

    for (int i = 0; i < NUM_BUTTONS; i++)  {
        buttons[i].update();
        if ( buttons[i].fell() ) {
          loopDelay_on = millis();
          loopDelay_sleep = millis();
          if (sleeping){
            sleeping = false;
            digitalWrite(TFT_BACKLIGHT, HIGH);
            play_start_sound();
              }else {  
            switch (i) {
                case 0:
                Serial.println("button0");
                choice_item_select(0);
                break;
                case 1:
                Serial.println("button1");
                choice_item_select(1);
                break;
                case 2:
                Serial.println("button2");
                choice_item_select(2);
                break;
                }
          }
        }  
    }
    // if (button0 != digitalRead(BT_POWER)){
    //   button0 = digitalRead(BT_POWER);
    //   Serial.println("button0");
    // }
    // if (button1 != digitalRead(BT_PROGRAM)){
    //   button1 = digitalRead(BT_PROGRAM);
    //   Serial.println("button1");
    // }
    // if (button2 != digitalRead(BT_CYCLE)){
    //   button2 = digitalRead(BT_CYCLE);
    //   Serial.println("button2");
    // }
    if (digitalRead(BT_DOOR)){ //closed
        doorState = false;
    }else{
        doorState = true;
    }
}

void choice_item_select(int chg) {
  switch (chg) {
      case 0:
        Serial.println("button 0 case 0");
        if ((doorState) && (!sleeping)){
          dryerOn = !dryerOn;
          if (dryerOn){
            Serial.println("Dryer On");
            program_begin();
            delay(500);
            program_controll();
          }else{
            Serial.println("Dryer Off");
            program_controll();
          }
        }  
        break;
      case 1:
        if (!dryerOn){
          Serial.println("button 1 case 1");
          dryerProgram++;
          if (dryerProgram>=4){
            dryerProgram=0;
          }
          
        }
        play_sleep_sound();
        Serial.println("button2");
        break;
      case 2:
        Serial.println("button 2 case 2");
        play_sleep_sound();
        
        break;  
    } 
    mqtt_update();
    change_data();
    
}

void check_door(){
  // Serial.println("Check Door");
  if (digitalRead(BT_DOOR)){
    doorState = false;
  }else{
    doorState = true;
  }  
  sleeping = false;
  digitalWrite(TFT_BACKLIGHT, HIGH);
  delay(50);
  door_check();

  loopDelay_sleep = millis();
  change_door(); 
  Serial.println("check_door"); 
  

}