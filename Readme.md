# 🛡️ RescueVault: Secure. Search. Save.

RescueVault is a decentralized, offline-capable disaster response communication system designed to provide a **critical communication link in areas with zero cellular connectivity**.  
It utilizes a **Pull Architecture over Long-Range WiFi**, **hardware-level encryption**, **machine learning**, and **GPS integration** to enable secure, resilient emergency communication without relying on any existing network infrastructure.

---

## 📌 Project Summary

RescueVault enables reliable emergency coordination in disaster-stricken environments where traditional networks collapse. Victims submit SOS reports through a locally hosted tactical hotspot, which are securely encrypted, geotagged, analyzed using machine learning, and delivered to responders via role-specific dashboards for faster, smarter decision-making.

---

## 🚀 Features

- **Offline SOS Hotspot**  
  Victims connect to a WiFi network (`SENTINEL_RESCUE_SOS`) hosted by the Edge Node to submit emergency reports.

- **Automated GPS Geotagging**  
  Every report is automatically appended with precise GPS coordinates from the NEO-6M module.

- **Hardware-Accelerated Encryption**  
  Secure transmission using a **Shrike-lite (RP2040 + FPGA)** board implementing **SIMON / ASCON** cryptographic cores.

- **Machine Learning–Assisted Analysis**  
  Incoming victim data is processed using ML models to assist in **priority classification**, **severity estimation**, and **resource allocation support**.

- **Role-Based Tactical Dashboards**  
  Separate dashboards are provided for **commanders**, **medical teams**, and **field responders**, each showing tailored views of relevant data.

- **Live Command Center**  
  A Python-based bridge visualizes real-time data through dynamic web dashboards.

---

## 🧠 Novelty and Key Innovations

RescueVault introduces a **multi-layered innovation stack** combining resilient networking, hardware security, machine learning, and operational visualization.

### 🔐 Hardware–Software Co-Design for Security
Unlike conventional IoT disaster systems that rely purely on software encryption, RescueVault integrates a **Shrike-lite (RP2040 + FPGA)** board.  
Hardware-accelerated **SIMON / ASCON ciphers** ensure that sensitive victim data is encrypted **before it ever leaves the device**, significantly reducing attack surfaces.

### 📡 Resilient Pull Architecture
RescueVault uses a **Pull-based communication model**, where the Command Center actively fetches data from the field node.  
This approach:
- Avoids unreliable continuous uplinks  
- Performs better in noisy RF environments  
- Ensures data consistency even under intermittent connectivity

### 🤖 Machine Learning–Enhanced Decision Support
RescueVault incorporates **machine learning models** to assist responders by:
- Classifying SOS messages by **urgency and severity**
- Flagging high-risk cases (e.g., medical emergencies, immobility indicators)
- Supporting intelligent triage and dispatch prioritization

ML operates **locally** within the command environment, preserving offline capability and data privacy.

### 🌐 Zero-Infrastructure Dependency
The entire system functions without:
- Cellular networks
- Internet connectivity
- Cloud services

From the victim’s smartphone to encrypted hardware transmission, ML analysis, and responder dashboards, RescueVault operates as a **self-contained tactical ecosystem**.

### 📍 Automated Geolocation Integration
GPS data from the **NEO-6M module** is automatically fused with victim-submitted information, ensuring responders receive **accurate and actionable location intelligence** even when victims are disoriented or unable to communicate clearly.

### 🧭 Role-Based Situational Awareness
Instead of a single monolithic dashboard, RescueVault provides **dedicated dashboards** for different operational teams:
- **Command Dashboard:** System-wide overview, priority alerts, resource coordination
- **Medical Dashboard:** Victim condition, triage status, medical urgency
- **Field Responder Dashboard:** Location-centric views and assignment details

This separation reduces cognitive overload and improves response efficiency.

---

## 🛠️ Core Architecture

### 1. Edge Node (Field Unit)

- **Microcontroller:** Arduino Mega 2560
- **Communication:** ESP8266 (NodeMCU)
- **Positioning:** NEO-6M GPS Module
- **Security:** Shrike-lite (RP2040 + FPGA)
- **Function:**  
  Hosts the tactical hotspot (`SENTINEL_RESCUE_SOS`) and stores encrypted victim reports in `victims.json`.

---

### 2. Gateway Node (Command Center)

- **Microcontroller:** ESP32 or ESP8266
- **Connectivity:** USB Serial → Windows Laptop
- **Function:**  
  Actively pulls encrypted data from the Edge Node for processing.

---

### 3. Command & ML Layer

- **Backend:** Python bridge (`pyserial`)
- **ML Processing:** Local inference for prioritization and classification
- **Frontend:** Multiple role-specific web dashboards
- **Function:**  
  Converts raw SOS data into actionable intelligence.

---

## 🔌 Pin Connections

### Arduino Mega ↔ ESP8266

| Mega Pin | ESP8266 Pin | Notes |
|--------|-------------|------|
| TX1 (18) | D7 (GPIO 13) | ⚠️ Voltage divider required |
| RX1 (19) | D6 (GPIO 12) | Direct |
| 5V / GND | VIN / GND | Common power |

---

### Arduino Mega ↔ Shrike-lite (FPGA)

| Mega Pin | FPGA / RP2040 | Notes |
|--------|---------------|------|
| SCK (52) | SCK | SPI clock |
| MOSI (51) | MOSI | Plaintext |
| MISO (50) | MISO | Ciphertext |
| CS (53) | CS | Chip select |

---

### Arduino Mega ↔ GPS Module

| GPS Pin | Mega Pin |
|-------|----------|
| TX | RX2 (17) |
| RX | TX2 (16) |
| VCC / GND | 5V / GND |

---

## 💻 Software Setup

### Edge Node
- **Code:** `Edge_Node_ESP/src/main.cpp`
- **Endpoint:** `/api/victims`

### FPGA Encryption
- **Language:** Verilog
- **Ciphers:** SIMON / ASCON
- **SPI Bridge:** RP2040

### Command Dashboard & ML
- **Requirements:** Python 3.x, `pyserial`, ML dependencies
- **Run:**
```bash
python esp_bridge.py
