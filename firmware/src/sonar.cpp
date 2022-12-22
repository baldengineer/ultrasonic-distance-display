#include <Main.h>

extern NewPing sonar;
extern void neo7_display_value(uint16_t value);

void handle_sonar(bool print_to_serial) {
    static uint32_t previous_ping = 0;
    if (millis() - previous_ping >= 500){
      uint32_t ping_measurement = sonar.ping_cm(); // returns 0 if too big
      if (ping_measurement == 0)
        ping_measurement = 888;
      neo7_display_value(uint16_t(ping_measurement));
      if (print_to_serial) {
        Serial.print("Ping: "); 
        Serial.println(ping_measurement);
      }
      previous_ping = millis();
    } 
}