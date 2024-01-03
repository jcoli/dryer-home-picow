/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* defines */
#define V03_MAX 3.6
#define V05_MAX 5.5
#define V12_MAX 14.5
#define V24_MAX 28.0

static const char version[] = "1.0a";
static const char develop[] =  "jcoli-Tecnocoli";
static const char email_develop[] = "jcoli@tecnocoli.com.br";
static const char processor[] = "PipicoW";

#define TIMER_INTERVAL_0_1S           100L
#define TIMER_INTERVAL_0_3S           300L
#define TIMER_INTERVAL_1S             1000L
#define TIMER_INTERVAL_1_5S           1500L
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0 
#define TIMER_INTERVAL_MS         100
#define HW_TIMER_INTERVAL_MS      50

#define TIMER_0_CICLE_0 400000
#define TIMER_0_CICLE_1 400000
#define TIMER_0_CICLE_2 7200000
#define TIMER_0_CICLE_3 400000 
#define TIMER_0_CICLE_4 1200000 

#define TIMER_1_CICLE_0 400000
#define TIMER_1_CICLE_1 400000
#define TIMER_1_CICLE_2 7200000
#define TIMER_1_CICLE_3 400000 
#define TIMER_1_CICLE_4 1200000 

#define TIMER_2_CICLE_0 400000
#define TIMER_2_CICLE_1 400000
#define TIMER_2_CICLE_2 7200000
#define TIMER_2_CICLE_3 400000 
#define TIMER_2_CICLE_4 1200000 

#define TIMER_3_CICLE_0 400000
#define TIMER_3_CICLE_1 400000
#define TIMER_3_CICLE_2 300000
#define TIMER_3_CICLE_3 200000 
#define TIMER_3_CICLE_4 60000 


#define TIMER_DOOR_BEEP 180000
#define TIMER_DOOR_OPEN 10000 
#define TIMER_DOOR_OPEN_TIMEOUT 600000
#define TIME_OUT 8000000
#define TIMER_SLEEP 600000
#define TIMER_ANALOG 5000
#define TIMER_MQTT 80
#define TIMER_SEND_MQTT 20000
#define TIMER_INPUT 300
#define TIMER_UPDATE 60000

#define DELTA_P0 10.5
#define DELTA_P1 12
#define DELTA_P2 13
#define DELTA_P3 13
#define DELTA_P0_PROP 17.5
#define DELTA_P1_PROP 19.5
#define DELTA_P2_PROP 22.5
#define DELTA_P3_PROP 50.5

#define TEMP_HEATER_0_OFF 75
#define TEMP_HEATER_0_ON 74.9
#define TEMP_HEATER_1_OFF 75
#define TEMP_HEATER_1_ON 74.9
#define TEMP_HEATER_2_OFF 75
#define TEMP_HEATER_2_ON 74.9
#define TEMP_HEATER_3_OFF 73
#define TEMP_HEATER_3_ON 73.9

#define BROKER_ADDR  IPAddress(192,168,61,85)
#define BROKER_USERNAME     "mqtt_user" 
#define BROKER_PASSWORD     "shady55"
#define DEVICE_NAME "Clothes_Dryer_9"

#define LOCAL_IP IPAddress(192,168,61,179)
#define GATEWAY IPAddress(192, 168, 61, 1)
#define SUBNET IPAddress(255, 255, 255, 0)
#define LOCAL_SSID "Area51"
#define LOCAL_PASS "shady6565"


#endif