#include <Arduino.h>

#define LED_PIN 2
#define BUTTON_PIN GPIO_NUM_4  // Chân nút bấm để thức dậy (EXT0 Wakeup)
#define TIME_TO_SLEEP 10       // Thời gian ngủ (giây) nếu đánh thức bằng Timer
#define uS_TO_S_FACTOR 1000000ULL 

// Biến lưu trong bộ nhớ RTC (duy trì giá trị qua các lần Deep Sleep)
RTC_DATA_ATTR int bootCount = 0;

// Hàm kiểm tra và in nguyên nhân thức dậy
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println("-> Thức dậy do TÍN HIỆU NGOẠI VI (Nút bấm GPIO 4)!");
      break;
    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("-> Thức dậy do HẾT THỜI GIAN TIMER (10s)!");
      break;
    default:
      Serial.printf("-> Khởi động lần đầu / Reset nguồn (Mã: %d)\n", wakeup_reason);
      break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(1000); // Chờ Serial khởi tạo ổn định

  // Tăng và hiển thị số lần boot từ bộ nhớ RTC
  bootCount++;
  Serial.println("\n====================================");
  Serial.printf("[ACTIVE MODE] Số lần boot (RTC Boot Count): %d\n", bootCount);
  
  // In nguyên nhân khiến ESP32 thức dậy
  print_wakeup_reason();

  // Giả lập trạng thái ACTIVE: Cháy LED 3 giây
  Serial.println("ESP32 đang HOẠT ĐỘNG (ACTIVE)... Đèn LED BẬT");
  digitalWrite(LED_PIN, HIGH);
  delay(3000);

  // Chuẩn bị vào Deep Sleep: Tắt LED
  digitalWrite(LED_PIN, LOW);
  Serial.println("ESP32 chuẩn bị vào DEEP SLEEP (Đèn LED TẮT)...");
  Serial.println("- Đánh thức cách 1: Bấm nút ở chân GPIO 4 (Logic LOW)");
  Serial.println("- Đánh thức cách 2: Tự thức dậy sau 10 giây (Timer)");
  Serial.flush(); // Đẩy hết dữ liệu Serial ra trước khi ngủ

  // 1. Cấu hình Nguồn thức dậy 1: Nút bấm ngoại vi (EXT0 - GPIO 4 mức 0)
  esp_sleep_enable_ext0_wakeup(BUTTON_PIN, 0);

  // 2. Cấu hình Nguồn thức dậy 2: Timer (10 giây)
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Bắt đầu vào Deep Sleep
  esp_deep_sleep_start();
}

void loop() {
  // Không dùng loop() vì chip sẽ reset lại setup() khi thức dậy từ Deep Sleep
}
