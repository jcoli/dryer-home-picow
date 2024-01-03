/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/
#include <Arduino.h>
#include <ArduinoHA.h> //MQTT


#include "defines.h"
#include "io_defines.h"
#include "wifi.h"
#include "display.h"
#include "mqtt_controll.h"
#include "sounds.h"
#include "input_digital_control.h"


void program_begin();
void door_check();
void program_controll();
void output_control();

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
extern int dryerCicle;
extern int dryerProgram;
extern bool cooling;
extern bool sleeping;

extern unsigned long loopDelay_cooling;
extern unsigned long loopDelay_door;
extern unsigned long loopDelay_start;
extern unsigned long loopDelay_on;
extern unsigned long loopDelay_delta;
extern unsigned long loopDelay_door_open;
extern unsigned long loopDelay_door_open_timeout;
extern unsigned long loopDelay_sleep;

extern HASwitch switch_on;
extern HABinarySensor door_state;
extern HABinarySensor heater_state;
extern HABinarySensor heater_enable;
extern HABinarySensor fan_state;
extern HABinarySensor fan_enable;
extern HABinarySensor state_sensor;
extern HASensorNumber temp_in_sensor;
extern HASensorNumber temp_out_sensor;
extern HASensorNumber delta_sensor;
extern HASensorNumber dryer_cycle;
extern HASelect program_select;

long timerCicle_0 = 0;
long timerCicle_1 = 0;
long timerCicle_2 = 0;
long timerCicle_3 = 0;
long timerCicle_4 = 0;
long timeOut = 0;

float delta_P = 0.0;
float delta_P_Prop = 0.0;

float tempHeaterOff = 0.0;
float tempHeaterOn = 0.0;

void program_begin(){
    if (dryerOn){
        switch (dryerProgram){
        case 0:
            Serial.println("program i 0");
            timerCicle_0 = TIMER_0_CICLE_0;
            timerCicle_1 = TIMER_0_CICLE_1;
            timerCicle_2 = TIMER_0_CICLE_2;
            timerCicle_3 = TIMER_0_CICLE_3;
            timerCicle_4 = TIMER_0_CICLE_4;
            delta_P = DELTA_P0;
            delta_P_Prop = DELTA_P0_PROP;
            tempHeaterOff = TEMP_HEATER_0_OFF;
            tempHeaterOn = TEMP_HEATER_0_ON;
            break;
        case 1:
            Serial.println("program i 1");
            timerCicle_0 = TIMER_1_CICLE_0;
            timerCicle_1 = TIMER_1_CICLE_1;
            timerCicle_2 = TIMER_1_CICLE_2;
            timerCicle_3 = TIMER_1_CICLE_3;
            timerCicle_4 = TIMER_1_CICLE_4;
            delta_P = DELTA_P1;
            delta_P_Prop = DELTA_P1_PROP;
            tempHeaterOff = TEMP_HEATER_1_OFF;
            tempHeaterOn = TEMP_HEATER_1_ON;
            break;    
        case 2:
            Serial.println("program i 2");
            timerCicle_0 = TIMER_2_CICLE_0;
            timerCicle_1 = TIMER_2_CICLE_1;
            timerCicle_2 = TIMER_2_CICLE_2;
            timerCicle_3 = TIMER_2_CICLE_3;
            timerCicle_4 = TIMER_2_CICLE_4;
            delta_P = DELTA_P2;
            delta_P_Prop = DELTA_P2_PROP;
            tempHeaterOff = TEMP_HEATER_2_OFF;
            tempHeaterOn = TEMP_HEATER_2_ON;
            break;    
        case 3:
            Serial.println("program i 3");
            timerCicle_0 = TIMER_3_CICLE_0;
            timerCicle_1 = TIMER_3_CICLE_1;
            timerCicle_2 = TIMER_3_CICLE_2;
            timerCicle_3 = TIMER_3_CICLE_3;
            timerCicle_4 = TIMER_3_CICLE_4;
            delta_P = DELTA_P3;
            delta_P_Prop = DELTA_P3_PROP;
            tempHeaterOff = TEMP_HEATER_3_OFF;
            tempHeaterOn = TEMP_HEATER_3_ON;
            break;    
        default:
            break;
        }
        heaterEnable = true;
        fanEnable = true;
        dryerState = true;

        loopDelay_cooling = millis();
        loopDelay_delta = millis();
        loopDelay_door = millis();
        loopDelay_door_open = millis();
        loopDelay_door_open_timeout = millis();
        loopDelay_start = millis();
        loopDelay_on = millis();
        loopDelay_sleep = millis();
        delay(200);
    }else{
        heaterEnable = false;
        fanEnable = false;
        dryerState = false;
    }
}

void program(){
    if ((dryerState) && (dryerOn) && (dryerCicle == 0)){
        // Serial.println("program");
        heaterState = true;
        heaterEnable = true;
        fanState = true;
        fanEnable = true;
        dryerCicle = 1;
        loopDelay_start = millis();
        change_data();
        play_start_sound();
        
    }else if ((dryerState) && (!dryerOn)){
        dryerState = false;
        heaterState = false;
        fanState = false;
        dryerCicle = 0;
        loopDelay_on = millis();
        loopDelay_sleep = millis();
        // Serial.println("program 2");
        play_sleep_sound();
        change_data();
    }   
    // }else if (!dryerOn){
    //     loopDelay_on = millis();
    // }
    if ((dryerOn) && ((millis() - loopDelay_on)>TIME_OUT)){
        dryerState = false;
        dryerOn = false;
        heaterState = false;
        heaterEnable = false;
        fanState = false;
        dryerCicle = 0;
        loopDelay_on = millis();
        digitalWrite(PW_HEATER, LOW);
        digitalWrite(PW_FAN, LOW);
        play_finish_sound();
        mqtt_update();
        change_data();

    }
    // Serial.print("program: ");
    // Serial.print((millis() - loopDelay_start));
    // Serial.print(" -- ");
    // Serial.println(timerCicle_1);
    if (((millis() - loopDelay_start)>timerCicle_1) && (dryerCicle == 1)){
    //    Serial.println("program 3"); 
       dryerCicle = 2;
       loopDelay_delta = millis();
       change_data();
    }

    if ((dryerCicle == 2) && (deltaTemp < (tempIn*delta_P_Prop/100)) && (heaterState)) {
        // Serial.print("program 4: ");  
        // Serial.print(deltaTemp);  
        // Serial.print(" - ");  
        // Serial.println(tempIn*delta_P_Prop/100);  
        dryerCicle = 3;
        loopDelay_delta = millis();
        change_data();
    }else if(dryerCicle == 2){
        Serial.println("program 4a");
        loopDelay_delta = millis();
        // Serial.print("program 4: ");  
        // Serial.print(deltaTemp);  
        // Serial.print(" - ");  
        // Serial.println(tempIn*delta_P_Prop/100); 
        
    }
    if ((dryerCicle == 2) && ((millis() - loopDelay_start)>timerCicle_2)){
        dryerCicle = 4;
        cooling = true;
        heaterState = false;
        loopDelay_cooling = millis();
        play_sleep_sound();
        change_data();
    }


    if ((dryerCicle == 3) && (deltaTemp > (tempIn*delta_P_Prop/100)) && (heaterEnable)){
        // Serial.println("program 5"); 
        dryerCicle = 2;
        loopDelay_delta = millis();
        change_data();
    }

    if (((millis() - loopDelay_delta) > timerCicle_3) && dryerCicle == 3 ){
        if(deltaTemp < (tempIn*delta_P_Prop/100)){
            // Serial.println("program 6");
            dryerCicle = 4;
            cooling = true;
            heaterEnable = false;
            loopDelay_cooling = millis();
            play_sleep_sound();
            change_data();
        }else{
            // Serial.println("program 6a");
            dryerCicle = 2;
            loopDelay_delta = millis();
            change_data();
        }    
    }    
    if (((millis() - loopDelay_cooling) > timerCicle_4) && dryerCicle == 4){
        // Serial.println("program 7");
        dryerState = false;
        fanState = false;
        dryerOn = false;
        dryerCicle = 0;
        change_data();
        play_finish_sound(); 
    }

}

void program_controll(){
    Serial.println("controll");   
    if (!doorState){
        door_check();
        // Serial.println("controll a");  
        
    }else{
        program();
        door_check();
        loopDelay_on = millis();
        if ((dryerState) ){
            //    Serial.println("controll 1");   
               loopDelay_sleep = millis(); 

                if (heaterEnable && doorState){
                    // Serial.println("controll 2");
                    if (tempIn<tempHeaterOn){
                        heaterState = true;
                    }else{
                        heaterState = false;
                    }
                }else{
                    heaterState = false;
                }

                if (fanEnable && doorState) {
                    fanState = true;
                }else{
                    fanState = false;
                }
                if (tempIn>=tempHeaterOff){
                    heaterState = false;
                }
                
        }else if(!dryerState){
                dryerState = false;
                heaterState = false;
                heaterEnable = false;
                fanState = false;
                fanEnable = false;
                dryerOn = false;
                cooling = false;
                dryerCicle = 0;
                
                
        }
    }
    output_control();
    mqtt_update();
    
    
}

void output_control(){
    digitalWrite(PW_HEATER, heaterState);
    digitalWrite(PW_FAN, fanState);
}

void door_check(){
    if (doorState){
        // Serial.println("door close");
        loopDelay_door = millis();
        loopDelay_door_open = millis();
        loopDelay_door_open_timeout = millis();
    }else{
        if (dryerState){
            // Serial.println("door 2");
            if ((millis() - loopDelay_door) > TIMER_DOOR_BEEP){
                if ((millis() - loopDelay_door_open) > TIMER_DOOR_OPEN){
                        Serial.println("door 2 a");
                        play_door_sound();
                        loopDelay_door_open = millis();
                        loopDelay_start = millis();
                        loopDelay_delta = millis();
                        loopDelay_cooling = millis();
                }    
            }else{
                loopDelay_start = millis();
                loopDelay_delta = millis();
                loopDelay_cooling = millis();
            }
            fanState = false;
            heaterState = false;
            output_control();
        }          
        
        
        if (((millis() - loopDelay_door_open_timeout) > TIMER_DOOR_OPEN_TIMEOUT) && (dryerCicle > 0)){
                dryerOn = false;
                dryerCicle = 0;  
                dryerState = false;
                heaterState = false;
                heaterEnable = false;
                fanState = false;
                fanEnable = false;
                cooling = false;
                play_sleep_sound(); 
                digitalWrite(PW_HEATER, LOW);
                digitalWrite(PW_FAN, LOW);
                mqtt_update();
                change_data();
        }
    } 
       
    
    
}
