#include <Main.h>

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