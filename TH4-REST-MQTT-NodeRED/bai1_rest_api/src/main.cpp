#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Thông tin Wi-Fi (thay bằng Wi-Fi nhà/lớp của bạn)
const char* ssid = "TEN_WIFI_NHA_BAN";
const char* pass = "MAT_KHAU_WIFI";

WebServer server(80);

// Hàm xử lý endpoint REST API /api/sensor
void handleSensorApi() {
  // Giả lập đọc giá trị nhiệt độ và độ ẩm
  float temp = 28.5 + random(-10, 10) / 10.0;
  float hum = 65.0 + random(-20, 20) / 10.0;

  // Đóng gói dữ liệu định dạng JSON kèm MSV của bạn (25112021)
  String jsonPayload = "{";
  jsonPayload += "\"sv_id\":\"25112021\",";
  jsonPayload += "\"temperature\":" + String(temp, 1) + ",";
  jsonPayload += "\"humidity\":" + String(hum, 1);
  jsonPayload += "}";

  // Trả về response HTTP 200 dạng application/json
  server.send(200, "application/json", jsonPayload);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Kết nối tới mạng Wi-Fi
  WiFi.begin(ssid, pass);
  Serial.print("Dang ket noi Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n-> Wi-Fi Da ket noi thanh cong!");
  Serial.print("Đuong dan REST API: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/api/sensor");

  // Đăng ký route /api/sensor
  server.on("/api/sensor", handleSensorApi);
  server.begin();
}

void loop() {
  server.handleClient();
}
