/*
  FILENAME:       WalkieTalkie.ino
  AUTHOR:         Orlando S. Hoilett
  CONTACT:        orlandohoilett@gmail.com
  VERSION:        2.0
  
  
  INSTRUCTABLE:   http://www.instructables.com/member/ohoilett/
  GITHUB:         https://github.com/hoilett/Arduino-Walkie-Talkie-Instructable.git
  HACKADAY:       https://hackaday.io/project/8598-wristwatch-walkie-talkie
  
  
  AFFILIATIONS:
  Institute for Innovation, Creative Exploration, and Personal
      Development (IICEPD), West Lafayette, IN
  Calvary Engineering, USA
      A group electronics enthusiasts 
      "let's have a little fun, and save teh world while we are at it"


  UPDATES:
  Version 0.0.0
  2015/11/23:2100>
            Instructables release version.
  Version 2.0
  2016/05/30:2322>
            Includes buzzer for playing Mighty Morphin Power Rangers
            Communicator notification when called.


  DESCRIPTION
  Simple example for creating a walkie talkie by sending audio
  using the nRF24L01 RF transceiver module. Also uses a call
  button which plays the Mighty Morphin Power Rangers Communicator
  notification when pressed for under a specified amount of time.
  Otherwise, audio is transmitted when Analog In 0 goes LOW. This
  is handled by the rfAudio library.

  
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
#include "printf.h"
#include <MusicPlayer.h>

RF24 radio(7,8);    // Set radio up using pins 7 (CE) 8 (CS)
RF24Audio rfAudio(radio,0); // Set up the audio using the radio, and set to radio number 0
const uint8_t callDetector = 3;

MusicPlayer myPlayer = MusicPlayer(melodyPin);
int PwrRngrs[] = { E7,E7,D7,E7,G7,E7 };
unsigned int duration[] = { E, E, S, E, E, E  };
int melodyPin = 5;
const uint8_t melodyLength = 6;
unsigned long notify = 500;


void setup() {      
  Serial.begin(115200);
  
  printf_begin();
  radio.begin();
  radio.printDetails();
  rfAudio.begin();

  pinMode(callDetector, INPUT);

  //sets the default state for each module to recevie
  //the RFAudio library has automatically handles, rfAudio.transmit()
  //when the Analog In 0 goes LOW so no need to add code to activate
  //rfAudio.transmit();
  rfAudio.receive();
}


void loop()
{
  unsigned long strt = 0;
  unsigned long fin = 0;

  //The code waits while the call detector pin is LOW (no audio is
  //being transmitted. When a call is detected, the time marked
  //and the code waits until transmission is complete again. If
  //the time for transmission is under 500 ms, then the call
  //button was pressed as opposed to the talk button and the
  //sender wanted to issue a call notification. The Mighty Morphin
  //Power Rangers Communicator tone is played to indicate the
  //notification.
  while(!digitalRead(callDetector));
  strt = millis();
  while(digitalRead(callDetector));
  fin = millis();
  if ((fin - strt) < notify) myPlayer.playMelody(PwrRngrs, duration, melodyLength);

//  while(((PIND>>3)&0x01) == 0)
//  {
//    Serial.println("hello world");
//  }
//  
//  if(digitalRead(3))
//  {
//    strt = millis();
//    flag = true;
//  }
//
//  while(digitalRead(3));
//  
//  if(flag) fin = millis();
//  
//  if (((fin - strt) < 500) && flag)
//  {
//    myPlayer.playMelody(melody, duration, melodyLength);
//    flag = false;
//  }
}
