
#include <Arduino.h>

// Định cấu hình chu kỳ mục tiêu: 1000ms (1 giây)
const unsigned long TARGET_INTERVAL = 1000;

// Biến cho millis()
unsigned long previousMillis = 0;
unsigned long lastMillisExec = 0;

// Biến cho delay()
unsigned long lastDelayExec = 0;

// Biến cho Hardware Timer
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile unsigned long timerInterruptCount = 0;
volatile bool timerFlag = false;
unsigned long lastTimerExec = 0;

// Hàm xử lý ngắt Timer (ISR)
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  timerInterruptCount++;
  timerFlag = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

// Giả lập tác vụ nặng khiến CPU bị bận (ví dụ tốn ~300ms)
void doHeavyWork() {
  delay(300); 
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- KẾT QUẢ ĐO CHU KỲ (TARGET: 1000 ms) ---");

  // Khởi tạo Hardware Timer (Timer 0, prescaler 80 -> 1us per tick)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true); // Ngắt mỗi 1.000.000 us = 1s
  timerAlarmEnable(timer);

  unsigned long now = millis();
  lastDelayExec = now;
  lastMillisExec = now;
  lastTimerExec = now;
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Kiểm tra DELAY()
  // -------------------------------------------------------------
  // Giả lập: Xử lý công việc -> delay 1000ms
  doHeavyWork(); 
  delay(TARGET_INTERVAL);
  unsigned long nowDelay = millis();
  Serial.print("[DELAY]   Thực tế: ");
  Serial.print(nowDelay - lastDelayExec);
  Serial.println(" ms (Lỗi giãn chu kỳ do CPU bận)");
  lastDelayExec = nowDelay;

  // 2. Kiểm tra MILLIS()
  // -------------------------------------------------------------
  if (currentMillis - previousMillis >= TARGET_INTERVAL) {
    previousMillis = currentMillis;
    Serial.print("[MILLIS]  Thực tế: ");
    Serial.print(currentMillis - lastMillisExec);
    Serial.println(" ms");
    lastMillisExec = currentMillis;
  }

  // 3. Kiểm tra HARDWARE TIMER
  // -------------------------------------------------------------
  if (timerFlag) {
    portENTER_CRITICAL(&timerMux);
    timerFlag = false;
    portEXIT_CRITICAL(&timerMux);

    unsigned long nowTimer = millis();
    Serial.print("[TIMER HW] Thực tế: ");
    Serial.print(nowTimer - lastTimerExec);
    Serial.println(" ms (Ngắt chính xác chuẩn 1s)");
    lastTimerExec = nowTimer;
  }
}
