# Pin Configuration – Smart Visitor Alert System

## ESP32 Pin Mapping

| Component                     | ESP32 Pin |
|------------------------------|-----------|
| IR Sensor (OUT)              | 27        |
| Ultrasonic TRIG              | 12        |
| Ultrasonic ECHO              | 14        |
| OLED MOSI                    | 23        |
| OLED CLK (SCK)               | 18        |
| OLED DC                      | 15        |
| OLED CS                      | 5         |
| OLED RESET                   | 22        |

---

## Power Connections

| Component        | Connection |
|------------------|------------|
| IR Sensor VCC    | 3.3V / 5V  |
| IR Sensor GND    | GND        |
| Ultrasonic VCC   | 5V         |
| Ultrasonic GND   | GND        |
| OLED VCC         | 3.3V / 5V  |
| OLED GND         | GND        |

---

## Detection Logic

- IR sensor detects motion.
- Motion must remain stable for 3 seconds.
- Ultrasonic sensor validates distance between 20–70 cm.
- Telegram notification is sent only if:
  - Visitor not previously detected
  - Cooldown period (20 seconds) has expired

---

## Display Behavior

- OLED shows:
  - Current Time (via NTP)
  - Dynamic Greeting
  - “Hello!” for 10 seconds when visitor detected

---

## Important Notes

- Telegram bot uses HTTPS (WiFiClientSecure).
- NTP server configured for IST (GMT +5:30).
- Cooldown logic prevents notification spam.
- Replace WiFi and Bot credentials with secure values before running.

