#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LED_PIN 2
#define BTN_PIN 4

const char* ssid = "TEN_WIFI_NHA_BAN";
const char* pass = "MAT_KHAU_WIFI";
const char* mqtt_broker = "test.mosquitto.org";

const char* topic_sub_led = "bcse/th4/25112021/control";
const char* topic_pub_btn = "bcse/th4/25112021/button";

WiFiClient espClient;
PubSubClient client(espClient);

int lastBtnState = HIGH;

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.printf("-> Nhan lenh tu Topic [%s]: %s\n", topic, message.c_str());

  if (message == "1" || message == "ON") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Đèn LED -> BẬT");
  } else if (message == "0" || message == "OFF") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Đèn LED -> TẮT");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Ket noi MQTT Broker...");
    String clientId = "ESP32_FullApp_25112021_" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println(" Thanh cong!");
      client.subscribe(topic_sub_led);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setServer(mqtt_broker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int btnState = digitalRead(BTN_PIN);
  if (btnState == LOW && lastBtnState == HIGH) {
    client.publish(topic_pub_btn, "PRESSED");
    Serial.println("Nút bấm đã được nhấn -> Sent MQTT!");
    delay(200);
  }
  lastBtnState = btnState;
}
