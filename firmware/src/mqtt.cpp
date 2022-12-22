#include <Main.h>
#include <ca.h>

/*
WiFiClientSecure espClient;
PubSubClient client(espClient);
uint8_t previous_mqtt_status = 0;
extern void long_delay(uint32_t delay_milliseconds);

void mqtt_callback(char* topic, byte* payload, unsigned int length);
String mqtt_status_str(int state);
void display_mqtt_state(bool force_update);
void setup_mqtt();
void mqtt_reconnect();


// dispense-treat-toggle
// treat-counter-text
void mqtt_reconnect() {
  while (!client.connected()) {
    Serial.println("MQTT");
    // Create a random client ID
    String clientId = "ultradist-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
     // Serial.println(F("connected"));

      // MQTT topics for subscription
      client.subscribe("ha/debug");
      //client.subscribe("stream/treat-counter-text");
      Serial.println("Sub'd");

    } else {
      Serial.println("MQTT FAIL");
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      long_delay(5000);
    }
  }
}

void setup_mqtt() {
    Serial.println("CA Cert");
    // Connect to MQTT Broker
    //espClient.setFingerprintSHA256(fingerprint); // when you do know it
    espClient.setCACert(lets_encrypt_ca_cert);
    
    client.setServer(MQTT_SERVER, MQTT_SERVERPORT);
    client.setCallback(mqtt_callback);

    while (client.state() != MQTT_CONNECTED) {
        mqtt_reconnect();
        display_mqtt_state(false);
        delay(250);
    }

    display_mqtt_state(false);
    Serial.println(F("done!")); 
}

void display_mqtt_state(bool force_update) {
  static uint8_t previous_client_state = 0xFF;
  uint8_t current_client_state = client.state();

  if (force_update || (previous_client_state != current_client_state)) {
    previous_client_state = current_client_state;
    //Serial.print(F("[")); Serial.print(millis()); Serial.print(F("]: ")); Serial.println(mqtt_status_str(current_client_state));
    Serial.println(mqtt_status_str(current_client_state));
  }
}

String mqtt_status_str(int state) {
  switch (state) {
    case MQTT_CONNECTION_TIMEOUT:
      return "Timeout, Connection";
      break;
    case MQTT_CONNECTION_LOST:
      return "Lost, Connection";
      break;
    case MQTT_CONNECT_FAILED:
      return "Failed, Connection";
      break;
    case MQTT_DISCONNECTED:
      return "Disconnected";
      break;
    case MQTT_CONNECTED:
      return "Connected";
      break;
    case MQTT_CONNECT_BAD_PROTOCOL:
      return "BAD_PROTOCOL";
      break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
      return "BAD_CLIENT_ID";
      break;
    case MQTT_CONNECT_UNAVAILABLE:
      return "UNAVAILABLE";
      break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
      return "Bad Cred";
      break;
    case MQTT_CONNECT_UNAUTHORIZED:
      return "Unauthorized";
      break;
    default:
      return "I dunno!";
      break;
  }
  return "No can haz.";
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String topic_str = topic;

  Serial.print(F("Message arrived [")); Serial.print(topic_str); Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

// This comparison works because the Arduino String object supports ==

// these were for treatbot, they don't apply to this project, but leaving them in
// as examples.
  if (topic_str == "stream/dispense-treat-toggle") {
    Serial.println(F("Processing stream/dispense-treat-toggle"));
    if ((char)payload[0] == '1') {
      // arm_indicator_countdown = true;
      // previous_indicator_millis = millis();
      Serial.println("1 for dispense");
      //start_treats_cycle();
    }
  } else if (topic_str == "stream/treat-counter-text") {
    Serial.println(F("Processing stream/treat-counter-text"));
    if ((char)payload[0] == '1') {
     // mp3_play(3);
    }
  } else if (topic_str == "stream/twitch-attn-indi") {
    if ((char)payload[0] == '1') {
      Serial.println("ATTN!!");
      //mp3_play(2);
    }
  }
}

void mqtt_stay_alive() {
    // Stay Connected and keep MQTT alive
  if (!client.connected())
    mqtt_reconnect();
  client.loop();
  display_mqtt_state(false); 
}

*/