/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/
#include <Arduino.h>

#include "io_defines.h"
#include "defines.h"

void read_analog();
void analog_input_begin();
void read_analog_begin();


const float beta = 4200.0;
const float r0 = 100000.0;
const float t0 = 273.0 + 25.0;
const float rx = r0 * exp(-beta/t0);
const int nAmostras = 7;
const float vcc = 3.3;
const float R = 85000.0;

float sumTemp_in = 0;
float sumTemp_out = 0;


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


void analog_input_begin(){
    analogReadResolution(12); 
    read_analog_begin();

}

void read_analog_begin(){
    float soma = 0;
    for (int i = 0; i < nAmostras; i++) {
        soma += analogRead(THERM1);
        delay (5);
    }
    float v = (vcc*soma)/(nAmostras*4095.0);
    float rt = (vcc*R)/v - R;

    // Calcula a temperatura
    double t = beta / log(rt/rx);
    float tempcelsius = t -273.0;
    sumTemp_in = tempcelsius*nAmostras;
    tempIn = (sumTemp_in/nAmostras);
    // Serial.println(analogRead(THERM1));
    soma = 0;
    for (int i = 0; i < nAmostras; i++) {
        soma += analogRead(THERM2);
        delay (5);
    }
    v = (vcc*soma)/(nAmostras*4095.0);
    rt = (vcc*R)/v - R;
    // Serial.println(analogRead(THERM2));
    // Calcula a temperatura
    t = beta / log(rt/rx);
    tempcelsius = t -273.0;
    sumTemp_out = tempcelsius*nAmostras;
    tempOut = (sumTemp_out/nAmostras);

    deltaTemp = tempIn - tempOut;
}

void read_analog(){
    float soma = 0;
    for (int i = 0; i < nAmostras; i++) {
        soma += analogRead(THERM1);
        delay (5);
    }
    float v = (vcc*soma)/(nAmostras*4095.0);
    float rt = (vcc*R)/v - R;

    // Calcula a temperatura
    double t = beta / log(rt/rx);
    float tempcelsius = t -273.0;
    sumTemp_in = sumTemp_in-tempIn+tempcelsius;
    tempIn = (sumTemp_in/nAmostras);
    // Serial.println(analogRead(THERM1));
    soma = 0;
    for (int i = 0; i < nAmostras; i++) {
        soma += analogRead(THERM2);
        delay (5);
    }
    v = (vcc*soma)/(nAmostras*4095.0);
    rt = (vcc*R)/v - R;
    // Serial.println(analogRead(THERM2));
    // Calcula a temperatura
    t = beta / log(rt/rx);
    tempcelsius = t -273.0;
    sumTemp_out = sumTemp_out-tempOut+tempcelsius;
    tempOut = (sumTemp_out/nAmostras);

    deltaTemp = tempIn - tempOut;
}