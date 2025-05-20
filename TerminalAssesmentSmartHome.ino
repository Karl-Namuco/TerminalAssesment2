#include <WiFi.h>
#include <FirebaseESP32.h>
#include <HTTPClient.h>

// WiFi credentials
#define WIFI_SSID "hi"
#define WIFI_PASSWORD "12345678"

// Firebase credentials
#define API_KEY "AIzaSyDaG2PeRuGL7T8SwFxm_q88pBPMkUpbk5A"
#define DATABASE_URL "https://terminalassesment2-aad1d-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_AUTH "Q1PJ90hDTCehOtcKgdL4UoH0s1WuB7U3lCKxqy7F"

// ThingSpeak API
const char* THINGSPEAK_API_KEY = "2ZNBQJS5RU27QDSN";
const char* THINGSPEAK_URL = "http://api.thingspeak.com/update";

// LED pins
#define LED1_PIN 16
#define LED2_PIN 17
#define LED3_PIN 18
#define LED4_PIN 19

// Sensor pins
#define MQ2_PIN 34
#define MQ135_PIN 35

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  // Set LED pins as output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Wait for Firebase to be ready
  while (!Firebase.ready()) {
    Serial.println("Waiting for Firebase...");
    delay(1000);
  }

  Serial.println("Firebase is ready!");
}

void loop() {
  if (Firebase.ready()) {
    // Update LEDs from Firebase
    readAndSetLED("/led1", LED1_PIN);
    readAndSetLED("/led2", LED2_PIN);
    readAndSetLED("/led3", LED3_PIN);
    readAndSetLED("/led4", LED4_PIN);

    // Read sensor values
    int mq2 = analogRead(MQ2_PIN);
    int mq135 = analogRead(MQ135_PIN);

    Serial.printf("MQ2: %d, MQ135: %d\n", mq2, mq135);

    // Determine status
    String gasStatus = (mq2 > 5000) ? "Danger" : "Safe";
    String airStatus = (mq135 > 450) ? "Not Clean" : "Clean";

    // Send status to Firebase
    Firebase.RTDB.setString(&fbdo, "/sensor/gas_status", gasStatus);
    Firebase.RTDB.setString(&fbdo, "/sensor/air_status", airStatus);

    // Optional: Send raw values to Firebase
    Firebase.RTDB.setInt(&fbdo, "/sensor/mq2", mq2);
    Firebase.RTDB.setInt(&fbdo, "/sensor/mq135", mq135);

    // Send data to ThingSpeak
    HTTPClient http;
    String url = String(THINGSPEAK_URL) + "?api_key=" + THINGSPEAK_API_KEY +
                 "&field1=" + String(mq2) +
                 "&field2=" + String(mq135);
    http.begin(url);
    http.GET();
    http.end();
  }

  delay(100); // 5-second update interval
}

void readAndSetLED(const String& path, int pin) {
  if (Firebase.RTDB.getBool(&fbdo, path)) {
    digitalWrite(pin, fbdo.boolData() ? HIGH : LOW);
  }
}
