#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TEN_WIFI_NHA_BAN";
const char* pass = "MAT_KHAU_WIFI";
const char* mqtt_broker = "test.mosquitto.org";

// Topic Publish kèm MSV của bạn (25112021)
const char* topic_telemetry = "bcse/th4/25112021/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Ket noi MQTT Broker...");
    String clientId = "ESP32_Telemetry_25112021_" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println(" Thanh cong!");
    } else {
      Serial.print(" Loi rc=");
      Serial.print(client.state());
      Serial.println(" Thu lai sau 5s...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Da ket noi!");

  client.setServer(mqtt_broker, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Định kỳ gửi dữ liệu mỗi 3 giây
  unsigned long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;

    float temp = 29.0 + random(-10, 10) / 10.0;
    float hum = 60.0 + random(-20, 20) / 10.0;

    String payload = "{";
    payload += "\"sv_id\":\"25112021\",";
    payload += "\"temp\":" + String(temp, 1) + ",";
    payload += "\"hum\":" + String(hum, 1);
    payload += "}";

    client.publish(topic_telemetry, payload.c_str());
    Serial.printf("Publish -> Topic [%s]: %s\n", topic_telemetry, payload.c_str());
  }
}
