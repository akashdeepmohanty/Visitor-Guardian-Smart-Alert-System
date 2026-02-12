#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "time.h"

// ================== USER CONFIGURATION ==================
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"
// ========================================================
#define IRSENSOR 27
#define TRIG_PIN 12
#define ECHO_PIN 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI 23
#define OLED_CLK 18
#define OLED_DC 15    
#define OLED_CS 5
#define OLED_RESET 22 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800; // IST
const int daylightOffset_sec = 0;

bool visitorDetected = false;
unsigned long lastNotifyTime = 0;
const unsigned long notifyCooldown = 20000; // 20s cooldown
unsigned long motionStartTime = 0;
unsigned long visitorStartTime = 0; // NEW: track Hello display duration
const unsigned long visitorDisplayDuration = 10000; // 10s

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to WiFi!");
}

String getTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Time Error";
  char timeStr[20];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  return String(timeStr);
}

String getGreeting() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Hello";
  int hour = timeinfo.tm_hour;
  if(hour >=5 && hour <12) return "Good Morning";
  else if(hour >=12 && hour <18) return "Good Afternoon";
  else if(hour >=18 && hour <23) return "Good Evening";
  else return "Good Night";
}

void sendTelegram(String msg) {
  bot.sendMessage(CHAT_ID, msg, "");
}

long readUltrasonicCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  long distance = duration * 0.034 / 2; // cm
  return distance;
}

void updateDisplay(bool visitor) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Time: ");
  display.println(getTime());
  display.setCursor(0, 16);
  display.println(getGreeting());

  if(visitor){
    display.setTextSize(2);
    display.setCursor(10, 40);
    display.println("Hello!");
  }

  display.display();
}

void setup() {
  Serial.begin(115200);
  pinMode(IRSENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED init failed!");
    for (;;);
  }
  display.clearDisplay();
  display.display();

  initWiFi();
  client.setInsecure(); // Telegram uses HTTPS

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("System Initializing...");
  display.display();
  delay(2000);
}

void loop() {
  bool irState = digitalRead(IRSENSOR);
  long distance = readUltrasonicCM();
  bool visitor = false;

  if(irState){
    if(motionStartTime == 0) motionStartTime = millis();
    // check ultrasonic distance and 3 sec stable
    if((millis() - motionStartTime >= 3000) && (distance >=20 && distance <=70) && !visitorDetected && (millis() - lastNotifyTime > notifyCooldown)){
      visitorDetected = true;
      lastNotifyTime = millis();
      visitorStartTime = millis(); // start 10s display timer
      sendTelegram("Visitor Detected at your door!");
      Serial.println("Visitor Detected!");
    }
  } else {
    motionStartTime = 0;
    visitorDetected = false;
  }

  // Keep Hello displayed for 10 seconds
  if(visitorDetected && (millis() - visitorStartTime <= visitorDisplayDuration)){
    visitor = true;
  } else {
    visitor = false;
  }

  updateDisplay(visitor);
  delay(500);
}
