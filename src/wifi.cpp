/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/
#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
// #include <WiFiClient.h>
#include <ArduinoHA.h> //MQTT

#include "defines.h"
#include "io_defines.h"

// WiFiClient wifiClient;




void printMacAddress(byte mac[]);
void printCurrentNet();
void printWifiData();
void printEncryptionType(int thisType);
void listNetworks();
void wifi_beging();
void checkStatus();

char ssid[] = LOCAL_SSID;        // your network SSID (name)
char pass[] = LOCAL_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

byte mac[6];
byte new_mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};
extern bool wifi_connected;

void wifi_begin(){
  Serial.println("WiFiNINA firmware check.");
  Serial.println();
  

  WiFi.config(LOCAL_IP, GATEWAY, SUBNET);
  WiFi.mode(WIFI_STA);

  // WiFi.begin(ssid, pass);

  // while (WiFi.status() != WL_CONNECTED) {
  //       delay(500);
  //       Serial.print(".");
  //   }


    WiFi.macAddress(mac);
    Serial.print("MAC: ");
    printMacAddress(mac);
    int tent = 0;
    while ((status != WL_CONNECTED) && (tent<30)) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        Serial.println(status);
        // Connect to WPA/WPA2 network:
        status = WiFi.begin(ssid, pass);
        tent++;
        // wait 10 seconds for connection:
        delay(1000);
    }
    delay(2000);
    if (status == WL_CONNECTED){
        Serial.print("You're connected to the network");
        wifi_connected = true;
        
    }
        Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    
    printCurrentNet();
    printWifiData();
  


}

void checkStatus(){
  if (status == WL_CONNECTED){
    wifi_connected = true;
  }else{
    wifi_connected = false;
  }
}

void listNetworks() {
  // scan for nearby networks:
  // Serial.println("** Scan Networks **");

  // int numSsid = WiFi.scanNetworks();
  // Serial.println("** Scan Networks **");
  // if (numSsid == -1) {
  //   Serial.println("Couldn't get a wifi connection");
  //   while (true);
  // }
 
  // // print the list of networks seen:
  // Serial.print("number of available networks:");
  // Serial.println(numSsid);
 
  // // print the network number and name for each network found:
  // for (int thisNet = 0; thisNet < numSsid; thisNet++) {
  //   Serial.print(thisNet);
  //   Serial.print(") ");
  //   Serial.print(WiFi.SSID(thisNet));
  //   Serial.print("\tSignal: ");
  //   Serial.print(WiFi.RSSI(thisNet));
  //   Serial.print(" dBm");
  //   Serial.print("\tEncryption: ");
  //   printEncryptionType(WiFi.encryptionType(thisNet));
  // }
}
 
void printEncryptionType(int thisType) {
  // // read the encryption type and print out the name:
  // switch (thisType) {
  //   case ENC_TYPE_WEP:
  //     Serial.println("WEP");
  //     break;
  //   case ENC_TYPE_TKIP:
  //     Serial.println("WPA");
  //     break;
  //   case ENC_TYPE_CCMP:
  //     Serial.println("WPA2");
  //     break;
  //   case ENC_TYPE_NONE:
  //     Serial.println("None");
  //     break;
  //   case ENC_TYPE_AUTO:
  //     Serial.println("Auto");
  //     break;
  //   case ENC_TYPE_UNKNOWN:
  //   default:
  //     Serial.println("Unknown");
  //     break;
  // }
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // // print the MAC address of the router you're attached to:
  // byte bssid[6];
  // WiFi.BSSID(bssid);
  
  // Serial.print("BSSID: ");
  // printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  // byte encryption = WiFi.encryptionType();
  // Serial.print("Encryption Type:");
  // Serial.println(encryption, HEX);
  // Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

/*
GPIO05	          CS	              PA4
GPIO18	          SCK	              PA5
GPIO23	          MISO	            PA6
GPIO14	          MOSI	            PA7
GPIO33	          BUSY/READY (IRQ)	PA3
EN (reset on LOW)	RST/EN	          PA2
GND	              GND	              GND
VIN	3.3v
*/