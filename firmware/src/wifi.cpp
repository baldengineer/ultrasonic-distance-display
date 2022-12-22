#include <Main.h>

extern void good_neo7_display(uint16_t value, uint8_t r, uint8_t g, uint8_t b);

void setup_WIFI() {
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
        //while(1);
    }
  }
  Serial.print("Connected!\nIP:");
  Serial.println(WiFi.localIP());
}

void new_setup_OTA() {
  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA.setHostname(WIFI_HOSTNAME);
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setPassword(OTA_PASS);
    ArduinoOTA.begin();
    Serial.println("OTA is ready...");
    uint32_t OTA_start_time = millis();
    while(millis() - OTA_start_time <= 10000) {
      static bool blink_state = true;
      static uint32_t previous_blink = 0;
      if (millis() - previous_blink >= 1000) {
        if (blink_state)
          good_neo7_display(888,0,0,255);
        else
          good_neo7_display(888,0,0,0);
        blink_state = !blink_state;
        previous_blink = millis();
      }
      ArduinoOTA.handle();
      yield();
    } 
    Serial.println("OTA Failed...");
    delay(1000);
    ESP.restart();
    while(1);
  } else {
    Serial.println("there's no wifi...");
    ESP.restart();
  }
}

void setup_OTA() {
  ArduinoOTA.setHostname(WIFI_HOSTNAME);
  ArduinoOTA.setPort(OTA_PORT);
  ArduinoOTA.setPassword(OTA_PASS);
  ArduinoOTA.begin();
}

void led_keep_alive() {
  static uint32_t previous_millis = 0;
  static bool led_state = true;
  uint32_t led_delay = ERROR_LED_BLINK;

  if (WiFi.status() == WL_CONNECTED)
    led_delay = NORMAL_LED_BLINK;

  if (millis() - previous_millis >= led_delay) {
    previous_millis = millis();
    led_state = !led_state;
    digitalWrite(BLUE_LED_PIN, led_state);
  }
}