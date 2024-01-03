/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/


#ifndef __VARIABLES_H__
#define __VARIABLES_H__
#include <Arduino.h>

float tempIn = 0;
float tempOut = 0;
float tempCpu = 0;
float deltaTemp = 0;
bool dryerState = false;
bool heaterState = false;
bool heaterEnable = false;
bool fanState = false;
bool fanEnable = false; 
bool dryerOn = false;
bool doorState = true;
bool lastDoorState = false;
bool cooling = false;
bool sleeping = false;
int dryerCicle = 0;
int dryerProgram = 0;


String ipaddress;
long rssi;
bool led_out = false;
bool door = false;
bool wifi_connected = false;
bool mqtt_connected = false;

String line = "";
String line1 = "";
String line2 = "";
bool stringComplete = false;
bool string1Complete = false;
bool string2Complete = false;



unsigned long loopDelay = millis();
unsigned long loopDelay_analog = millis();
unsigned long loopDelay_input = millis();
unsigned long loopDelay_mqtt = millis();
unsigned long lastAvailabilityToggleAt = millis();

unsigned long loopDelay_cooling = millis();
unsigned long loopDelay_delta = millis();
unsigned long loopDelay_door = millis();
unsigned long loopDelay_door_open = millis();
unsigned long loopDelay_door_open_timeout = millis();
unsigned long loopDelay_start = millis();
unsigned long loopDelay_on = millis();
unsigned long loopDelay_sleep = millis();
unsigned long loopDelay_update = millis();


#endif