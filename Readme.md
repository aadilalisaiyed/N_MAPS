# 🛡️ RescueVault: Secure. Search. Save.

RescueVault is a robust, offline-capable disaster response communication system designed to aid rescue operations in areas with **zero cellular connectivity**.  
It uses **Long-Range WiFi (HTTP Pull Architecture)** and **GPS** to create a decentralized network for victims to send SOS signals directly to a command center.

---

## 🚀 Features

- **Offline SOS Hotspot**  
  Victims connect to a WiFi network (`SENTINEL_RESCUE_SOS`) hosted by the Edge Node to submit their status.

- **Tactical Dashboard**  
  Dark-mode, high-contrast HTML interface optimized for low-power devices.

- **GPS Tracking**  
  Automatically appends precise GPS coordinates to every victim report.

- **Hardware-Accelerated Encryption**  
  Uses a **Shrike-lite (RP2040 + FPGA)** board implementing **SIMON / ASCON** ciphers for secure transmission.

- **Live Command Center**  
  Windows-based Python bridge visualizes victim data in real time by monitoring local JSON updates.

---

## 🛠️ Hardware Architecture

RescueVault is built on a **modular three-part architecture**.

### 1. Edge Node (Field Unit)

- **Microcontroller:** Arduino Mega 2560 (Central Hub)
- **Communication:** ESP8266 (NodeMCU) – hosts web interface
- **Positioning:** NEO-6M GPS Module
- **Security:** Shrike-lite FPGA (SIMON cipher acceleration)
- **Role:**  
  Collects GPS + encrypted victim data and stores reports in a local `victims.json` file.

---

### 2. Gateway Node (Command Center)

- **Microcontroller:** ESP32 or ESP8266
- **Connectivity:** USB Serial → Laptop
- **Role:**  
  Wirelessly connects to the Edge Node and **pulls** data into the command system.

---

## 🔌 Pin Connections

### Arduino Mega ↔ ESP8266 (Edge Node)

| Mega Pin | ESP8266 Pin | Notes |
|--------|-------------|------|
| TX1 (18) | D7 (GPIO 13) | ⚠️ Requires voltage divider (1kΩ / 2kΩ) |
| RX1 (19) | D6 (GPIO 12) | Direct connection |
| 5V / GND | VIN / GND | Common power and ground |

---

### Arduino Mega ↔ Shrike-lite (FPGA)

| Mega Pin | FPGA / RP2040 Pin | Notes |
|--------|------------------|------|
| SCK (52) | SCK | SPI clock |
| MOSI (51) | TX / MOSI | Data to cipher |
| MISO (50) | RX / MISO | Ciphertext output |
| CS (53) | CS | Chip select |

---

### Arduino Mega ↔ GPS Module

| GPS Pin | Mega Pin | Notes |
|-------|----------|------|
| TX | RX2 (17) | Serial data |
| RX | TX2 (16) | Command data |
| VCC / GND | 5V / GND | Power supply |

---

## 💻 Software Setup

### 1. Edge Node (ESP8266)

- **Code:** `Edge_Node_ESP/src/main.cpp`
- **Upload:** Flash to NodeMCU connected to the Arduino Mega
- **Function:**  
  Hosts SOS webpage and API endpoint:
  
---

### 2. FPGA Encryption (Verilog)

- **Implementation:** SIMON / ASCON cipher cores on FPGA fabric
- **Bridge:** RP2040 handles SPI communication between Arduino Mega and FPGA

---

### 3. Command Dashboard (Python)

- **Requirement:** Python 3.x, `pyserial`
- **Run:**
```bash
python esp_bridge.py

If you want, I can also:
- Split this into multiple `.md` files (hardware, software, crypto)
- Add architecture diagrams (Mermaid)
- Turn this into a GitHub-polished README with badges and TOC

