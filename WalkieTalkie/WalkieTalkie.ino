/*
  FILENAME:   walkie talkie basic.ino
  AUTHOR:     Orlando S. Hoilett
  CONTACT:    orlandohoilett@gmail.com
  VERSION:    0.0.0
  WEBSITE:    http://www.instructables.com/member/ohoilett/
  
  
  AFFILIATIONS:
  Institute for Innovation, Creative Exploration, and Personal
      Development, West Lafayette, IN
  Calvary Engineering, USA
      A group electronics enthusiasts 
      "let's have a little fun, and save teh world while we are at it"


  UPDATES:
  Version 0.0.0
  2015/11/23:2100>
            Instructables release version.


  DESCRIPTION
  Simple example for creating a walkie talkie by sending audio
  using the nRF24L01 RF transceiver module.

  
  DISCLAIMER
  This code is in the public domain. Please feel free to modify,
  use, etc however you see fit. But, please give reference to
  original authors as a courtesy to Open Source developers.

  Modified example from RF24 Audio Library by TMRh20. Comments
  from TMRh20 below.
  
*/

/* RF24 Audio Library TMRh20 2014

This sketch is intended to demonstrate the basic functionality of the audio library.

Requirements:
2 Arduinos (Uno,Nano,Mega, etc supported)
2 NRF24LO1 Radio Modules
1 or more input devices (microphone, ipod, etc)
1 or more output devices (speaker, amplifier, etc)

Setup:
1. Change the CE,CS pins below to match your chosen pins (I use 7,8 on 328 boards, and 48,49 on Mega boards)
2. Upload this sketch to two or more devices
3. Send serial commands via the serial monitor to control transmission and volume (volume only affects receiving devices)

Default Pin Selections:
Speaker: pins 9,10 on UNO, Nano,  pins 11,12 on Mega 2560
Input/Microphone: Analog pin A0 on all boards

*/


#include <RF24.h>
#include <SPI.h>
#include <RF24Audio.h>
#include "printf.h"    // General includes for radio and audio lib
#include <MusicPlayer.h>

RF24 radio(7,8);    // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio,0); // Set up the audio using the radio, and set to radio number 0

const uint8_t callDetector = 3;
const uint16_t callThreshold = 20; //20% duty cycle?
//
//
//int melody[] = { E6,E6,D6,E6,G6,E6  };
int melody[] = { E7,E7,D7,E7,G7,E7 };

//unsigned int duration[] = { E, E, E, E, E, E  };
unsigned int duration[] = { E, E, S, E, E, E  };
//unsigned int duration[] = { S, S, S, S, S, S };

int melodyPin = 5;
int melodyLength = 6;

MusicPlayer myPlayer = MusicPlayer(melodyPin);


void setup() {      
  Serial.begin(115200);
  
  printf_begin();
  radio.begin();
  radio.printDetails();
  rfAudio.begin();

  pinMode(callDetector, INPUT);

  //sets interrupt to check for call button
  //attachInterrupt(digitalPinToInterrupt(callDetector), sample, RISING);
  
  //sets the default state for each module to recevie
  rfAudio.receive();
}


//horrible way to use interrupts :)
void sample()
{
  unsigned long time0 = micros();
  unsigned long i = 0;

  while((micros() - time0) < 2000) //2 milliseconds
  {
    if (digitalRead(callDetector)) i++;
    delayMicroseconds(20);
  }

  if (i > callThreshold) myPlayer.playMelody(melody, duration, melodyLength);
  delay(200);
}


void loop()
{
  if(digitalRead(callDetector)) sample();
}
