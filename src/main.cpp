/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/

#include <Arduino.h>
#include <WiFi.h>

#include "wifi.h"
#include "mqtt_controll.h"
#include "defines.h"
#include "io_defines.h"
#include "display.h"
#include "input_digital_control.h"
#include "output_digital_control.h"
#include "input_analog_control.h"
#include "sounds.h"
#include "program.h"
#include "variables.h"


#define pinToUse      0
bool test = false;

void setup() {
  Serial.begin(115200);
  display_begin();
  Serial.println("teste");
  delay(1000);
  
  
  wifi_begin();
  // Serial.println("teste4");
  
  delay(1000);
  mqtt_begin();
  delay(1000);
  // show_display();
  Serial.println("teste54");
  // io_input_controll(); 
  delay(300);
  // change_data();
  Serial.println("teste6");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pinToUse, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(pinToUse, HIGH);
  // play_sleep_sound();
  
  io_input_begin();
  Serial.println("teste1");
  analog_input_begin();
  Serial.println("teste2");
  io_output_begin();
  Serial.println("teste33");
  Serial.println("teste4");
  delay(1000);
  delay(300);
  Serial.println("teste4a");
  show_display();
  change_data();
  play_sleep_sound();
}

// void setup1() {
 
// }

void loop() {
  if (((millis() - loopDelay_sleep) > TIMER_SLEEP) && !sleeping){
      if ((!dryerState) || (!doorState)){
        digitalWrite(TFT_BACKLIGHT, LOW);
        sleeping = true;
        Serial.println("sleeping");  
      }else{
        digitalWrite(TFT_BACKLIGHT, HIGH);  
        loopDelay_sleep = millis();
        sleeping = false;
        Serial.println("not sleeping");  
      }  
    }  

    if (millis() - loopDelay_mqtt > TIMER_MQTT){
        loopDelay_mqtt = millis();
        mqtt_loop();
        io_input_controll();    
        
    } 

    if ((millis() - loopDelay) > TIMER_SEND_MQTT){
       loopDelay = millis();
       checkStatus();
       program_controll();
       mqtt_update();  
    // if(wifi_connected){
    //   digitalWrite(WIFI_LED, HIGH);
    // }else{
    //   digitalWrite(WIFI_LED, LOW);
    //   wifi_begin();
    //   delay(100);
    // }
    if (digitalRead(LED_BUILTIN)){
          digitalWrite(LED_BUILTIN, LOW); 
          digitalWrite(pinToUse, LOW);
          test = false;
          Serial.println("False");
        }else{
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(pinToUse, HIGH);
          test = true;
          Serial.println("True");
        }
        Serial.printf("Core temperature: %2.1fC\n", analogReadTemp());
  }
  if ((millis() - loopDelay_analog) > TIMER_ANALOG){
    loopDelay_analog = millis();
    read_analog();
    if (dryerOn){
      program_controll();
    }
    
    if (test)    Serial.printf("Core 1 temperature: %2.1fC\n", analogReadTemp());
  }
  if (millis() - loopDelay_input > TIMER_INPUT){
    loopDelay_input = millis();
    io_input_controll();  
    
  }
  if ((millis() - loopDelay_update) > TIMER_UPDATE){
      Serial.println("update");  
      // change_data();
      loopDelay_update = millis();
  }  
}

// void loop1() {
//   if ((millis() - loopDelay_analog) > TIMER_ANALOG){
//     loopDelay_analog = millis();
//     read_analog();
//     if (dryerOn){
//       program_controll();
//     }
//     if (test)    Serial.printf("Core 1 temperature: %2.1fC\n", analogReadTemp());
//   }
//   if (millis() - loopDelay_input > TIMER_INPUT){
//     loopDelay_input = millis();
//     io_input_controll();   
//   }
//   if ((millis() - loopDelay_update) > TIMER_UPDATE){
//       Serial.println("update");  
//       // change_data();
//       loopDelay_update = millis();
//   }  
// }