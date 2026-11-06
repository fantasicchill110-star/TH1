#include <Arduino.h>

#define LED_PIN 2
#define TIME_TO_SLEEP 5 /* Thời gian ngủ (giây) */
#define uS_TO_S_FACTOR 1000000ULL /* Hệ số chuyển từ micro giây sang giây */

// Biến lưu trong bộ nhớ RTC (không bị mất khi Deep Sleep)
RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_TIMER: Serial.println("-> Thức dậy do Timer!"); break;
    default: Serial.printf("-> Thức dậy do nguyên nhân khác / Khởi động lại: %d\n", wakeup_reason); break;
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(1000); 

  // Tăng số lần boot
  bootCount++;
  Serial.println("====================================");
  Serial.printf("[ACTIVE MODE] Số lần boot (RTC Count): %d\n", bootCount);
  print_wakeup_reason();

  // Giả lập trạng thái ACTIVE: Sáng LED và hoạt động trong 3 giây
  Serial.println("ESP32 đang ở chế độ ACTIVE (Đèn LED BẬT)...");
  digitalWrite(LED_PIN, HIGH);
  delay(3000);

  // Chuẩn bị vào DEEP SLEEP
  Serial.println("ESP32 chuẩn bị vào DEEP SLEEP trong 5 giây (Đèn LED TẮT)...");
  digitalWrite(LED_PIN, LOW);
  Serial.flush(); 

  // Cấu hình Nguồn thức dậy (Timer 5 giây)
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  // Bắt đầu Deep Sleep
  esp_deep_sleep_start();
}

void loop() {
  // Không bao giờ chạy vào loop() vì chip sẽ reset lại setup() sau khi thức dậy từ Deep Sleep
}
