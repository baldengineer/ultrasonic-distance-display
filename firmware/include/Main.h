#include <Arduino.h>
#include <Neo7Segment.h>
#include <NewPing.h>

// Generic
#define MOTION 0x1
#define NO_MOTION 0x0
#define ULTRA_TRIGGER_PIN 0
#define ULTRA_ECHO_PIN    14
#define PIR_MOTION_PIN    12
#define NEO7_DATA_PIN     15
#define BLUE_LED_PIN      2

// Neo7Segment (Neo7Segment.h)
#define PIXELS_PIN          NEO7_DATA_PIN   // Pin number
#define PIXELS_DIGITS       3   // Number of digits
#define PIXELS_PER_SEGMENT  4   // If you want more than 10 pixels per segment, modify the Neo7Segment_Var.cpp
#define PIXELS_PER_POINT    1   // CANNOT be higher than PIXELS_PER_SEGMENT
Neo7Segment disp(PIXELS_DIGITS, PIXELS_PER_SEGMENT, PIXELS_PER_POINT, PIXELS_PIN);

// Ultrasonic Sensor (NewPing.h)
#define MOTION_TIMEOUT_MILLISECONDS 15000
#define MAX_DISTANCE 200
NewPing sonar(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.




/* ESP8266 available pins
https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/pinouts

* IO0 - RED LED (and output only)
IO2 - pulled-up Blue LED (and output only)
* IO15 - pulled-down (and output only)
IO16 - Wake up out of deep-sleep

* IO14 SPI-SCK
* IO12 SPI-MISO
IO13 SPI-MOSI

IO5 I2C-SCL
IO4 I2C-SDA

IO3 RX
IO1 TX

*/