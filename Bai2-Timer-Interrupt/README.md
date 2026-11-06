## Bài 1: So sánh nhịp định kỳ (delay, millis, hardware timer)

### 1. Kết quả thực nghiệm (Chụp ảnh Serial Monitor dán vào đây)
- **Chu kỳ cài đặt:** 1000 ms.
- **Tác vụ CPU bận giả lập:** 300 ms.

| Phương pháp | Khoảng thời gian thực tế đo được | Sai số / Đánh giá |
| :--- | :--- | :--- |
| `delay()` | ~1300 ms | Sai số lớn (+300ms). Chu kỳ bị bận do CPU phải chờ xử lý xong tác vụ mới tính tiếp `delay()`. |
| `millis()` | ~1000 ms | Độ chính xác cao. Không bị ảnh hưởng nhiều trừ khi tác vụ nặng hơn cả chu kỳ. |
| **Timer HW** | **1000 ms** | **Chính xác nhất.** Ngắt cứng được ưu tiên xử lý ngay lập tức bất kể CPU đang làm gì. |

### 2. Kết luận
- **Khi CPU bận:** `Timer phần cứng` và `millis()` bám nhịp tốt nhất. `delay()` làm chặn (blocking) luồng thực thi nên làm trôi nhịp hoàn toàn.
- **Cách dùng khuyến nghị:** Dùng Hardware Timer cho các tác vụ thời gian thực nghiêm ngặt, dùng `millis()` cho các tác vụ không chặn thông thường, tránh dùng `delay()`.

- 

## Bài 2: Đề xuất & So sánh các chế độ tiêu thụ điện ESP32

### 1. Dấu hiệu nhận biết các chế độ
- **Chế độ Active (Hoạt động):** 
  - LED Pin 2 sáng[cite: 4].
  - Mạch gửi dữ liệu liên tục qua Serial Monitor với nội dung: `[ACTIVE MODE] Số lần boot...`[cite: 4].
  - Dòng điện tiêu thụ mức cao (khoảng 80mA - 160mA/WiFi).
- **Chế độ Deep Sleep (Ngủ sâu):** 
  - LED Pin 2 tắt hoàn toàn[cite: 4].
  - Mắt truyền Serial ngưng hoạt động[cite: 4].
  - Dòng điện tiêu thụ giảm xuống mức cực thấp (~10µA đối với con chip ESP32 lẻ).
  - Giá trị biến `bootCount` lưu trong bộ nhớ RTC được duy trì và tăng lên sau mỗi chu kỳ thức dậy[cite: 4].

### 2. Giới hạn đo lường thực tế với phần cứng hiện tại
- **Khó khăn:** Không thể đo chính xác dòng điện micro-ampe (µA) bằng các đồng hồ vạn năng thông thường hoặc đo trực tiếp từ cổng USB PC[cite: 4].
- **Lý do phần cứng:** Mạch nạp ESP32 DevKit tích hợp sẵn chip chuyển đổi USB-to-UART (CP2102/CH340) và IC hạ áp (AMS1117). Các linh kiện phụ này vẫn tiêu thụ dòng tĩnh (~10mA-15mA) ngay cả khi chip ESP32 đã đi vào Deep Sleep[cite: 4].
- **Giải pháp:** Sử dụng phương pháp quan sát gián tiếp hành vi (trạng thái LED, log Serial Monitor và biến RTC Memory) để chứng minh ESP32 đã thực hiện chuyển đổi chế độ thành công[cite: 4].





