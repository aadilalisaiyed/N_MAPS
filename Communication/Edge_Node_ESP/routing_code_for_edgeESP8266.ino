#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h> 

// --- SECURITY SETTINGS (Added by Teammate) ---
const int MAX_USERS = 5;
unsigned long lastSubmitTime = 0;
const int SUBMIT_COOLDOWN = 5000; // 5 Seconds delay between posts

const char* ssid = "SENTINEL_RESCUE_SOS";
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

// --- HTML UI (With Honeypot Security) ---
const char SOS_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SENTINEL SOS</title>
    <style>
        body { background: #000; color: #0f0; font-family: 'Courier New', monospace; margin: 0; padding: 15px; }
        .box { border: 2px solid #0f0; padding: 15px; border-radius: 5px; }
        h2 { color: #f00; text-transform: uppercase; border-bottom: 1px solid #f00; padding-bottom: 5px; margin-top: 0; text-align: center; }
        label { display: block; margin-top: 15px; font-weight: bold; font-size: 0.9em; }
        input, select, textarea { width: 100%; padding: 12px; margin-top: 5px; background: #111; border: 1px solid #0f0; color: #fff; font-size: 16px; box-sizing: border-box; }
        /* HIDDEN HONEYPOT FIELD */
        .hp-field { display: none !important; }
        .map { height: 120px; background: #050505; border: 1px solid #004400; margin: 15px 0; background-image: linear-gradient(#002200 1px, transparent 1px), linear-gradient(90deg, #002200 1px, transparent 1px); background-size: 20px 20px; display: flex; align-items: center; justify-content: center; }
        .dot { width: 10px; height: 10px; background: red; border-radius: 50%; box-shadow: 0 0 10px red; }
        .row { display: flex; gap: 10px; } .col { flex: 1; }
        .tactical-requirement-grid { display: flex; flex-direction: column; gap: 12px; margin-top: 10px; }
        .req-box input { display: none; }
        .visual-btn { background: #1a1a1a; border: 3px solid #444; color: #888; padding: 20px; font-size: 1.1em; font-weight: bold; text-align: center; border-radius: 10px; cursor: pointer; display: flex; align-items: center; justify-content: center; gap: 15px; }
        .req-box input:checked + .visual-btn { background: #004400; border-color: #00ff00; color: #00ff00; box-shadow: 0 0 20px rgba(0, 255, 0, 0.4); }
        .btn-main { width: 100%; padding: 18px; background: #f00; color: #fff; border: none; font-size: 1.3em; font-weight: bold; margin-top: 30px; cursor: pointer; text-transform: uppercase; border-radius: 5px; }
        .footer-note { font-size: 0.7em; color: #555; text-align: center; margin-top: 20px; }
    </style>
</head>
<body>
    <div class="box">
        <h2>⚠ SOS SIGNAL</h2>
        <div class="map"><div class="dot"></div></div>
        <form action="/submit" method="POST">
            <input type="text" name="hp_verify" class="hp-field"> 
            
            <label>NAME (नाम)</label><input type="text" name="name" required>
            <div class="row">
                <div class="col"><label>GENDER</label><select name="gender"><option>Male</option><option>Female</option></select></div>
                <div class="col"><label>PEOPLE</label><input type="number" name="count" value="1"></div>
            </div>
            <label>PHONE</label><input type="tel" name="phone" required>
            <label>SITUATION</label><textarea name="situ"></textarea>
            <div class="tactical-requirement-grid">
                <label class="req-box"><input type="checkbox" name="n1" value="Medical"><div class="visual-btn">➕ MEDICAL</div></label>
                <label class="req-box"><input type="checkbox" name="n2" value="Food"><div class="visual-btn">🍞 FOOD</div></label>
                <label class="req-box"><input type="checkbox" name="n3" value="Rescue"><div class="visual-btn">🆘 RESCUE</div></label>
            </div>
            <button type="submit" class="btn-main">SEND SOS</button>
        </form>
    </div>
</body>
</html>
)rawliteral";

// --- SAVE FUNCTION (JSON FORMAT) ---
void logData(String jsonEntry) {
    // We save to "victims.json" so ESP_2 and Python can find it
    File logFile = LittleFS.open("/victims.json", "a"); 
    if (!logFile) {
        Serial.println("Failed to open file for logging");
        return;
    }
    logFile.println(jsonEntry + ","); // Add comma for the array
    logFile.close();
    Serial.println("💾 DATA SAVED: " + jsonEntry);
}

void handleSubmit() {
    // --- 1. SECURITY CHECKS (Honeypot + Cooldown) ---
    if (server.arg("hp_verify") != "" || (millis() - lastSubmitTime < SUBMIT_COOLDOWN)) {
        server.send(403, "text/plain", "Action Blocked (Spam/Bot Detected)");
        Serial.println("⛔ Security Block Triggered");
        return;
    }
    lastSubmitTime = millis();

    // --- 2. DATA PREPARATION (Convert Form to JSON) ---
    // This is crucial so your Python script works!
    String name = server.arg("name");
    String gender = server.arg("gender");
    String count = server.arg("count");
    String phone = server.arg("phone");
    String situ = server.arg("situ");
    
    String needs = "";
    if (server.hasArg("n1")) needs += "Medical ";
    if (server.hasArg("n2")) needs += "Food ";
    if (server.hasArg("n3")) needs += "Rescue";
    
    // Create Valid JSON
    String jsonEntry = "{";
    jsonEntry += "\"name\":\"" + name + "\",";
    jsonEntry += "\"gender\":\"" + gender + "\",";
    jsonEntry += "\"count\":\"" + count + "\",";
    jsonEntry += "\"phone\":\"" + phone + "\",";
    jsonEntry += "\"situation\":\"" + situ + "\",";
    jsonEntry += "\"needs\":\"" + needs + "\"";
    jsonEntry += "}";

    // --- 3. SAVE ---
    logData(jsonEntry);

    server.send(200, "text/html", "<h1>✔ SOS SENT</h1><p>Stay calm. Help is coming.</p><a href='/'>BACK</a>");
}

// --- ADMIN VIEW (Standard Route) ---
void handleView() {
    // We keep the route as "/view" so ESP_2 can find it
    if (LittleFS.exists("/victims.json")) {
        File f = LittleFS.open("/victims.json", "r");
        server.streamFile(f, "application/json");
        f.close();
    } else {
        server.send(200, "text/plain", "No data found.");
    }
}

// --- CLEAR DATA ---
void handleClear() {
    LittleFS.remove("/victims.json");
    server.send(200, "text/plain", "Logs Wiped.");
}

void setup() {
    Serial.begin(115200);
    
    if(!LittleFS.begin()){
        Serial.println("Formatting LittleFS...");
        LittleFS.format();
        LittleFS.begin();
    }

    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    // MAX_USERS logic applied here
    WiFi.softAP(ssid, "", 1, 0, MAX_USERS); 
    
    dnsServer.start(53, "*", apIP);

    server.on("/", []() { server.send(200, "text/html", SOS_HTML); });
    server.on("/submit", HTTP_POST, handleSubmit);
    
    // Kept standard routes so your Dashboard Works
    server.on("/view", HTTP_GET, handleView); 
    server.on("/clear", HTTP_GET, handleClear);

    server.onNotFound([]() {
        server.sendHeader("Location", String("http://") + apIP.toString(), true);
        server.send(302, "text/plain", "");
    });

    server.begin();
    Serial.println("SENTINEL SECURE NODE ONLINE.");
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
}
