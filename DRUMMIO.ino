/*
  " HELLO DRUM " Ver.1.0

  Created June 28, 2017
  by Ryo Kosaka

 Mod
 ADD TFT 1,44 INCH DISPLAY
 ADD SD SUPPORT
 ADD EEPROM SUPPORT
 ADD 32 NEW ANALOG INPUTS WITH 74HC4067 
 NEW LIBRARIES USE
   <SD.h> 
   <SPI.h>
   <Adafruit_GFX.h>
   <TFT_ILI9163C.h>
   <Wire.h> 
 
 " DRUMMIO "  Ver.1.1
  Created Jul 26, 2017
  By Bipolab Engineering
  Buenos Aires, Argentina
*/

#include <MIDI.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <Wire.h>    
 
#define disk1 0x50    //Address of 24LC256 eeprom chip
 
MIDI_CREATE_DEFAULT_INSTANCE();


///////////////////////////////////   INITIAL VALUE   ////////////////////////////////////////////

//{threshold, sensitivity, note, flag, velocity, peakValue} 
//*** NOTICE *** peakValue and velocity must be zero.
//*** NOTICE *** Except for the notes, the hi-hat should have the same value for open and close.

int SNARE[6]       = {150, 958, 38, 3, 0, 0};
int SNARE_RIM[6]   = {5, 500, 37, 3 , 0, 0}; 
int HIHAT[6]       = {100, 800, 46, 1, 0, 0};     
int HIHAT_CLOSE[6] = {100, 800, 42, 1, 0, 0};
int KICK[6]        = {200, 700, 36, 1, 0, 0};
int HIHAT_PEDAL[4] = {600, 0, 44, 0};
int SETTING[4]     = {4, 500 ,100 ,1};  //{scantime, snare/rim, pedalVelocity , masktime}

///////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// pin definition for the Uno ////////////////////////////////////
#define __CS 10
#define __DC 8

int  button_set = 3;
int  button_up = 4;
int  button_down = 5;
int  button_next = 6;
int  button_back = 7;

///////////////////////////////////// Color definitions //////////////////////////////////////////
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF
/////////////////////////////////////////////////////////////////////////////////////////////////


// create an TFT LCD instance

TFT_ILI9163C display = TFT_ILI9163C(__CS, __DC);

boolean snareFlag = false;
boolean hihatFlag = false;
boolean kickFlag = false;
boolean pedalFlag = false;
boolean buttonState = true;
boolean buttonState_set = true;
int UPDOWN = 0;
int NEXTBACK = 0;

////////////////////////////// char array to print to the screen
char sensorPrintout[4];


void setup() {
  Wire.begin(); 

/////////////////////////////////////START DISPLAY//////////////////////////////////////////////
  display.begin();
  display.setRotation(1);
  display.clearScreen();
  display.setCursor(5,64);
  display.setTextSize(2);
  display.print("Arc Drums");
  display.setTextSize(1);
  delay(1000);
////////////////////////////////////////////////////////////////////////////////////////////
   
MIDI.begin(0); // INICIALIZAR MIDI EN CANAL OMNI
   
   pinMode(button_set, INPUT_PULLUP);
   pinMode(button_up,  INPUT_PULLUP);
   pinMode(button_down, INPUT_PULLUP);
   pinMode(button_next, INPUT_PULLUP);
   pinMode(button_back, INPUT_PULLUP);
   

  // clear the screen with a black background
  display.setCursor(20,20);
 // display.drawPixel(display.width()/2, display.height()/2, GREEN);


}

void loop() {
  
 button_set  = digitalRead(3);
 button_up   = digitalRead(4);
 button_down = digitalRead(5);
 button_next = digitalRead(6);
 button_back = digitalRead(7);
 
 // Read the value of the sensor on A0
  String sensorVal01 = String(analogRead(A0));
  String sensorVal02 = String(analogRead(A1));
  String sensorVal03 = String(analogRead(A2));
  String sensorVal04 = String(analogRead(A3));
  String sensorVal05 = String(analogRead(A4));

  // convert the reading to a char array
  sensorVal01.toCharArray(sensorPrintout, 4);
  // convert the reading to a char array
  sensorVal02.toCharArray(sensorPrintout, 4);
  // convert the reading to a char array
  sensorVal03.toCharArray(sensorPrintout, 4);
  // convert the reading to a char array
  sensorVal04.toCharArray(sensorPrintout, 4);
  // convert the reading to a char array
  sensorVal05.toCharArray(sensorPrintout, 4);
  /////////////////////////////////// CIRCUIT ///////////////////////////////////////

  
  int piezoValue_0 = analogRead(A0); //snare head
  int piezoValue_1 = analogRead(A1); //snare rim
  int piezoValue_2 = analogRead(A2); //hi-hat
  int piezoValue_3 = analogRead(A3); //kick drum
  int fsrValue_0   = analogRead(11); //hi-hat pedal
 

  ////////////////////////////////// EDIT MODE ////////////////////////////////////

  int UP[4] = {5, 50, 1,1};  //{threshold, sensitivity, note, flag}
  int UP_ADVANCE[4] = {1, 50, 1,1};  //{scantime, rim / head, pedal velocity ,masktime}

  char* instrument[] = {
    "SNARE HEAD",  "SNARE RIM", "HIHAT OPEN",
    "HIHAT CLOSE", "HIHAT PEDAL", "KICK", "ADVANCED SETTING"
  };

  char* setting[] = {
    "THRESHOLD",  "SENSITIVITY", "NOTE", "FLAG"
  };

  char* setting_ADVANCE[] = {
    "SCAN TIME", "HEAD / RIM ","PEDAL VELO", "MASK TIME"
  };

  if (UPDOWN < 0) {
    UPDOWN = 15;
  }

  if (UPDOWN > 15) {
    UPDOWN = 0;
  }

  if (NEXTBACK < 0) {
    NEXTBACK = 3;
  }

  if (NEXTBACK > 3) {
    NEXTBACK = 0;
  }

  ////////////////////////////// EDIT BUTTON ////////////////////////////////

  if (button_set == LOW && buttonState == true && buttonState_set == true) {
  display.clearScreen(); 
  display.setCursor(0,50);
  display.setTextSize(2);
    // write the text to the top left corner of the screen
    display.print("EDIT MODE");
    
    buttonState = false;
    buttonState_set = false;
    delay(500);
  }

  if (button_set == LOW && buttonState == true && buttonState_set == false) {
  display.clearScreen(); 
  display.setCursor(0,50);
    // write the text to the top left corner of the screen
    display.print("EDIT DONE");
    
    buttonState = false;
    buttonState_set = true;
    delay(500);
  }

  if (button_up == LOW && buttonState == true && buttonState_set == false) {

    switch (UPDOWN) {
      case 0:
        SNARE[NEXTBACK] = SNARE[NEXTBACK] + UP[NEXTBACK];
        break;

      case 1:
        SNARE_RIM[NEXTBACK] = SNARE_RIM[NEXTBACK] + UP[NEXTBACK];
        break;

      case 2:
        switch (NEXTBACK) {
          case 2:
            HIHAT[NEXTBACK] = HIHAT[NEXTBACK] + UP[NEXTBACK];
            break;

          default:
            HIHAT_CLOSE[NEXTBACK] = HIHAT_CLOSE[NEXTBACK] + UP[NEXTBACK];
            HIHAT[NEXTBACK] = HIHAT[NEXTBACK] + UP[NEXTBACK];
        }
        break;

      case 3:
        switch (NEXTBACK) {
          case 2:
            HIHAT_CLOSE[NEXTBACK] = HIHAT_CLOSE[NEXTBACK] + UP[NEXTBACK];
            break;

          default:
            HIHAT_CLOSE[NEXTBACK] = HIHAT_CLOSE[NEXTBACK] + UP[NEXTBACK];
            HIHAT[NEXTBACK] = HIHAT[NEXTBACK] + UP[NEXTBACK];
        }
        break;

      case 4:
      switch (NEXTBACK) {
          case 0:
            HIHAT_PEDAL[NEXTBACK] = HIHAT_PEDAL[NEXTBACK] + UP[NEXTBACK];
            break;

          case 2:
            HIHAT_PEDAL[NEXTBACK] = HIHAT_PEDAL[NEXTBACK] + UP[NEXTBACK];
          break;
        }
        break;

      case 5:
        KICK[NEXTBACK] = KICK[NEXTBACK] + UP[NEXTBACK];
        break;

      case 6:
        SETTING[NEXTBACK] = SETTING[NEXTBACK] + UP_ADVANCE[NEXTBACK];
        break;
    }
    buttonState = false;
    delay(30);
  }

  if (button_down == LOW && buttonState == true && buttonState_set == false) {

    switch (UPDOWN) {
      case 0:
        SNARE[NEXTBACK] = SNARE[NEXTBACK] - UP[NEXTBACK];
        break;

      case 1:
        SNARE_RIM[NEXTBACK] = SNARE_RIM[NEXTBACK] - UP[NEXTBACK];
        break;

      case 2:
        switch (NEXTBACK) {
          case 2:
            HIHAT[NEXTBACK] = HIHAT[NEXTBACK] - UP[NEXTBACK];
            break;

          default:
            HIHAT_CLOSE[NEXTBACK] = HIHAT_CLOSE[NEXTBACK] - UP[NEXTBACK];
            HIHAT[NEXTBACK] = HIHAT[NEXTBACK] - UP[NEXTBACK];
        }
        break;

      case 3:
        switch (NEXTBACK) {
          case 2:
            HIHAT_CLOSE[NEXTBACK] = HIHAT_CLOSE[NEXTBACK] - UP[NEXTBACK];
            break;

          default:
            HIHAT_CLOSE[NEXTBACK] = HIHAT_CLOSE[NEXTBACK] - UP[NEXTBACK];
            HIHAT[NEXTBACK] = HIHAT[NEXTBACK] - UP[NEXTBACK];
        }
        break;

      case 4:
              switch (NEXTBACK) {
          case 0:
            HIHAT_PEDAL[NEXTBACK] = HIHAT_PEDAL[NEXTBACK] - UP[NEXTBACK];
            break;

          case 2:
            HIHAT_PEDAL[NEXTBACK] = HIHAT_PEDAL[NEXTBACK] - UP[NEXTBACK];
          break;
        }
        break;

      case 5:
        KICK[NEXTBACK] = KICK[NEXTBACK] - UP[NEXTBACK];
        break;

      case 6:
        SETTING[NEXTBACK] = SETTING[NEXTBACK] - UP_ADVANCE[NEXTBACK];
        break;
    }
    buttonState = false;
    delay(30);
  }

  ///////////////////////////// UP DOWN NEXT BACK ////////////////////////////////

  if (button_up == LOW && buttonState == true && buttonState_set == true) {
    UPDOWN = ++UPDOWN;
    buttonState = false;
    delay(30);
  }

  if (button_down == LOW && buttonState == true && buttonState_set == true) {
    UPDOWN = --UPDOWN;
    buttonState = false;
    delay(30);
  }

  if (button_next == LOW && buttonState == true && buttonState_set == true) {
    NEXTBACK = ++NEXTBACK;
    buttonState = false;
    delay(30);
  }

  if (button_back == LOW && buttonState == true && buttonState_set == true) {
    NEXTBACK = --NEXTBACK;
    buttonState = false;
    delay(30);
  }

  if (buttonState == false && button_up == HIGH && button_down == HIGH && button_next == HIGH && button_back == HIGH && button_set == HIGH) {
    switch (UPDOWN) {
      case 0:
        display.clearScreen();
        display.setTextSize(1);
        // write the text to the top left corner of the screen
        
        display.setCursor(0,0);
        display.println(instrument[UPDOWN]);
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SNARE[NEXTBACK]);
       
        break;

      case 1:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println(instrument[UPDOWN]);
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SNARE_RIM[NEXTBACK]);
        break;

      case 2:
         display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println(instrument[UPDOWN]);
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(HIHAT[NEXTBACK]);
        break;

      case 3:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println(instrument[UPDOWN]);
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(HIHAT_CLOSE[NEXTBACK]);
        break;

      case 4:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println(instrument[UPDOWN]);
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(HIHAT_PEDAL[NEXTBACK]);
        break;

      case 5:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(30,0);
        display.println(instrument[UPDOWN]);
        
        display.setCursor(0,10);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,10);
        display.println(KICK[NEXTBACK]);
        break;

      case 6:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 01");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;
      
      case 7:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 02");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;
        
     case 8:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 03");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;
      case 9:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 04");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;

     case 10:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 05");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;

     case 11:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 06");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;
   
   case 12:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 07");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;

     case 13:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 08");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;

     case 14:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 09");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;
     case 15:
        display.clearScreen();
        display.setTextSize(1);
        
        // write the text to the top left corner of the screen
        display.setCursor(0,0);
        display.println("TACA 10");
        
        display.setCursor(0,20);
        display.println(setting[NEXTBACK]);
        
        display.setCursor(110,20);
        display.println(SETTING[NEXTBACK]);
        break;
        
    }
    

    
    buttonState = true;
  }

  ////////////////////////////////// SNARE ///////////////////////////////////

  if (piezoValue_0 > SNARE[0] && snareFlag == false) {

    for (int i = 0; i < SETTING[0]; i++) {
      int peak0 = analogRead(A0);
      int peak1 = analogRead(A1);
      delay(1);

      if (peak0 > SNARE[4]) {
        SNARE[4] = peak0;
      }

      if (peak1 > SNARE_RIM[4]) {
        SNARE_RIM[4] = peak1;
      }
    }

    SNARE[5] = SNARE[4];
    SNARE_RIM[5] = SNARE_RIM[4];
    SNARE[4] = map(SNARE[4], SNARE[0], SNARE[1], 0, 127);
    SNARE_RIM[4] = map(SNARE_RIM[4], SNARE_RIM[0], SNARE_RIM[1], 0, 127);
    SNARE[4] = ( SNARE[4] *  SNARE[4]) / 127; // Curve setting
    //SNARE_RIM[4] = (SNARE_RIM[4] * SNARE_RIM[4]) / 127;

    if (SNARE[4] <= 1) {
      SNARE[4] = 1;
    }

    if (SNARE[4] > 127) {
      SNARE[4] = 127;
    }

    if (SNARE_RIM[4] <= 0) {
      SNARE_RIM[4] = 0;
    }

    if (SNARE_RIM[4] > 127) {
      SNARE_RIM[4] = 127;
    }

    if (SNARE_RIM[5] > SETTING[1]) {
      MIDI.sendNoteOn(SNARE_RIM[2], SNARE_RIM[4], 1);   //(note, velocity, channel)
      MIDI.sendNoteOn(SNARE_RIM[2], 0, 1);

        display.clearScreen();
        display.setTextColor(YELLOW);  
        display.setTextSize(1);
        display.setCursor(0,20);
        display.print("SNARE");
       // display.setCursor(0,40);
        
      
      snareFlag = true;
    }

    //else if (SNARE[5] > SNARE_RIM[5])
    else {
      MIDI.sendNoteOn(SNARE[2], SNARE[4], 1);   //(note, velocity, channel)
      MIDI.sendNoteOn(SNARE[2], 0, 1);
        display.clearScreen();
        
        display.println("SNARE HEAD");
        display.setCursor(0, 20);
        display.println(SNARE[4]);
        display.setCursor(0, 40);
        display.println(SNARE_RIM[5]);
      snareFlag = true;
    }
  }

  //////////////////////////////// HI-HAT /////////////////////////////////////

  if (piezoValue_2 > HIHAT[0] && hihatFlag == false) {

    for (int i = 0; i < SETTING[0]; i++) {
      int peak2 = analogRead(A2);
      delay(1);

      if (peak2 > HIHAT[4]) {
        HIHAT[4] = peak2;
      }
    }

    HIHAT[5] = HIHAT[4];
    HIHAT[4] = map(HIHAT[4], HIHAT[0], HIHAT[1], 0, 127);
    HIHAT[4] = (HIHAT[4] * HIHAT[4]) / 127;

    if (HIHAT[4] <= 1) {
      HIHAT[4] = 1;
    }

    if (HIHAT[4] > 127) {
      HIHAT[4] = 127;
    }

    if (fsrValue_0 < HIHAT_PEDAL[0]) {
      MIDI.sendNoteOn(HIHAT[2], HIHAT[4], 1);
      MIDI.sendNoteOn(HIHAT[2], 0, 1);
       display.clearScreen();
       display.setCursor(0, 20);
       display.println("HIHAT OPEN");
       display.setCursor(0, 40);
       display.println(HIHAT[4]);
      hihatFlag = true;
    }

    else if (fsrValue_0 >= HIHAT_PEDAL[0]) {
      MIDI.sendNoteOn(HIHAT_CLOSE[2], HIHAT[4], 1);
      MIDI.sendNoteOn(HIHAT_CLOSE[2], 0, 1);
       
       display.clearScreen();
       display.setCursor(0, 20);
       display.println("HIHAT CLOSE");
       display.setCursor(0, 40);
       display.print(HIHAT[4]);
      hihatFlag = true;
    }
  }

  //////////////////////////////////// KICK //////////////////////////////////

  if (piezoValue_3 > KICK[0] && kickFlag == false) {

    for (int i = 0; i < SETTING[0]; i++) {
      int peak3 = analogRead(A3);
      delay(1);

      if (peak3 > KICK[4]) {
        KICK[4] = peak3;
      }
    }

    KICK[5] = KICK[4];
    KICK[4] = map(KICK[4], KICK[0], KICK[1], 0, 127);
    KICK[4] = (KICK[4] * KICK[4]) / 127;

    if (KICK[4] <= 1) {
      KICK[4] = 1;
    }

    if (KICK[4] > 127) {
      KICK[4] = 127;
    }

    MIDI.sendNoteOn(KICK[2], KICK[4], 1);
    MIDI.sendNoteOn(KICK[2], 0, 1);
     display.clearScreen();
     display.setCursor(0, 20);
     display.println("KICK");
     display.setCursor(0, 400);
     display.println(KICK[4]);
    kickFlag = true;
  }

  ///////////////////////////// HIHAT PEDAL ////////////////////////////

  if (fsrValue_0 > HIHAT_PEDAL[0] && pedalFlag == false) {
    MIDI.sendNoteOn(HIHAT_PEDAL[2], SETTING[2], 1);  //(note, velocity, channel)
    MIDI.sendNoteOn(HIHAT_PEDAL[2], 0, 1);
     display.clearScreen();
     display.setCursor(0, 20);
     display.println("HIHAT PEDAL");
     display.setCursor(0, 40);
     display.println(SETTING[2]);
    pedalFlag = true;
  }

  ////////////////////////////// FLAG /////////////////////////////

  if (piezoValue_0 < (SNARE[5] * (0.01 * SNARE[3])) && snareFlag == true) {
    delay(SETTING[3]); //mask time
    int sensorValue0 = analogRead(A0);
    if (sensorValue0 < (SNARE[5] * (0.01 * SNARE[3]))) {
      snareFlag = false;
    }
  }

  if (fsrValue_0 <= HIHAT_PEDAL[0] && piezoValue_2 < (HIHAT[5] * (0.01 * HIHAT[3])) && hihatFlag == true) {
    delay(SETTING[3]);
    int sensorValue1 = analogRead(A2);
    if (sensorValue1 < (HIHAT[5] * (0.01 * HIHAT[3]))) {
      hihatFlag = false;
    }
  }

  if (fsrValue_0 >= HIHAT_PEDAL[0] && piezoValue_2 < (HIHAT[5] *(0.01 * HIHAT[3])) && hihatFlag == true) {
    delay(SETTING[3]);
    int sensorValue2 = analogRead(A2);
    if (sensorValue2 < (HIHAT[5] * (0.01 * HIHAT[3]))) {
      hihatFlag = false;
    }
  }

  if (piezoValue_3 < (KICK[5] * (0.01 * KICK[3])) && kickFlag == true) {
    delay(SETTING[3]);
    int sensorValue3 = analogRead(A3);
    if (piezoValue_3 < (KICK[5] * (0.01 * KICK[3]))) {
      kickFlag = false;
    }
  }

 if ( fsrValue_0 <= HIHAT_PEDAL[0] && pedalFlag == true) {
    pedalFlag = false;
  }
}
