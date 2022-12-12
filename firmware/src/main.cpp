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

}

const RgbColor colors[] = {red,green,blue,white};

bool check_motion_PIR() {
  static bool previous_motion = false;
  bool current_motion = digitalRead(PIR_MOTION_PIN);

  if (previous_motion != current_motion) {
    previous_motion = current_motion;
    return current_motion;
  }
  return previous_motion;
}

// void single_pixel_ping_please() {
//   #define pixel_wait 500
//   while(1) {
//     strip.SetPixelColor(28, black);
//     strip.SetPixelColor(0,white);
//     strip.Show();
//     delay(pixel_wait);
//     for (int outer_loop=1; outer_loop < PixelCount; outer_loop++) {
//       strip.SetPixelColor(outer_loop-1, black);
//       strip.SetPixelColor(outer_loop, white);
//       strip.Show();
//       delay(pixel_wait);
//     }
//     strip.SetPixelColor(27,black);
//     strip.SetPixelColor(28, white);
//     strip.Show();
//     delay(pixel_wait*4);
//   }
// }

// void neopixelbus_color_change() {
//     static byte color_index = 0;
//   for (int x=0; x < PixelCount; x++) {
//     strip.SetPixelColor(x, colors[color_index]);
//   }
//   color_index++;
//   if (color_index >=4)
//     color_index = 0;
//   strip.Show();
//   delay(1000);
// }

void loop() {
  static uint32_t display_count = 100;
  // single_pixel_ping_please();
  if ( !disp.IsReady() )
    return;

  static unsigned long previous_count = 0;
  if (millis() - previous_count >= 100) {
    Serial.print("display_count: "); 
    Serial.println(display_count);
    display_count++;
    if (display_count > 128)
      display_count = 0;

    String dot = ( ( display_count%2 ) == 0 ) ? "." : "";
    char count_string[4];
    sprintf(count_string, "%03lu", display_count);
    String digit = String( count_string );
    Serial.print("count_string: ");
    Serial.println(count_string);
    Serial.print("digit: ");
    Serial.println(digit);    
    

    disp.SetDigit(0,String(digit.charAt(0)), disp.Color(255,255,255));
    disp.SetDigit(1,String(digit.charAt(1)), disp.Color(255,255,255));
    disp.SetDigit(2,String(digit.charAt(2)), disp.Color(255,255,255));

    previous_count = millis();
  }  



  static uint32_t previous_millis = 0;
  static bool led_state = true;
  if (millis() - previous_millis >= 500) {
    previous_millis = millis();
    led_state = !led_state;
    digitalWrite(BLUE_LED_PIN, led_state);
  }

  if (check_motion_PIR() == MOTION) {
    static uint32_t previous_ping = 0;
    if (millis() - previous_ping >= 500){
      Serial.print("Ping: ");
      Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
      Serial.println("cm");
      previous_ping = millis();
    }
  }
}

