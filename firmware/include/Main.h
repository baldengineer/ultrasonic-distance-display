#include <NeoPixelBus.h>

#define ULTRA_TRIGGER_PIN 0
#define ULTRA_ECHO_PIN    14
#define PIR_MOTION_PIN    12
#define NEO7_DATA_PIN     15
#define BLUE_LED_PIN      2

#define colorSaturation 32

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);
/*
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