#include <Main.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <WIFI_CONFIG.h>

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  uint8_t notConnectedCounter = 0;
  Serial.print("WiFi connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    notConnectedCounter++;
    if(notConnectedCounter > 128) { // Reset board if not connected after 5s
        Serial.println("WiFi Failed");
        //ESP.restart();
        while(1);
    }
  }
  Serial.print("Connected!\nIP:");
  Serial.println(WiFi.localIP());
  ArduinoOTA.begin();

  // Ultrasonic
  pinMode(ULTRA_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRA_ECHO_PIN, INPUT);
  // PIR
  pinMode(PIR_MOTION_PIN, INPUT);
  // Keep Alive LED
  pinMode(BLUE_LED_PIN, OUTPUT);
  // Neo7Segment
  disp.Begin(32);
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

void neo7_display_value(uint16_t value) {
  static uint16_t previous_value = 0;
  char count_string[4];
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

void loop() {
  ArduinoOTA.handle();
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

  if (millis() - motion_timeout <= MOTION_TIMEOUT_MILLISECONDS) {
    static uint32_t previous_ping = 0;
    if (millis() - previous_ping >= 500){
      uint32_t ping_measurement = sonar.ping_cm(); // returns 0 if too big
      if (ping_measurement == 0)
        ping_measurement = 888;
      neo7_display_value(uint16_t(ping_measurement));
      Serial.print("Ping: "); Serial.println(ping_measurement); 
      previous_ping = millis();
    } 
  } else {
      neo7_display_value(0);
  }
}

