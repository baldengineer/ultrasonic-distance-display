#include <Arduino.h>
#include "Main.h"
#include <NewPing.h>
// #include <NeoPixelBus.h>
// const uint16_t PixelCount = 29; // this example assumes 4 pixels, making it smaller will cause a failure
// const uint8_t PixelPin = NEO7_DATA_PIN;  // make sure to set this to the correct pin, ignored for Esp8266
// NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(PixelCount, PixelPin);

#include <Neo7Segment.h>

#define PIXELS_DIGITS       3   // Number of digits
#define PIXELS_PER_SEGMENT  4   // If you want more than 10 pixels per segment, modify the Neo7Segment_Var.cpp
#define PIXELS_PER_POINT    1   // CANNOT be higher than PIXELS_PER_SEGMENT
#define PIXELS_PIN          NEO7_DATA_PIN   // Pin number

Neo7Segment disp(PIXELS_DIGITS, PIXELS_PER_SEGMENT, PIXELS_PER_POINT, PIXELS_PIN);

#define MOTION 0x1
#define NO_MOTION 0x0
#define MAX_DISTANCE 200 

NewPing sonar(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.


void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\nSetting up ranger");
  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);

  Serial.println("Set up PIR");
  pinMode(PIR_MOTION_PIN, INPUT);

  pinMode(BLUE_LED_PIN, OUTPUT);
  pingTimer = millis(); // Start now.

    // this resets all the neopixels to an off state
  // strip.Begin();
  // strip.Show();

    // Start the display with a brightness value of 20
  disp.Begin(20);
  if ( !disp.IsReady() )
    Serial.println("Neo7Segment Error...");
}

bool check_motion_PIR() {
  static bool previous_motion = false;
  bool current_motion = digitalRead(PIR_MOTION_PIN);

  if (previous_motion != current_motion) {
    previous_motion = current_motion;
    return current_motion;
  }
  return previous_motion;
}

void neo7_display_value(uint8_t value) {
  char count_string[4];
  sprintf(count_string, "%03d", value);
  String digit = String( count_string );
  uint8_t r,g,b;

  if (value >= 60) {
    r = 255;
    g = 255;
    b = 255;
  } 

  if ((value < 60 ) && (value >= 10)) {
    r = 0;
    g = 255;
    b = 0;
  }

  if ((value < 10)) {
    r = 255;
    g = 0;
    b = 0;
  }

  bool motion_state = check_motion_PIR();
  if (value == 0) {
    disp.SetDigit(0," ", disp.Color(0,0,0));
    disp.SetDigit(1," ", disp.Color(0,0,0));
    if (motion_state)
      disp.SetDigit(2,".", disp.Color(128,128,128));  
    else 
      disp.SetDigit(2," ", disp.Color(0,0,0));
  } else {
    // left to right
    disp.SetDigit(0,String(digit.charAt(0)), disp.Color(r,g,b));
    disp.SetDigit(1,String(digit.charAt(1)), disp.Color(r,g,b));
    if (motion_state)
     disp.SetDigit(2,(String(digit.charAt(2)) + "."), disp.Color(r,g,b)); 
    else
     disp.SetDigit(2,String(digit.charAt(2)), disp.Color(r,g,b));
  }
}

void loop() {
  static uint32_t previous_millis = 0;
  static bool led_state = true;
  if (millis() - previous_millis >= 500) {
    previous_millis = millis();
    led_state = !led_state;
    digitalWrite(BLUE_LED_PIN, led_state);
  }

  static uint32_t motion_timeout = 0;
  if (check_motion_PIR() == MOTION)
    motion_timeout = millis();

  if (millis() - motion_timeout <= 60000) {
    static uint32_t previous_ping = 0;
    if (millis() - previous_ping >= 500){
      uint32_t ping_measurement = sonar.ping_cm();
      neo7_display_value(uint8_t(ping_measurement));
      Serial.print("Ping: ");
      Serial.print(ping_measurement); // Send ping, get distance in cm and print result (0 = outside set distance range)
      Serial.println("cm");
      previous_ping = millis();
    } 
  } else {
      neo7_display_value(0);
  }
}

