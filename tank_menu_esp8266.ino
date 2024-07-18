// ============================================================
// Example:     LCDML: graphic display with u8g
// ============================================================
// Author:      Jomelo
// Last update: 21.01.2018
// License:     MIT
// ============================================================
// Description:
// This example shows how to use the u8glib with the LCDMenuLib
// The menu can placed in a box that can be placed anywhere on
// the screen.
// ============================================================
// *********************************************************************
// special settings
// *********************************************************************
// enable this line when you are not usigng a standard arduino
// for example when your chip is an ESP or a STM or SAM or something else
#define _LCDML_cfg_use_ram 

  // include libs
  #include <LCDMenuLib2.h>
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  #include <ArduinoOTA.h>
  
  // U8g2lib
  #include <Arduino.h>
  #include <U8g2lib.h>

  #ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
  #endif
  #ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
  #endif

  // WiFi
  const char *ssid = "whataday"; // Enter your WiFi name
  const char *password = "wowwhataday";  // Enter WiFi password

  // MQTT Broker
  const char *mqtt_broker = "192.168.0.26";
  const char *pump = "cmnd/75gpumps/power";
  const char *power_head = "cmnd/75gpumps/power2";
  const char *protein_skimmer = "cmnd/75gpumps/power3";
  const char *uv = "cmnd/75gpumps/power4";
  const char *ligths = "cmnd/75glights/power1";
  const char *ato = "cmnd/75gdosingpumps/power5";
  const char *feed = "cmnd/hass/feedmode";
  const char *hass = "cmnd/hass/hass";
  const char *atofill = "cmnd/hass/atof";
  const char *mqtt_username = "reef";
  const char *mqtt_password = "12433";
  const int mqtt_port = 1883;
  int buttonState = 0;
  const int buttonPin = 2;

  WiFiClient espClient;
  PubSubClient client(espClient);

// *********************************************************************
// U8GLIB
// *********************************************************************
  // U8g2 Constructor List (Frame Buffer)
  // The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
  // Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
  U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, 14, 13, 15, U8X8_PIN_NONE);
  //U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 12, /* reset=*/ U8X8_PIN_NONE);   // (Uno and co

  // settings for u8g lib and LCD
  #define _LCDML_DISP_w                 128            // LCD width
  #define _LCDML_DISP_h                 64             // LCD height
  // font settings
  #define _LCDML_DISP_font              u8g_font_6x13  // u8glib font (more fonts under u8g.h line 1520 ...)
  #define _LCDML_DISP_font_w            6              // font width
  #define _LCDML_DISP_font_h            13             // font height
  // cursor settings
  #define _LCDML_DISP_cursor_char       "X"            // cursor char
  #define _LCDML_DISP_cur_space_before  2              // cursor space between
  #define _LCDML_DISP_cur_space_behind  4              // cursor space between
  // menu position and size
  #define _LCDML_DISP_box_x0            0              // start point (x0, y0)
  #define _LCDML_DISP_box_y0            0              // start point (x0, y0)
  #define _LCDML_DISP_box_x1            128            // width x  (x0 + width)
  #define _LCDML_DISP_box_y1            64             // hight y  (y0 + height)
  #define _LCDML_DISP_draw_frame        1              // draw a box around the menu
   // scrollbar width
  #define _LCDML_DISP_scrollbar_w       10  // scrollbar width (if this value is < 3, the scrollbar is disabled)

  // nothing change here
  #define _LCDML_DISP_cols_max          ((_LCDML_DISP_box_x1-_LCDML_DISP_box_x0)/_LCDML_DISP_font_w)
  #define _LCDML_DISP_rows_max          ((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0-((_LCDML_DISP_box_y1-_LCDML_DISP_box_y0)/_LCDML_DISP_font_h))/_LCDML_DISP_font_h)

  // rows and cols
  // when you use more rows or cols as allowed change in LCDMenuLib.h the define "_LCDML_DISP_cfg_max_rows" and "_LCDML_DISP_cfg_max_string_length"
  // the program needs more ram with this changes
  #define _LCDML_DISP_rows              _LCDML_DISP_rows_max  // max rows
  #define _LCDML_DISP_cols              20                   // max cols



// *********************************************************************
// Prototypes
// *********************************************************************
  void lcdml_menu_display();
  void lcdml_menu_clear();
  void lcdml_menu_control();


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

  // For beginners
  // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
  LCDML_add         (0 , LCDML_0           , 1  , "The Panel"    ,  mFunc_feed);
  LCDML_add         (1  , LCDML_0          , 2  , "ReturnPump"   , NULL);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (2  , LCDML_0_2        , 1  , "OFF"          , mFunc_pumpoff);
  LCDML_add         (3  , LCDML_0_2        , 2  , "ON"           , mFunc_pump);
  LCDML_add         (4  , LCDML_0_2        , 3  , "ReturnPump"   , mFunc_goToRootMenu);
  LCDML_add         (5  , LCDML_0          , 3  , "PowerHead"    , NULL);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (6  , LCDML_0_3        , 1  , "OFF"          , mFunc_phoff);
  LCDML_add         (7  , LCDML_0_3        , 2  , "ON"           , mFunc_ph);
  LCDML_add         (8  , LCDML_0_3        , 3  , "PowerHead"    , mFunc_goToRootMenu);
  LCDML_add         (9  , LCDML_0          , 4  , "ProteinSkimmer",  NULL);  
  LCDML_add         (10 , LCDML_0_4        , 1  , "Toggle"       , mFunc_pstoggle);
  LCDML_add         (11 , LCDML_0_4        , 2  , "OFF"          , mFunc_psoff);
  LCDML_add         (12 , LCDML_0_4        , 3  , "ON"           , mFunc_ps);
  LCDML_add         (13 , LCDML_0_4        , 4  , "ProteinSkimmer", mFunc_goToRootMenu);
  LCDML_add         (14 , LCDML_0          , 5  , "ATO"          ,  NULL);                    // NULL = no menu function
  LCDML_add         (15 , LCDML_0_5        , 1  , "OFF"          , mFunc_atooff);
  LCDML_add         (16 , LCDML_0_5        , 2  , "ON"           , mFunc_ato);
  LCDML_add         (17 , LCDML_0_5        , 3  , "ATO"          , mFunc_goToRootMenu);
  LCDML_add         (18 , LCDML_0          , 6  , "Ligths"       ,  NULL); 
  LCDML_add         (19 , LCDML_0_6        , 1  , "OFF"          , mFunc_loff);
  LCDML_add         (20 , LCDML_0_6        , 2  , "ON"           , mFunc_l);
  LCDML_add         (21 , LCDML_0_6        , 3  , "Ligths"       , mFunc_goToRootMenu);
  LCDML_add         (22 , LCDML_0          , 7  , "Control All"  ,  NULL);
  LCDML_add         (23 , LCDML_0_7        , 1  , "OFF"          , mFunc_off);
  LCDML_add         (24 , LCDML_0_7        , 2  , "ON"           , mFunc_on);
  LCDML_add         (25 , LCDML_0_7        , 3  , "ATOFill"      , mFunc_atofill);
  LCDML_add         (26 , LCDML_0_7        , 4  , "Restart"      ,  mFunc_restart);
  LCDML_add         (27 , LCDML_0_7        , 5  , "Control All"  ,  mFunc_goToRootMenu);


  // Advanced menu (for profit) part with more settings
  // Example for one function and different parameters
  // It is recommend to use parameters for switching settings like, (small drink, medium drink, big drink) or (200ml, 400ml, 600ml, 800ml) ...
  // the parameter change can also be released with dynParams on the next example
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  //LCDML_addAdvanced (16 , LCDML_0         , 5  , NULL,          "Parameter"      , NULL,                0,            _LCDML_TYPE_default);                    // NULL = no menu function
  //LCDML_addAdvanced (17 , LCDML_0_5       , 1  , NULL,          "Parameter 1"      , mFunc_para,       10,            _LCDML_TYPE_default);                    // NULL = no menu function
 // LCDML_addAdvanced (18 , LCDML_0_5       , 2  , NULL,          "Parameter 2"      , mFunc_para,       20,            _LCDML_TYPE_default);                    // NULL = no menu function
 // LCDML_addAdvanced (19 , LCDML_0_5       , 3  , NULL,          "Parameter 3"      , mFunc_para,       30,            _LCDML_TYPE_default);                    // NULL = no menu function
  //LCDML_add         (20 , LCDML_0_5       , 4  , "Back"             , mFunc_back);              // this menu function can be found on "LCDML_display_menuFunction" tab


  // Example for dynamic content
  // 1. set the string to ""
  // 2. use type  _LCDML_TYPE_dynParam   instead of    _LCDML_TYPE_default
  // this function type can not be used in combination with different parameters
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (29 , LCDML_0         , 10  , NULL,          ""                  , mDyn_para,                0,   _LCDML_TYPE_dynParam);                     // NULL = no menu function

  // Example for conditions (for example for a screensaver)
  // 1. define a condition as a function of a boolean type -> return false = not displayed, return true = displayed
  // 2. set the function name as callback (remove the braces '()' it gives bad errors)
  // LCDMenuLib_addAdvanced(id, prev_layer,     new_num, condition,   lang_char_array, callback_function, parameter (0-255), menu function type  )
  LCDML_addAdvanced (28 , LCDML_0         , 9  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);       // this menu function can be found on "LCDML_display_menuFunction" tab

  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    29

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);




// *********************************************************************
// SETUP
// *********************************************************************
  void setup()
  {
    u8g2.begin();
    ArduinoOTA.begin();
    // serial init; only be needed if serial control is used
    Serial.begin(115200);                // start serial
    Serial.println(F(_LCDML_VERSION)); // only for examples
    pinMode(12, OUTPUT);
    pinMode(buttonPin, INPUT);
    // LCDMenuLib Setup
    LCDML_setup(_LCDML_DISP_cnt);
    
    // Enable Menu Rollover
    LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 seconds
    //LCDML.SCREEN_disable();

    // Some needful methods

    // You can jump to a menu function from anywhere with
    //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name
  
 // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  //client.publish(topic, "on");
  client.subscribe(hass);
}
void(* resetFunc) (void) = 0;//declare reset function at address 0
void callback(char *topic, byte *payload,  unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
    
      
  }
  Serial.println();
  Serial.println("-----------------------");

  if ((char)payload[0] == '1') {
    digitalWrite(12, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(12    , LOW);  // Turn the LED off by making the voltage HIGH
    u8g2.drawStr( 0, (_LCDML_DISP_font_h * 1), "screen");

  }


  
}



// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    // this function must called here, do not delete it
    LCDML.loop();
    client.loop();
    ArduinoOTA.handle();
buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
    // turn LED on:
    client.publish(hass, "off");
    delay(1500);
  }
   }


// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_glcd_tft_box_x1 > _LCDML_glcd_tft_w)
# error _LCDML_glcd_tft_box_x1 is to big
# endif

# if(_LCDML_glcd_tft_box_y1 > _LCDML_glcd_tft_h)
# error _LCDML_glcd_tft_box_y1 is to big
# endif
