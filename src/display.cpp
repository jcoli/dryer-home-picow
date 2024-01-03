/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Free_Fonts.h>

#include "defines.h"
#include "io_defines.h"
#include "wifi.h"
#include "mqtt_controll.h"
#include "sounds.h"

#include "icons/images.h"
#include "icons/wi-fi.h"
#include "icons/fan-2.h"
#include "icons/alert-decagram-outline-25.h"
#include "icons/bell-alert.h"
#include "icons/thermometer-alert.h"
#include "icons/thermometer.h"
#include "icons/home-assistant.h"
#include "icons/fan-13.h"
#include "icons/door+closed.h"
#include "icons/bx+door+open.h"
#include "icons/bxs+door+open.h"
#include "icons/dryer.h"
#include "icons/arduino.h"
#include "icons/snowflake.h"
#include "icons/heat-wave.h"
#include "icons/tumble-dryer.h"
#include "icons/logo.h"
#include "icons/cat.h"
#include "icons/vin3.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDA, TFT_SCL, TFT_RST);

void display_begin();
void show_display();
void change_door();
void change_data();


extern bool wifi_connected;

extern float tempIn;
extern float tempOut;
extern float deltaTemp;
extern bool dryerState;
extern bool heaterState;
extern bool fanState;
extern bool dryerOn;
extern bool doorState;
extern int dryerCicle;
extern int dryerProgram;
extern bool sleeping;
extern bool mqtt_connected;

extern unsigned long loopDelay_cooling;
extern unsigned long loopDelay_door;
extern unsigned long loopDelay_start;
extern unsigned long loopDelay_on;
extern unsigned long loopDelay_sleep;

void display_begin(){
    tft.initR(INITR_BLACKTAB); // Initialize ST7735R screen
    // tft.initR(INITR_GREENTAB); 
    tft.setRotation(0);
    delay(100);
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
    tft.setFont(&FreeMono9pt7b);
    // tft.setFont(&FreeMonoBold9pt7b);
    // tft.setFont(&TomThumb);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_RED);
//     tft.setCursor(0,13);
//     tft.print("Secadora");
//     tft.setTextColor(ST77XX_WHITE);
//     tft.setCursor(0,50);
//     tft.print("Init.....");
//     Serial.println("teste");
     tft.drawBitmap(2, 0, vin3, 120, 25, ST7735_RED);
    //  tft.drawBitmap(2, 16, tumbledryer, 32, 32, ST7735_RED);
    //  tft.drawBitmap(42, 16, heatwave, 32, 32, ST7735_RED);
    //  tft.drawBitmap(82, 16, snowflake, 32, 32, ST7735_RED);
    //  tft.drawBitmap(2, 50, arduinodisplay, 32, 32, ST7735_CYAN);
    //  tft.drawBitmap(42, 50, dryer, 32, 32, ST7735_CYAN);
    //  tft.drawBitmap(82, 50, bxsdooropen, 32, 32, ST7735_CYAN);
    //  tft.drawBitmap(2, 85, wifi, 32, 32, ST7735_YELLOW);
    //  tft.drawBitmap(42, 85, thermometeralert, 32, 32, ST7735_YELLOW);
    //  tft.drawBitmap(82, 85, doorclosed, 32, 32, ST7735_YELLOW);
    //  tft.drawBitmap(2, 120, fan, 32, 32, ST7735_ORANGE);
    //  tft.drawBitmap(42, 120, thermometer, 32, 32, ST7735_ORANGE);
    //  tft.drawBitmap(82, 120, fan13, 32, 32, ST7735_GREEN);
     
}

void show_display(){
    tft.setTextWrap(true);
    tft.setTextSize(1);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST7735_YELLOW);
    // tft.setCursor(3,13);
    // tft.print("Secadora");
    tft.drawBitmap(2, 0, vin3, 120, 25, ST7735_RED);
    
    
    // tft.setCursor(100, 13);
    if(wifi_connected){
        tft.drawBitmap(2, 20, wifi, 32, 32, ST7735_GREEN);
    } else{
        tft.drawBitmap(2, 20, wifi, 32, 32, ST7735_YELLOW);
    }
    // tft.drawBitmap(50, 15, dryer, 32, 32, ST7735_YELLOW);
    tft.fillRect(90,20, 110,35, ST7735_BLACK);  
    if (doorState){
        tft.drawBitmap(90, 20, doorclosed, 32, 32, ST7735_GREEN);
    }else{
        tft.drawBitmap(90, 20, bxsdooropen, 32, 32, ST7735_ORANGE);
    }
    if (mqtt_connected){
        tft.drawBitmap(47, 20, homeassistant, 32, 32, ST7735_GREEN);
    }else{
        tft.drawBitmap(47, 20, homeassistant, 32, 32, ST7735_ORANGE);
    }

    tft.drawRoundRect(2,56,125,37, 5, ST7735_YELLOW);
    tft.drawBitmap(5, 58, dryer, 32, 32, ST7735_YELLOW);
    tft.setCursor(42,78);
    if (dryerProgram == 0){
        tft.print("Normal");
    }else if (dryerProgram == 1){
        tft.print("Medio");
    }else if (dryerProgram == 2){
        tft.print("Leve");
    }else if (dryerProgram == 3){
        tft.print("Aquecer");
    }

    tft.drawRoundRect(2,95,126,64, 5, ST7735_YELLOW);
    tft.setCursor(42,131);
    if (!dryerOn){ 
        tft.print("Deslig.");
    }else{
        if(dryerProgram != 3){
            if (dryerCicle == 0){
                tft.print("Inicio");
            }else if (dryerCicle == 1){
                tft.print("Pre-Aqu.");
            }else if (dryerCicle == 2){
                tft.print("Secagem");
            }else if (dryerCicle == 3){
                tft.print("Fim Sec.");
            }else if (dryerCicle == 4){
                tft.print("Resfr.");
            }
        }else {
            tft.print("Aquec.");
        }
    }
    
    tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_GREEN);
    tft.drawBitmap(5, 126, fan, 32, 32, ST7735_GREEN);
    

    
  
} 

void change_data(){
    if (sleeping){
            sleeping = false;
            digitalWrite(TFT_BACKLIGHT, HIGH);
            loopDelay_sleep = millis();
            play_start_sound();
            show_display();
    }else{
    
        tft.drawBitmap(2, 0, vin3, 120, 25, ST7735_RED);
        tft.setTextColor(ST7735_YELLOW);
        if(wifi_connected){
            tft.fillRect(2,20, 30,30, ST7735_BLACK);
            tft.drawBitmap(2, 20, wifi, 32, 32, ST7735_GREEN);
        } else{
            tft.fillRect(2,20, 30,30, ST7735_BLACK);
            tft.drawBitmap(2, 20, wifi, 32, 32, ST7735_YELLOW);
        }
        // tft.drawBitmap(50, 15, dryer, 32, 32, ST7735_YELLOW);
        if (mqtt_connected){
            tft.drawBitmap(47, 20, homeassistant, 32, 32, ST7735_GREEN);
        }else{
            tft.drawBitmap(47, 20, homeassistant, 32, 32, ST7735_ORANGE);
        }
          
        if (doorState){
            tft.fillRect(90,20, 30,30, ST7735_BLACK);
            tft.drawBitmap(90, 20, doorclosed, 32, 32, ST7735_GREEN);
        }else{
            tft.fillRect(90,20, 30,30, ST7735_BLACK);
            tft.drawBitmap(90, 20, bxsdooropen, 32, 32, ST7735_ORANGE);
        }

        tft.setCursor(42,78);
        tft.fillRect(42,59, 80,33, ST7735_BLACK);
        if (dryerProgram == 0){
            tft.fillRect(5,60, 30,30, ST7735_BLACK);
            tft.drawBitmap(5, 58, dryer, 32, 32, ST7735_RED);
            tft.print("Normal");
        }else if (dryerProgram == 1){
            tft.fillRect(5,60, 30,30, ST7735_BLACK);
            tft.drawBitmap(5, 58, dryer, 32, 32, ST7735_ORANGE);
            tft.print("Medio");
        }else if (dryerProgram == 2){
            tft.fillRect(5,60, 30,30, ST7735_BLACK);
            tft.drawBitmap(5, 58, dryer, 32, 32, ST7735_YELLOW);
            tft.print("Leve");
        }else if (dryerProgram == 3){
            tft.fillRect(5,59, 30,30, ST7735_BLACK);
            tft.drawBitmap(5, 58, dryer, 32, 32, ST7735_WHITE);
            tft.print("Aquecer");
        }

        // tft.drawRoundRect(2,94,125,64, 5, ST7735_YELLOW);
        tft.fillRect(42,110, 85,30, ST7735_BLACK);
        tft.setCursor(42,131);
        if (!dryerOn){ 
            tft.fillRect(5,98, 30,30, ST7735_BLACK);
            tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_WHITE);
            tft.fillRect(5,126, 30,30, ST7735_BLACK);
            tft.drawBitmap(5, 126, fan, 32, 32, ST7735_WHITE);
            tft.print("Deslig.");
        }else{
            if(dryerProgram != 3){
                if (dryerCicle == 0){
                   tft.fillRect(5,98, 30,30, ST7735_BLACK);
                   tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_GREEN);
                   tft.fillRect(5,126, 30,30, ST7735_BLACK);
                   tft.drawBitmap(5, 126, fan, 32, 32, ST7735_GREEN);
                   tft.print("Inicio");
                }else if (dryerCicle == 1){
                    tft.fillRect(5,98, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_ORANGE);
                    tft.fillRect(5,126, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 126, fan, 32, 32, ST7735_CYAN);
                    tft.print("Pre-Aqu.");
                }else if (dryerCicle == 2){
                    tft.fillRect(5,98, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_RED);
                    tft.fillRect(5,126, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 126, fan, 32, 32, ST7735_CYAN);
                    tft.print("Secagem");
                }else if (dryerCicle == 3){
                    tft.fillRect(5,98, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_ORANGE);
                    tft.fillRect(5,126, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 126, fan, 32, 32, ST7735_CYAN);
                    tft.print("Fim Sec.");
                }else if (dryerCicle == 4){
                    tft.fillRect(5,98, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_WHITE);
                    tft.fillRect(5,126, 30,30, ST7735_BLACK);
                    tft.drawBitmap(5, 126, fan, 32, 32, ST7735_CYAN);
                    tft.print("Resfr.");
                }
            }else {
                tft.fillRect(5,98, 30,30, ST7735_BLACK);
                tft.drawBitmap(5, 98, heatwave, 32, 32, ST7735_YELLOW);
                tft.fillRect(5,126, 30,30, ST7735_BLACK);
                tft.drawBitmap(5, 126, fan, 32, 32, ST7735_CYAN);
                tft.print("Aquec..");
            }
        }    
   
        
    
    }
    
}  





void change_door(){
    tft.fillRect(90,20, 110,35, ST7735_BLACK);  
    if (doorState){
        tft.drawBitmap(90, 20, doorclosed, 32, 32, ST7735_GREEN);
    }else{
        tft.drawBitmap(90, 20, bxsdooropen, 32, 32, ST7735_ORANGE);
    }
}
