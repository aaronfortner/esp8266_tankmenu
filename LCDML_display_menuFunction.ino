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

// *********************************************************************
void mFunc_information(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // setup function
    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "To close this");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "function press");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 3), "any button or use");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 4), "back button");
    } while( u8g2.nextPage() );
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if(LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
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

    g_func_timer_info = 20;       // reset and set timer

    char buf[20];
    sprintf (buf, "wait %d seconds", g_func_timer_info);

    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "or press back button");
    } while( u8g2.nextPage() );



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
    if(LCDML.TIMER_ms(g_timer_1, 1000))
    {
      g_timer_1 = millis();
      g_func_timer_info--;                // increment the value every second
      char buf[20];
      sprintf (buf, "wait %d seconds", g_func_timer_info);

      u8g2.setFont(_LCDML_DISP_font);
      u8g2.firstPage();
      do {
        u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
        u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "or press back button");
      } while( u8g2.nextPage() );

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
    char buf[17];
    sprintf (buf, "count: %d of 3", 0);

    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "press a or w button");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), buf);
    } while( u8g2.nextPage() );

    // Reset Button Value
    g_button_value = 0;

    // Disable the screensaver for this function until it is closed
    LCDML.FUNC_disableScreensaver();
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    // loop function, can be run in a loop when LCDML_DISP_triggerMenu(xx) is set
    // the quit button works in every DISP function without any checks; it starts the loop_end function

    // the quit button works in every DISP function without any checks; it starts the loop_end function
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML.BT_checkLeft() || LCDML.BT_checkUp()) // check if button left is pressed
      {
        LCDML.BT_resetLeft(); // reset the left button
        LCDML.BT_resetUp(); // reset the left button
        g_button_value++;

        // update LCD content
        char buf[20];
        sprintf (buf, "count: %d of 3", g_button_value);

        u8g2.setFont(_LCDML_DISP_font);
        u8g2.firstPage();
        do {
          u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "press a or w button");
          u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), buf);
        } while( u8g2.nextPage() );
      }
    }

   // check if button count is three
    if (g_button_value >= 3) {
      LCDML.FUNC_goBackToMenu();      // leave this function
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

    // setup function
    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "screensaver");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "press any key");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 3), "to leave it");
    } while( u8g2.nextPage() );

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())           // ****** LOOP *********
  {
    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())          // ****** STABLE END *********
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
void mFunc_para(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {

    char buf[20];
    sprintf (buf, "parameter: %d", param);

    // setup function
    u8g2.setFont(_LCDML_DISP_font);
    u8g2.firstPage();
    do {
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), buf);
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 2), "press any key");
      u8g2.drawStr( 0, (_LCDML_DISP_font_h * 3), "to leave it");
    } while( u8g2.nextPage() );

    LCDML.FUNC_setLoopInterval(100);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop())               // ****** LOOP *********
  {
    // For example
    switch(param)
    {
      case 10:
        // do something
        break;

      case 20:
        // do something
        break;

      case 30:
        // do something
        break;

      default:
        // do nothing
        break;
    }


    if (LCDML.BT_checkAny()) // check if any button is pressed (enter, up, down, left, right)
    {
      LCDML.FUNC_goBackToMenu();  // leave this function
    }
  }

  if(LCDML.FUNC_close())      // ****** STABLE END *********
  {
    // you can here reset some global vars or do nothing
  }
}

// *********************************************************************
void mFunc_pump(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(pump, "on");
    LCDML.MENU_goRoot();
  }




}


// *********************************************************************
void mFunc_ph(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(power_head, "on");
    LCDML.MENU_goRoot();    
    
  }
}


// *********************************************************************
void mFunc_ps(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(protein_skimmer, "on");
    LCDML.MENU_goRoot();
  }
}

// *********************************************************************
void mFunc_ato(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(ato, "on");
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_l(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
   
    client.publish(ligths, "on");
    // read all the available characters
    LCDML.MENU_goRoot();
  }
}
//**********************************************************************
void mFunc_off(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
   Serial.println("power1 0");
   client.publish(pump, "off");
   delay(300);
   Serial.println("power2 0");
   client.publish(power_head, "off");
   delay(300);
   Serial.println("power3 0");
   client.publish(protein_skimmer, "off");
   delay(300);
   Serial.println("power4 0");
   client.publish(protein_skimmer, "off");
   delay(300);
   Serial.println("power5 0");
   client.publish(ligths, "off");
   // read all the available characters
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_pumpoff(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(pump, "off");
    LCDML.MENU_goRoot();
  }




}


// *********************************************************************
void mFunc_phoff(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(power_head, "off");
    LCDML.MENU_goRoot();    
    LCDML.MENU_goRoot();
  }
}


// *********************************************************************
void mFunc_psoff(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(protein_skimmer, "off");
    LCDML.MENU_goRoot();
  }
}

// *********************************************************************
void mFunc_atooff(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    client.publish(ato, "off");
    LCDML.MENU_goRoot();
  }
}



// *********************************************************************
void mFunc_loff(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
   
    client.publish(ligths, "off");
    // read all the available characters
    LCDML.MENU_goRoot();
  }
}
//**********************************************************************
void mFunc_on(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
   Serial.println("power1 1");
   client.publish(pump, "on");
   delay(300);
   Serial.println("power2 1");
   client.publish(power_head, "on");
   delay(300);
   Serial.println("power3 1");
   client.publish(protein_skimmer, "on");
   delay(300);
   Serial.println("power4 1");
   client.publish(protein_skimmer, "on");
   delay(300);
   Serial.println("power5 1");
   client.publish(ligths, "on");
    // read all the available characters
    LCDML.MENU_goRoot();
  }
}
//**********************************************************************
void mFunc_feed(uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
   
   
   client.publish(feed, "on");
    // read all the available characters
    LCDML.MENU_goRoot();
  }
}
// *********************************************************************
void mFunc_pstoggle (uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    Serial.println("power3 2");
    client.publish(protein_skimmer, "toggle");
    LCDML.MENU_goRoot();
  }
}
// *********************************************************************
void mFunc_restart (uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    Serial.println("restart 1");
    resetFunc(); //call reset
    LCDML.MENU_goRoot();
  }
}
// *********************************************************************
void mFunc_atofill (uint8_t param)
// *********************************************************************
{
  if(LCDML.FUNC_setup())          // ****** SETUP *********
  {
    // remmove compiler warnings when the param variable is not used:
    LCDML_UNUSED(param);

    // Jump to main screen
    Serial.println("ato fill");
    client.publish(atofill, "on");
    LCDML.MENU_goRoot();
  }
}
