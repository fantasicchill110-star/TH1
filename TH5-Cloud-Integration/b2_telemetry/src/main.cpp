#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

WiFiClient wifi;
PubSubClient mqtt(wifi);

int STUDENT_ID = 25112050;
String student_group = "aet2022/nhom-02/telemetry";
String clientId = "ESP32-Client-" + String(STUDENT_ID);

float getTemp() {
  return 26.0;
};

float getHum() {
  return 30.0;
};

float getUptime() {
  return millis() / 1000;
};

String sensorAPI() {
  Serial.println("\n Sending response");
  JsonDocument doc;
  doc["temp"] = getTemp();
  doc["hum"] = getHum();
  doc["uptime"] = getUptime();
  doc["sid"] = STUDENT_ID;
  doc["rssi"] = WiFi.RSSI();

  String outJson;
  serializeJson(doc, outJson);

  return outJson;
}

void reconnect() {
  while (!mqtt.connected()) {
    Serial.println("Not connected, retrying mosquitto");
    Serial.println(mqtt.connected());
    mqtt.connect(clientId.c_str());
  };

  Serial.println("Exited, connected");
  return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Begin server");

  WiFi.begin("Wokwi-GUEST");
  while (WiFi.status() != WL_CONNECTED)

  {
    delay(250);
    Serial.println("Not connected, retrying");
  }

  Serial.println("Server started");
  mqtt.setServer("broker.hivemq.comk", 1883);

  reconnect();
}

void loop() {
  Serial.println("Publishing topic.");

  if (!mqtt.connected()) {
    reconnect();
  };

  String item = sensorAPI();
  mqtt.publish(student_group.c_str(), item.c_str());
  
  mqtt.loop();
}
