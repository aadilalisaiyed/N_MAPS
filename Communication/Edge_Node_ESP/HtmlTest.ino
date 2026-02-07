#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h> 
#include <LittleFS.h>
#include <espnow.h>

// --- CONFIGURATION ---
// 🔴 ENTER ESP_2 MAC ADDRESS HERE! 🔴
uint8_t broadcastAddress[] = {0xA0, 0x20, 0xA6, 0x01, 0x02, 0x03}; 

const char* ssid = "RESCUE_NODE"; // Updated Name
const char* password = NULL; 
const byte DNS_PORT = 53;

IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

// Data Packet Structure
typedef struct struct_message {
  char text[200];
} struct_message;

struct_message myData;

// --- 1. SERVE WEBPAGE ---
void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Error: Upload index.html to Data Folder!");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// --- 2. HANDLE SOS & TRANSMIT ---
void handleSave() {
  if (!server.hasArg("plain")) return;
  
  String dataReceived = server.arg("plain"); // The JSON string
  
  // A. Save to Local Storage (LittleFS)
  File f = LittleFS.open("/victims.json", "a");
  if (f) {
    f.println(dataReceived + ",");
    f.close();
  }

  // B. Transmit to ESP_2 (Base Station)
  dataReceived.toCharArray(myData.text, 200);
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  server.send(200, "text/plain", "SECURED & TRANSMITTED");
}

// --- 3. CAPTIVE PORTAL REDIRECT ---
void handleNotFound() {
  server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
  server.send(302, "text/plain", "Redirecting...");
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) res += String((ip >> (8 * i)) & 0xFF) + ".";
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

void setup() {
  Serial.begin(115200);
  
  // 1. Initialize Storage
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Error");
    return;
  }

  // 2. Dual Mode: Station (for ESP-NOW) + AP (for Victims)
  WiFi.mode(WIFI_AP_STA); 

  // 3. Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Failed");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // Sender
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // 4. Start Hotspot
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  
  // 5. Start Servers
  dnsServer.start(DNS_PORT, "*", apIP);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}