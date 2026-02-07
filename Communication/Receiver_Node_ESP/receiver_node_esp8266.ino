
/**
 * Project: Sentinel's Vault - Receiver Node (Command Gateway)
 * Platform: ESP32
 * Description: Receives ESP-NOW packets and serializes them to JSON for the Python Dashboard.
 */

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// 1. Define Data Structure (MUST MATCH SENDER)
typedef struct struct_message {
  char name[32];
  char phone[15];
  char situation[64];
  char needs[32];
} struct_message;

struct_message incomingData;

// 2. Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingDataPtr, int len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  
  // --- JSON SERIALIZATION ---
  // This format allows your Python .exe to detect and save the record.
  
  Serial.print("{");
  Serial.print("\"name\":\"");      Serial.print(incomingData.name);      Serial.print("\",");
  Serial.print("\"phone\":\"");     Serial.print(incomingData.phone);     Serial.print("\",");
  Serial.print("\"situation\":\""); Serial.print(incomingData.situation); Serial.print("\",");
  Serial.print("\"needs\":\"");     Serial.print(incomingData.needs);     Serial.print("\"");
  Serial.println("}");  // End of JSON Line

  // Visual Confirmation on the ESP board
  digitalWrite(2, HIGH);
  delay(50);
  digitalWrite(2, LOW);
}

void setup() {
  // Initialize Serial Monitor at the same baud rate as your Python script
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the callback function to handle incoming packets
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  
  Serial.println("✅ GATEWAY ONLINE: Listening for Mesh Packets...");
}

void loop() {
  // The receiver is interrupt-driven; nothing needed in loop.
  delay(1000); 
}
