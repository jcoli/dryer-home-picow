/**
Version: 0a
Tecnocoli - 11/2023
jcoli - Jeferson Coli - jcoli@teccnocoli.com.br
Pipicow - Clothes Dryer
**/
#include <Arduino.h>
#include <ArduinoHA.h> //MQTT

#include "io_defines.h"
#include "defines.h"
#include "display.h"
#include "pitches.h"
#include "input_digital_control.h"


void play_sleep_sound();
void play_start_sound();
void play_door_sound();
void play_finish_sound();

void play_sleep_sound(){
  Serial.println("sound1"); 
  int melody[] = { NOTE_D6, NOTE_D6};
  int noteDurations[] = { 8, 8}; 
  for (int thisNote = 0; thisNote < 2; thisNote++) {   // ergodic all notes
      int noteDuration = 1000 / noteDurations[thisNote]; // calculate the note duration
      tone(BUZZER_PIN , melody[thisNote], noteDuration);   // let speaker sonds
      int pauseBetweenNotes = noteDuration * 1.30;       // set a minimum time between notes
      delay(pauseBetweenNotes);                          // delay for the while
      noTone(BUZZER_PIN );                                 // stop the tone playing:
  }

}

void play_finish_sound(){
    int melody[] = { NOTE_C7, NOTE_C6, NOTE_C7,NOTE_C6,NOTE_C7};
        int noteDurations[] = { 8, 8, 4, 8, 4}; 
        for (int thisNote = 0; thisNote < 5; thisNote++) {   // ergodic all notes
            int noteDuration = 1000 / noteDurations[thisNote]; // calculate the note duration
            tone(BUZZER_PIN , melody[thisNote], noteDuration);   // let speaker sonds
            int pauseBetweenNotes = noteDuration * 1.30;       // set a minimum time between notes
            delay(pauseBetweenNotes);                          // delay for the while
            noTone(BUZZER_PIN );                                 // stop the tone playing:
        }
}

void play_door_sound(){ 
  Serial.println("sound2"); 
  int melody[] = { NOTE_D6, NOTE_D6};
  int noteDurations[] = { 8, 8}; 
  for (int thisNote = 0; thisNote < 2; thisNote++) {   // ergodic all notes
      int noteDuration = 1000 / noteDurations[thisNote]; // calculate the note duration
      tone(BUZZER_PIN , melody[thisNote], noteDuration);   // let speaker sonds
      int pauseBetweenNotes = noteDuration * 1.30;       // set a minimum time between notes
      delay(pauseBetweenNotes);                          // delay for the while
      noTone(BUZZER_PIN );                                 // stop the tone playing:
  }

}

void play_start_sound(){
  Serial.println("sound3"); 
  int melody[] = { NOTE_D6, NOTE_D6};
  int noteDurations[] = { 8, 8}; 
  for (int thisNote = 0; thisNote < 2; thisNote++) {   // ergodic all notes
      int noteDuration = 1000 / noteDurations[thisNote]; // calculate the note duration
      tone(BUZZER_PIN , melody[thisNote], noteDuration);   // let speaker sonds
      int pauseBetweenNotes = noteDuration * 1.30;       // set a minimum time between notes
      delay(pauseBetweenNotes);                          // delay for the while
      noTone(BUZZER_PIN );                                 // stop the tone playing:
  }
  
}

