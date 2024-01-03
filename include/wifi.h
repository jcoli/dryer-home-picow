/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/

#ifndef __WIFI_H__
#define __WIFI_H__

void printMacAddress(byte mac[]);
void printCurrentNet();
void printWifiData();
void printEncryptionType(int thisType);
void listNetworks();
void wifi_begin();
void checkStatus();



#endif
