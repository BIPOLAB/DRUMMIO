// ============================================================
// Example:     DisplayType - I2C LiquidCrystal 
// ============================================================
// Description:
// This example includes the complete functionality over some
// tabs. All Tabs which are started with "LCDML_display_.."
// generates an output on the display / console / ....
// This example is for the author to test the complete functionality
// ============================================================
// *********************************************************************
// special settings
// *********************************************************************
// enable this line when you are not usigng a standard arduino
// for example when your chip is an ESP or a STM or SAM or something else

//#define _LCDML_cfg_use_ram 

// *********************************************************************
// includes
// *********************************************************************
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  #include <LCDMenuLib2.h>
  #include <hellodrum.h>

//Using MIDI Library. If you want to use USB-MIDI, comment out the next two lines.
//#include <MIDI.h>
//MIDI_CREATE_DEFAULT_INSTANCE();

//Uncomment the next two lines for using USB-MIDI with atmega32u4 or Teensy
#include <USB-MIDI.h>
USBMIDI_CREATE_DEFAULT_INSTANCE();

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 9 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



// *********************************************************************
// LCDML display settings
// *********************************************************************
  // settings for LCD
  #define _LCDML_DISP_cols  16
  #define _LCDML_DISP_rows  2

  #define _LCDML_DISP_cfg_cursor                     0x7E   // cursor Symbol
  #define _LCDML_DISP_cfg_scrollbar                  0      // enable a scrollbar

  // LCD object
  // for i2c there are many different steps for initialization, some are listed here
  // when the rows and cols are not set here, they have to be set in the setup
  //LiquidCrystal_I2C lcd(0x27);  // Set the LCD I2C address
  //LiquidCrystal_I2C lcd(0x27, BACKLIGHT_PIN, POSITIVE);  // Set the LCD I2C address
  LiquidCrystal_I2C lcd(0x27,_LCDML_DISP_cols,_LCDML_DISP_rows);
  
  //LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
  //Define MUX Pins
  HelloDrumMUX_4051 mux(4, 16, 17, 33);
  //HelloDrumMUX_4067 mux(2, 4, 16, 17, 36);

  //Please name your piezo.
  HelloDrum pad1(0); //mux pin
  HelloDrum pad2(1); //mux pin
  HelloDrum pad3(2); //mux pin
  HelloDrum pad4(3); //mux pin
  HelloDrum pad5(4); //mux pin
  HelloDrum pad6(5); //mux pin
  HelloDrum pad7(6); //mux pin
  HelloDrum pad8(7); //mux pin

  const uint8_t scroll_bar[5][8] = {
    {B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001}, // scrollbar top
    {B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001}, // scroll state 1
    {B10001, B10001, B11111, B11111, B10001, B10001, B10001, B10001}, // scroll state 2
    {B10001, B10001, B10001, B10001, B11111, B11111, B10001, B10001}, // scroll state 3
    {B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111}  // scrollbar bottom
  };

// *********************************************************************
// Prototypes
// *********************************************************************
  void lcdml_menu_display();
  void lcdml_menu_clear();
  void lcdml_menu_control();

// *********************************************************************
// Global variables
// *********************************************************************


// *********************************************************************
// Objects
// *********************************************************************
  LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL); // root menu element (do not change)
  LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0
  // LCDML_0_X      => layer 1
  // LCDML_0_X_X    => layer 2
  // LCDML_0_X_X_X  => layer 3
  // LCDML_0_...      => layer ...

  // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)

 //Main Menu Layer 0
  LCDML_add         (0  , LCDML_0         , 1  , "Leds Config"      , NULL);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (1  , LCDML_0         , 2  , "Midi Config"      , mFunc_timer_info);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (2  , LCDML_0         , 3  , "Pads Config"      , NULL);                    // NULL = no menu function
  LCDML_add         (12 , LCDML_0         , 4  , "Drum Map"         , NULL);                    // NULL = no menu function
  LCDML_add         (16 , LCDML_0         , 5  , "Dummy B"          , NULL);                    // NULL = no menu function
  LCDML_add         (17 , LCDML_0         , 6  , "Screensaver"      , mFunc_screensaver);       // this menu function can be found on "LCDML_display_menuFunction" tab
  
  //Main Menu Layer 1
  LCDML_add         (3  , LCDML_0_1       , 1  , "Encoder Led"            , NULL);                    // NULL = no menu function
  LCDML_add         (13 , LCDML_0_4       , 1  , "Go to Root"       , mFunc_goToRootMenu);      // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (14 , LCDML_0_4       , 2  , "Jump to Time info", mFunc_jumpTo_timer_info); // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (15 , LCDML_0_4       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab

  //Main Menu Layer 2
  LCDML_add         (4  , LCDML_0_1_1     , 1  , "P1 dummy"         , mFunc_padEdit);                    // NULL = no menu function
  LCDML_add         (5  , LCDML_0_1_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function
  
  //Main Menu Layer 3
  LCDML_add         (6  , LCDML_0_1_1_2   , 1  , "Red"              , mFunc_ledsEdit);                     // NULL = no menu function
  LCDML_add         (7  , LCDML_0_1_1_2   , 2  , "Blue"             , NULL);                    // NULL = no menu function
  LCDML_add         (8  , LCDML_0_1_1_2   , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (9  , LCDML_0_1_1     , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (10 , LCDML_0_1       , 2  , "Pad Led"        , mFunc_p2);                // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (11 , LCDML_0_1       , 3  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab
  
  
  
  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    17

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);

// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {
    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    
    // serial init; only be needed if serial control is used
    Serial.begin(9600);                // start serial
    Serial.println(F(_LCDML_VERSION)); // only for examples

    // LCD Begin
    lcd.init();
    lcd.backlight();
    //lcd.begin(_LCDML_DISP_cols,_LCDML_DISP_rows);  // some display types needs here the initialization


    // set special chars for scrollbar
    lcd.createChar(0, (uint8_t*)scroll_bar[0]);
    lcd.createChar(1, (uint8_t*)scroll_bar[1]);
    lcd.createChar(2, (uint8_t*)scroll_bar[2]);
    lcd.createChar(3, (uint8_t*)scroll_bar[3]);
    lcd.createChar(4, (uint8_t*)scroll_bar[4]);

    // LCDMenuLib Setup
    LCDML_setup(_LCDML_DISP_cnt);

    // Some settings which can be used

    // Enable Menu Rollover
    LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 seconds
    //LCDML.SCREEN_disable();

    // Some needful methods

    // You can jump to a menu function from anywhere with
    //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name
  }

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    LCDML.loop();
  }
