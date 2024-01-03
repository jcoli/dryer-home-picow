/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/
#include <Arduino.h>
#include <ArduinoHA.h> //MQTT
#include <WiFi.h>
// #include <WiFiClient.h>



#include "defines.h"
#include "io_defines.h"
#include "wifi.h"
#include "display.h"
#include "program.h"
#include "sounds.h"

void mqtt_begin();
void mqtt_loop();
void mqtt_update();
void onSelectProgram(int8_t index, HASelect* sender);
void on_switch_on(bool state, HASwitch* sender);


WiFiClient wifiClient;

// extern byte mac[6];
extern bool wifi_connected;


extern float tempIn;
extern float tempOut;
extern float tempCpu;
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
extern bool mqtt_connected;

extern unsigned long loopDelay_cooling;
extern unsigned long loopDelay_door;
extern unsigned long loopDelay_start;
extern unsigned long loopDelay_on;
extern unsigned long loopDelay_sleep;



byte unique[6];

byte mac1[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};

HADevice device(mac1, sizeof(mac1));
HAMqtt mqtt(wifiClient, device, 20);
HASwitch switch_on("switch_dryer_on_20");
HASelect program_select("dryer_program_select_20");
HASensor dryer_cycle("dryer_cycle_20");
HASensorNumber temp_in_sensor("dryer_temp_in_20", HASensorNumber::PrecisionP1);
HASensorNumber temp_out_sensor("dryer_temp_out_20", HASensorNumber::PrecisionP1);
HASensorNumber delta_sensor("dryer_delta_temp_20", HASensorNumber::PrecisionP1);
HASensorNumber temp_cpu_sensor("dryer_temp_cpu_20", HASensorNumber::PrecisionP1);
HABinarySensor fan_enable("fan_dryer_enable_20");
HABinarySensor state_sensor("on_dryer_state_20");
HABinarySensor door_state("door_dryer_state_20");
HABinarySensor heater_state("heater_dryer_state_20");
HABinarySensor heater_enable("heater_dryer_enable_20");
HABinarySensor fan_state("fan_dryer_state_20");





void mqtt_begin(){
    Serial.println("mqtt");
    // device.enableSharedAvailability();
    device.enableLastWill();
    // mqtt.onMessage(callback_mqtt);
    Serial.println("mqtt1z");

    unique[0] = 18;
    unique[1] = 96;
    unique[2] = 87;
    unique[3] = 85;
    unique[4] = 54;
    unique[5] = 89;

    Serial.println("mqtt1");
  
    // WiFi.macAddress(mac);
    
    device.setUniqueId(unique, 6);
    device.setName("Clothes_Dryer_20");
    device.setSoftwareVersion("2.1.1");
    device.setManufacturer("Tecnocoli");
    device.setModel("12A");
    device.enableSharedAvailability();
    
    switch_on.setName("Liga");
    switch_on.onCommand(on_switch_on);
    switch_on.setIcon("mdi:tumble-dryer"); 
    switch_on.setState(dryerOn);  
    

    door_state.setName("Porta");
    door_state.setDeviceClass("door");
    door_state.setIcon("mdi:door");
    door_state.setState(!doorState);
    door_state.setCurrentState(doorState);
    
    fan_state.setName("Ventilador Acionado");
    fan_state.setDeviceClass("running");
    fan_state.setIcon("mdi:fan");
    fan_state.setState(fanState);
    fan_state.setCurrentState(fanState);

    fan_enable.setName("Ventilador Habilitado");
    fan_enable.setDeviceClass("lock");
    fan_enable.setIcon("mdi:fan");
    fan_enable.setState(fanEnable);
    fan_enable.setCurrentState(fanEnable);

    heater_state.setName("Aquecedor Acionado");
    heater_state.setDeviceClass("running");
    heater_state.setIcon("mdi:heat-wave");
    heater_state.setState(heaterState);
    heater_state.setCurrentState(heaterState);

    heater_enable.setName("Aquecedor Habilitado");
    heater_enable.setDeviceClass("lock");
    heater_enable.setIcon("mdi:heat-wave");
    heater_enable.setState(heaterEnable);
    heater_enable.setCurrentState(heaterEnable);

    state_sensor.setName("Estado Secadora");
    state_sensor.setDeviceClass("power");
    state_sensor.setIcon("mdi:tumble-dryer");
    state_sensor.setState(dryerState);
    state_sensor.setCurrentState(dryerState);

    temp_in_sensor.setName("Temperatura IN");
    temp_in_sensor.setUnitOfMeasurement("°C");
    temp_in_sensor.setDeviceClass("temperature");
    temp_in_sensor.setValue(tempIn);
    temp_in_sensor.setCurrentValue(tempOut);

    temp_out_sensor.setName("Temperatura OUT");
    temp_out_sensor.setUnitOfMeasurement("°C");
    temp_out_sensor.setDeviceClass("temperature");
    temp_out_sensor.setValue(tempOut);
    temp_out_sensor.setCurrentValue(tempOut);

    temp_cpu_sensor.setName("Temperatura CPU");
    temp_cpu_sensor.setUnitOfMeasurement("°C");
    temp_cpu_sensor.setDeviceClass("temperature");
    temp_cpu_sensor.setValue(tempCpu);
    temp_cpu_sensor.setCurrentValue(tempOut);
    
    delta_sensor.setName("Temperatura Delta");
    delta_sensor.setUnitOfMeasurement("°C");
    delta_sensor.setDeviceClass("temperature");
    delta_sensor.setValue(deltaTemp);
    delta_sensor.setCurrentValue(deltaTemp);
    

    dryer_cycle.setName("Ciclo Secadora");
    dryer_cycle.setIcon("mdi:tumble-dryer");
    dryer_cycle.setValue("Desligado");
    
    program_select.setOptions("Normal;Medio;Leve;Aquecer");
    program_select.onCommand(onSelectProgram);
    program_select.setName("Programa");
    program_select.setIcon("mdi:curtains-closed");
    program_select.setState(dryerProgram);
    program_select.setCurrentState(dryerProgram);
    program_select.setOptimistic(false);
    program_select.setRetain(false);

    Serial.println("mqtt2");
    if (wifi_connected){
        if (mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD) == true){
            Serial.println("Connected to MQTT broker");
          } else {
            Serial.println("Could not connect to MQTT broker");
        }
        // Serial.print("Broker: ");
        // Serial.println(status);
        delay(500);
    } 
    
    // if (mqtt.isConnected()){
    //   Serial.println("mqtt 3");
    // }else{
    //   Serial.println("mqtt 3a");
    // }
    
    


  
}

void mqtt_loop(){
  if (mqtt.isConnected()){
    // Serial.println("Mqtt Loop");
    mqtt_connected = true;
    mqtt.loop();
  }else{
    mqtt_connected = false;
    int status=mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
    // Serial.print("Broker: ");
    // Serial.println(status);
    // Serial.println("Mqtt Reconect");
    mqtt.loop();
  }
  // if (mqtt.isConnected()){
  //     Serial.println("mqtt 3");
  // }else{
  //     Serial.println("mqtt 3a");
  // }
  
  // Serial.println("Mqtt Loop");
}

void mqtt_update(){
  // if (mqtt.isConnected()){
  //     // Serial.println("mqtt 30");
  //   }else{
      // Serial.println("mqtt 30a");
      // mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
    // } 
  tempCpu = analogReadTemp();

  temp_in_sensor.setValue(tempIn);
  
  temp_out_sensor.setValue(tempOut);

  temp_cpu_sensor.setValue(tempCpu);

  delta_sensor.setValue(deltaTemp);
  
  state_sensor.setState(dryerState);
 
  door_state.setState(!doorState);
 
  fan_state.setState(fanState);

  fan_enable.setState(fanEnable);
  
  heater_state.setState(heaterState);
 
  heater_enable.setState(heaterEnable);
 
  
  switch (dryerCicle){
    case 0:
            dryer_cycle.setValue("Desligado");
            break;
        case 1:
            dryer_cycle.setValue("Pre_Aquecer");
            break;
        case 2:
            dryer_cycle.setValue("Secagem");
            break;
        case 3:
            dryer_cycle.setValue("Fim Secagem");
            break;
        case 4:
            dryer_cycle.setValue("Resfriando");
            break;
        default:
            // unknown option
            return;

  }
  program_select.setState(dryerProgram);
  // program_select.setCurrentState(dryerProgram);
  switch_on.setState(dryerOn);
  Serial.println("Mqtt Update");
   
}

void on_switch_on(bool state, HASwitch* sender){
    Serial.println("Switch On");
    if (sleeping){
            sleeping = false;
            digitalWrite(TFT_BACKLIGHT, HIGH);
            play_start_sound();
            loopDelay_sleep = millis();
    }
    dryerOn = (state ? HIGH : LOW);
    sender->setState(state);
    change_data();
    if (dryerOn){
      Serial.println("Dryer On");
      program_begin();
      delay(500);
      program_controll();;
    }else{
      Serial.println("Dryer Off");
      program_controll();;
  }
}

void onSelectProgram(int8_t index, HASelect* sender){
  if (!dryerOn){
      switch (index) {
        case 0:
            dryerProgram = 0;
            Serial.println("Normal");
            break;
        case 1:
            dryerProgram = 1;
            Serial.println("Medio");
            break;
        case 2:
            dryerProgram = 2;
            Serial.println("Leve");
            break;
        case 3:
            dryerProgram = 3;
            Serial.println("Aquecer");
            break;
        default:
            // unknown option
            return;
        }
        sender->setState(index);
        change_data();
  }else{
    // delay(200);
    Serial.println("not select");
    // program_select.setState(dryerProgram);
    // program_select.setCurrentState(dryerProgram);
   
  }
}



