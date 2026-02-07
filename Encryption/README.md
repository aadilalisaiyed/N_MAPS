PRESENT Cipher: Hardware-Accelerated Lightweight Encryption
Module: Security & Cryptography (Project SENTINEL-VAULT)
 Overview
This folder contains the implementation of the PRESENT Cipher, a state-of-the-art lightweight block cipher. In the context of Project SENTINEL-VAULT, this module serves as the Hardware Root of Trust (HRoT), ensuring that sensitive intelligence and SOS data are mathematically locked before they ever touch a storage medium.

Technical Architecture
We have implemented the PRESENT-80 variant, optimized for FPGA logic:

Block Size: 64 bits (Handles data in compact, efficient chunks).

Key Length: 80 bits (Provides a massive 1.2 decillion possible combinations).

Structure: Substitution-Permutation Network (SPN) with 31 rounds of transformation.

Hardware Efficiency: Unlike AES, which requires significant memory (RAM) for S-Boxes, PRESENT is designed to run entirely on Combinational Logic Gates, making it perfect for the Vicharak Shrike FPGA.

Why PRESENT instead of AES?
In a tactical deployment scenario, power and speed are everything. We chose PRESENT for three strategic reasons:

Low Footprint: PRESENT uses approximately 5x fewer logic gates than AES-128, leaving more FPGA space for our Anti-Tamper and Electronic Warfare modules.

Ultra-Low Latency: Encryption happens in real-time clock cycles, ensuring that SOS messages are secured the microsecond they are received via UART.

Hardware-Native: It is specifically designed for constrained environments (IoT/Defense), where traditional software encryption (like Python/C++ libraries) would be too slow or vulnerable to side-channel attacks.

Security Features
Hardware Root of Trust: The encryption keys are hardcoded into the FPGA's bitstream or physical dip-switches. The ESP32 (Software layer) never sees the raw key, preventing leaks via software hacks.

Zero Plaintext Leakage: The ESP32 strips the plaintext and passes it to the FPGA. Only the Ciphertext (Scrambled Data) is sent back to be saved in the SPIFFS memory. Even if the flash memory is dumped, the data is useless without the hardware key.

Lightweight Confusion & Diffusion: Utilizing a 4-bit S-Box for non-linear confusion and a bit-permutation layer for rapid diffusion, ensuring that even a 1-bit change in input results in a completely different ciphertext.
