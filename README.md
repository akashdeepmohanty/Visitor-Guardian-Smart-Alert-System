# Smart Visitor Alert System

## Overview
The Smart Visitor Alert System is an ESP32-based IoT project designed to detect visitors at the door using an IR motion sensor and ultrasonic distance validation. Upon confirming a visitor, the system sends a real-time notification via Telegram and displays greeting messages along with live time on an OLED screen.

This project demonstrates embedded system design combined with IoT integration, secure HTTPS communication, time synchronization using NTP, and multi-sensor validation logic.

---

## Features
- ESP32-based WiFi connectivity
- IR motion detection
- Ultrasonic distance verification (20–70 cm range)
- 3-second stable detection logic
- Telegram bot notification (HTTPS secure)
- 20-second notification cooldown protection
- Real-time clock using NTP (IST configured)
- Dynamic greeting based on time of day
- OLED display (Time + Greeting + Visitor Message)
- 10-second “Hello” display timer

---

## Hardware Components
- ESP32 Development Board
- IR Motion Sensor (Digital Output)
- Ultrasonic Sensor (HC-SR04)
- 0.96" OLED Display (SSD1306 SPI)
- Jumper Wires

---

## System Working

1. The IR sensor detects motion.
2. Motion must remain stable for at least 3 seconds.
3. Ultrasonic sensor validates distance between 20–70 cm.
4. If conditions are satisfied:
   - Telegram notification is sent.
   - “Hello!” message is displayed on OLED for 10 seconds.
5. System enforces a 20-second cooldown to prevent repeated alerts.
6. OLED continuously displays:
   - Current time (via NTP)
   - Dynamic greeting (Morning/Afternoon/Evening/Night)

This multi-layer validation ensures reliable visitor detection and reduces false triggers.

---

## Technical Concepts Implemented
- WiFi Networking using ESP32
- HTTPS Communication (WiFiClientSecure)
- Telegram Bot API Integration
- JSON Handling (ArduinoJson)
- NTP Time Synchronization
- Multi-Sensor Data Fusion
- Cooldown and Timer Logic using millis()
- SPI OLED Interfacing
- Embedded State Management

---

## Libraries Used
- WiFi.h
- WiFiClientSecure.h
- UniversalTelegramBot
- ArduinoJson
- Adafruit_GFX
- Adafruit_SSD1306

---

## Security Note
WiFi credentials and Telegram bot tokens are not included in this repository. Replace placeholder values in the code with your own secure credentials.

---

## Future Improvements
- Add camera snapshot integration
- Add cloud logging of visitor timestamps
- Add buzzer alert for indoor notification
- Replace polling delay with fully non-blocking architecture
- Add mobile app dashboard

---

## Author
Developed by **Akashdeep Mohanty**

This project was built as part of advanced embedded systems and IoT experimentation using ESP32 and real-time communication protocols.

