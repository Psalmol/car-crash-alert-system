# 🚗 ESP32-Based Car Crash Detection & Alert System

This project implements a crash detection and alert system using an **ESP32 microcontroller**, **accelerometer/gyroscope (LSM6DS3)**, **VL53L0X proximity sensors**, and **SIM800L GSM module**.  
It detects crashes, warns about nearby obstacles, and sends GPS location via SMS when requested.

---

## 📌 Features
- 🚨 **Crash Detection** – detects sudden impact using accelerometer/gyroscope.  
- 📡 **GSM Alerts** – sends SMS with Google Maps link of current location.  
- 📍 **GPS On-Demand** – location sent only when `GPS` keyword is received.  
- 🛑 **Proximity Warning** – warns the driver with buzzer if objects are too close (front, left, right, rear).   

---

## 🛠️ Hardware Components
- ESP32 Dev Module  
- SIM800L GSM Module  
- GY-LSM6DS3 Accelerometer & Gyroscope  
- 4 ultrasonic sensor Sensors (front, left, right, rear)  
- Buzzer  
- Power source (battery pack)  

---

## 📐 Wiring Diagram
## 📌 Pin Mapping

| Component        | ESP32 Pin | Notes                          |
|------------------|-----------|--------------------------------|
| SIM800L (TX)     | GPIO16    | ESP32 RX (HardwareSerial)      |
| SIM800L (RX)     | GPIO17    | ESP32 TX (HardwareSerial)      |
| LSM6DS3 (SDA)    | GPIO21    | I2C SDA                        |
| LSM6DS3 (SCL)    | GPIO22    | I2C SCL                        |
| VL53L0X Front    | GPIO23    | Custom I2C addr via XSHUT      |
| VL53L0X Left     | GPIO19    | Custom I2C addr via XSHUT      |
| VL53L0X Right    | GPIO18    | Custom I2C addr via XSHUT      |
| VL53L0X Rear     | GPIO5     | Custom I2C addr via XSHUT      |
| Buzzer           | GPIO25    | Active buzzer for warnings     |
| GPS (TX)         | GPIO34    | ESP32 RX2 (if external GPS used) |
| GPS (RX)         | GPIO35    | ESP32 TX2 (if needed)          |
| Power Source     | VIN / 5V  | Separate supply recommended    |


---

## 💻 Software
- Developed using **PlatformIO** with **ESP-IDF/Arduino framework**  
- Libraries used:
  - `Wire.h` (I2C communication)  
  - `TinyGPSPlus` (GPS parsing)  
  - `Adafruit_LSM6DS3` (IMU handling)  
  - `VL53L0X` (proximity sensors)  

---

## ⚙️ How It Works
1. System continuously monitors acceleration and gyroscope data.  
2. If crash threshold is exceeded → system triggers GSM call or SMS alert.  
3. VL53L0X sensors measure distance; if below threshold → buzzer warning.  
4. User can send `GPS` keyword → ESP32 replies with location via SMS.  

---


## 🚀 Getting Started
### Clone Repository
```bash
git clone https://github.com/psalmol/esp32-crash-detection.git
cd esp32-crash-detection
