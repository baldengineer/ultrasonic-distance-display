#include <Main.h>

extern bool check_motion_PIR();
extern Neo7Segment disp;

void good_neo7_display(uint16_t value, uint8_t r, uint8_t g, uint8_t b) {
  char count_string[6];
  sprintf(count_string, "%03d", value);
  String digit = String( count_string );
  
  disp.SetDigit(0,String(digit.charAt(0)), disp.Color(r,g,b));
  disp.SetDigit(1,String(digit.charAt(1)), disp.Color(r,g,b));
  disp.SetDigit(2,String(digit.charAt(2)), disp.Color(r,g,b));
}

void neo7_display_value(uint16_t value) {
  static uint16_t previous_value = 0;
  char count_string[6];
  sprintf(count_string, "%03d", value);
  String digit = String( count_string );
  uint8_t r,g,b;

  // only do Neo updates when the value changes (especially useful for off)
  if (previous_value == value)
    return;
  previous_value = value;

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