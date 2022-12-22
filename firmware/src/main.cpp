#include <Main.h>

extern void setup_WIFI();
extern void setup_OTA();
extern void led_keep_alive();
extern void neo7_display_value(uint16_t value);
extern void handle_sonar();
// void mqtt_stay_alive();
// void setup_mqtt();

// extern WiFiClientSecure espClient;
// extern PubSubClient client(espClient);

Neo7Segment disp(PIXELS_DIGITS, PIXELS_PER_SEGMENT, PIXELS_PER_POINT, PIXELS_PIN);
NewPing sonar(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200);
  delay(250);

  Serial.println("\n\nUltrasonic Distance Display (Now in Colour)");
  
  setup_WIFI();
  if (WiFi.status() == WL_CONNECTED) {
    setup_OTA();
    //setup_mqtt();
  }

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
    // if (current_motion == MOTION)
    //   client.publish("ha/notify","Motion Detect");
    return current_motion;
  }
  return previous_motion;
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA.handle();
  //  mqtt_stay_alive();
  }

  led_keep_alive();

  static uint32_t motion_timeout = 0;
  if (check_motion_PIR() == MOTION)
    motion_timeout = millis();

  if (millis() - motion_timeout <= MOTION_TIMEOUT_MILLISECONDS) {
      handle_sonar();
  } else {
      neo7_display_value(0);
  }
}

void long_delay(uint32_t delay_milliseconds) {
  delay(delay_milliseconds);
}