#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebServer.h>

WebServer server(8080);

int STUDENT_ID = 25112050;

float getTemp() {
  return 26.0;
};

float getHum() {
  return 30.0;
};

float getUptime() {
  return millis() / 1000;
};

void sensorAPI() {
  Serial.println("\n Sending response");
  JsonDocument doc;
  doc["temp"] = getTemp();
  doc["hum"] = getHum();
  doc["uptime"] = getUptime();
  doc["sid"] = STUDENT_ID;

  String outJson;
  serializeJson(doc, outJson);

  server.send(200, "application/json", outJson);
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
  Serial.println("\n Connect at");
  Serial.println(WiFi.localIP());

  server.on("/api/sensor", HTTP_GET, sensorAPI);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
  // Serial.println("Server is still up \n");
  server.handleClient();
}
