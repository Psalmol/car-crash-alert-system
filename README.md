esp32-crash-detection/
│── src/              # Main source code (.ino or .cpp/.h files)
│── docs/             # Diagrams, wiring schematics, flowcharts
│── images/           # Project photos/screenshots
│── README.md         # Main documentation
│── platformio.ini    



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
git clone https://github.com/psalmol/esp32-crash-detection.git
cd esp32-crash-detection

##  🔄 System Flow

1. **Startup**
   - ESP32 initializes all sensors (LSM6DS3, VL53L0Xs, SIM800L, GPS if enabled).
   - Each VL53L0X is assigned a unique I2C address using XSHUT pins.

2. **Continuous Monitoring**
   - IMU (LSM6DS3) monitors acceleration and angular velocity.
   - VL53L0X sensors continuously measure distances (front, left, right, rear).

3. **Crash Detection**
   - If acceleration exceeds threshold → crash detected.
   - ESP32 triggers GSM call/SMS alert via SIM800L.
   - Alert message includes predefined text (and GPS location if active).

4. **Proximity Warning**
   - If any VL53L0X reading < threshold distance → buzzer activates.
   - Buzzer deactivates once distance is safe.

5. **GPS On-Demand**
   - SIM800L listens for incoming SMS.
   - If message contains keyword **“GPS”** → ESP32 retrieves current location.
   - Sends SMS with Google Maps link:  
     `https://maps.google.com/?q=latitude,longitude`

6. **Standby / Repeat**
   - Loop continues until power is turned off.

 
 ## PlatformIO Project Configuration File

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino

; Serial monitor speed
monitor_speed = 115200

; Libraries used
lib_deps =
    adafruit/Adafruit Unified Sensor @ ^1.1.14
    adafruit/Adafruit LSM6DS @ ^3.0.7
    adafruit/Adafruit VL53L0X @ ^1.3.1
    mikalhart/TinyGPSPlus @ ^1.0.3

