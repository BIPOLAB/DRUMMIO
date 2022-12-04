/* ===================================================================== *
 *                                                                       *
 * Menu Callback Function                                                *
 *                                                                       *
 * ===================================================================== *
 *
 * EXAMPLE CODE:

// *********************************************************************
void your_function_name(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    //LCDML_UNUSED(param);
    // setup
    // is called only if it is started

    // starts a trigger event for the loop function every 100 milliseconds
    LCDML.FUNC_setLoopInterval(100);

    // uncomment this line when the menu should go back to the last called position
    // this could be a cursor position or the an active menu function
    // GBA means => go back advanced
    //LCDML.FUNC_setGBA() 

    //
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop
    // is called when it is triggered
    // - with LCDML_DISP_triggerMenu( milliseconds )
    // - with every button or event status change

    // uncomment this line when the screensaver should not be called when this function is running
    // reset screensaver timer
    //LCDML.SCREEN_resetTimer();

    // check if any button is pressed (enter, up, down, left, right)
    if(LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // loop end
    // you can here reset some global vars or delete it
    // this function is always called when the functions ends.
    // this means when you are calling a jumpTo ore a goRoot function
    // that this part is called before a function is closed
  }
}


 * ===================================================================== *
 */


//Define control buttons
HelloDrumButton button(7, 9, 8, 6, 5); //(EDIT,UP,DOWN,NEXT,BACK)

// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    lcd.setCursor(0, 0);
    lcd.print(F("To close this"));
    lcd.setCursor(0, 1);
    lcd.print(F("function press"));
    
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if(LCDML.BT_checkAny()) { // check if any button is pressed (enter, up, down, left, right)
      // LCDML_goToMenu stops a running menu function and goes to the menu
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_func_timer_info = 0;  // time counter (global variable)
unsigned long g_timer_1 = 0;    // timer variable (global variable)
void mFunc_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    lcd.print(F("20 sec wait")); // print some content on first row
    g_func_timer_info = 2;       // reset and set timer
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds

    LCDML.TIMER_msReset(g_timer_1);
  }


  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // reset screensaver timer
    LCDML.SCREEN_resetTimer();

    // this function is called every 100 milliseconds

    // this method checks every 1000 milliseconds if it is called
    if(LCDML.TIMER_ms(g_timer_1, 1000)) {
      g_func_timer_info--;                // increment the value every second
      lcd.setCursor(0, 0);                // set cursor pos
      lcd.print(F("  "));
      lcd.setCursor(0, 0);                // set cursor pos
      lcd.print(g_func_timer_info);       // print the time counter value
    }

    // this function can only be ended when quit button is pressed or the time is over
    // check if the function ends normally
    if (g_func_timer_info <= 0)
    {
      // leave this function
      LCDML.FUNC_goBackToMenu();
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}


// *********************************************************************
uint8_t g_button_value = 0; // button value counter (global variable)
void mFunc_p2(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    // print LCD content
    // print LCD content
    lcd.setCursor(0, 0);
    lcd.print(F("press left or up"));
    lcd.setCursor(0, 1);
    lcd.print(F("Color: "));
    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();

  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkUp() ) // check if button right is pressed
      {
        //LCDML.BT_resetRight(); // reset the right button
        LCDML.BT_resetUp(); // reset the right button
        g_button_value++;

      }
       if (g_button_value ==3) {
      g_button_value =0;
    }
    
      if (LCDML.BT_checkDown() ) // check if button right is pressed
      {
       // LCDML.BT_resetLeft(); // reset the right button
        LCDML.BT_resetDown(); // reset the right button
        g_button_value--;
      
      }
      if (g_button_value ==0) {
      g_button_value =3;
    }
    }

    // check if button count is three
    switch (g_button_value) {
      case 1:
      pixels.clear(); // Set all pixel colors to 'off'
       for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
           pixels.setPixelColor(i, pixels.Color(0, 55, 0));
           pixels.show();   // Send the updated pixel colors to the hardware.  
        // update LCD content
        // update LCD content
        lcd.setCursor(6, 1); // set cursor
        lcd.print("Red        "); // print change content 
      }  
      break; 
    case 2:
      pixels.clear(); // Set all pixel colors to 'off'
       for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
           pixels.setPixelColor(i, pixels.Color(55, 55, 0));
           pixels.show();   // Send the updated pixel colors to the hardware.  
        lcd.setCursor(6, 1); // set cursor
        lcd.print("Yellow     "); // print change content      
      }  
      break; 
    case 3:
      pixels.clear(); // Set all pixel colors to 'off'
       for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
           pixels.setPixelColor(i, pixels.Color(15, 0, 155));
           pixels.show();   // Send the updated pixel colors to the hardware.  
        lcd.setCursor(6, 1); // set cursor
        lcd.print("Blue       "); // print change content      
      }  
      break; 
    }

   
 if (LCDML.BT_checkEnter() ) // check if button enter is pressed
      {
         LCDML.MENU_goRoot();

        
      }
   
  }

  if(LCDML.FUNC_close())     // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}



// *********************************************************************
void mFunc_screensaver(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // update LCD content
    lcd.setCursor(0, 0); // set cursor
    lcd.print(F("    DruMMio    ")); // print change content
    lcd.setCursor(0, 1); // set cursor
    lcd.print(F(" press any key "));
    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())
  {
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())
  {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_back(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // end function and go an layer back
    LCDML.FUNC_goBackToMenu(1);      // leave this function and go a layer back
  }
}


// *********************************************************************
void mFunc_goToRootMenu(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // go to root and display menu
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_jumpTo_timer_info(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
    
    // Jump to main screen
    LCDML.OTHER_jumpToFunc(mFunc_timer_info);
  }
}



// *********************************************************************
void mFunc_padEdit(uint8_t param)
// *********************************************************************
{
  bool buttonPush = button.GetPushState();
  bool editStart = button.GetEditState();
  bool editDone = button.GetEditdoneState();
  bool display = button.GetDisplayState();

  const char *padName = button.GetPadName();
  const char *item = button.GetSettingItem();
  int settingValue = button.GetSettingValue();
  int velocity = button.GetVelocity();
  const char *hitPad = button.GetHitPad();

  button.readButtonState();
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);
    
    
  }
  if (buttonPush == true)
    {
        lcd.clear();
        lcd.print(padName);
        lcd.setCursor(0, 1);
        lcd.print(item);
        lcd.setCursor(13, 1);
        lcd.print(settingValue);
    }

    if (editStart == true)
    {
        lcd.clear();
        lcd.print("EDIT START");
        delay(500);
        lcd.clear();
        lcd.print(padName);
        lcd.setCursor(0, 1);
        lcd.print(item);
        lcd.setCursor(13, 1);
        lcd.print(settingValue);
    }

    if (editDone == true)
    {
        lcd.clear();
        lcd.print("EDIT DONE");
        delay(500);
        lcd.clear();
        lcd.print(padName);
        lcd.setCursor(0, 1);
        lcd.print(item);
        lcd.setCursor(13, 1);
        lcd.print(settingValue);
    }
}




// *********************************************************************
void mFunc_ledsEdit(uint8_t param)
// *********************************************************************
{

 if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    // print LCD content
    // print LCD content
    lcd.setCursor(0, 0);
    lcd.print(F("Select Color"));
    lcd.setCursor(0, 1);
    lcd.print(F("Color: 0 of 3"));
    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();

  }

  LCDML.MENU_goRoot();
}
