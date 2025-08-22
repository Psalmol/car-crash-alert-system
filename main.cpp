#include <Wire.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <Adafruit_LSM6DS3.h>
#include <Adafruit_Sensor.h>

//Pin Mapping 
#define BUZZER_PIN  25
#define ULTRA_FRONT_TRIG  13
#define ULTRA_FRONT_ECHO  34
#define ULTRA_LEFT_TRIG   14
#define ULTRA_LEFT_ECHO   35
#define ULTRA_RIGHT_TRIG  27
#define ULTRA_RIGHT_ECHO  32
#define ULTRA_REAR_TRIG   26
#define ULTRA_REAR_ECHO   3
#define SIM800_TX 16   // ESP32 RX
#define SIM800_RX 17   // ESP32 TX
#define GPS_RX  34
#define GPS_TX  35

// Creating objects from the library
Adafruit_LSM6DS3 lsm6ds3;
HardwareSerial sim800(1);    // GSM
HardwareSerial gpsSerial(2); // GPS
TinyGPSPlus gps;

// setting threshold
// this needs to be calibrated experimentally
const float CRASH_ACCEL_THRESHOLD = 25.0;   
const int PROXIMITY_THRESHOLD = 20;        


void sendSMS(String msg) {
  sim800.println("AT+CMGF=1");
  delay(100);
  sim800.println("AT+CMGS=\"+234............\""); // actual phone number
  delay(200);
  sim800.print(msg);
  sim800.write(26); // CTRL+Z to send
  delay(2000);
}

long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.034 / 2;          // Convert to cm
  return distance;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Ultrasonic pins
  pinMode(ULTRA_FRONT_TRIG, OUTPUT); pinMode(ULTRA_FRONT_ECHO, INPUT);
  pinMode(ULTRA_LEFT_TRIG, OUTPUT);  pinMode(ULTRA_LEFT_ECHO, INPUT);
  pinMode(ULTRA_RIGHT_TRIG, OUTPUT); pinMode(ULTRA_RIGHT_ECHO, INPUT);
  pinMode(ULTRA_REAR_TRIG, OUTPUT);  pinMode(ULTRA_REAR_ECHO, INPUT);

  // GSM
  sim800.begin(9600, SERIAL_8N1, SIM800_RX, SIM800_TX);

  // GPS
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  // IMU
  if (!lsm6ds3.begin_I2C()) {
    Serial.println("LSM6DS3 not detected!");
    while (1);
  }

  Serial.println("Crash Detection System Ready");
}

void loop() {
  // 1. Crash Detection
  sensors_event_t accel, gyro, temp;
  lsm6ds3.getEvent(&accel, &gyro, &temp);

  float accMagnitude = sqrt(pow(accel.acceleration.x, 2) +
                            pow(accel.acceleration.y, 2) +
                            pow(accel.acceleration.z, 2));

  if (accMagnitude > CRASH_ACCEL_THRESHOLD) {
    Serial.println("Crash detected!");
    sendSMS("🚨 Crash Detected! Vehicle may have collided.");
    delay(5000);
  }

  // 2. Proximity Warning
  long dFront = readUltrasonic(ULTRA_FRONT_TRIG, ULTRA_FRONT_ECHO);
  long dLeft  = readUltrasonic(ULTRA_LEFT_TRIG, ULTRA_LEFT_ECHO);
  long dRight = readUltrasonic(ULTRA_RIGHT_TRIG, ULTRA_RIGHT_ECHO);
  long dRear  = readUltrasonic(ULTRA_REAR_TRIG, ULTRA_REAR_ECHO);

  if ((dFront > 0 && dFront < PROXIMITY_THRESHOLD) ||
      (dLeft  > 0 && dLeft  < PROXIMITY_THRESHOLD) ||
      (dRight > 0 && dRight < PROXIMITY_THRESHOLD) ||
      (dRear  > 0 && dRear  < PROXIMITY_THRESHOLD)) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // 3. GPS On-Demand via SMS
  while (sim800.available()) {
    String msg = sim800.readString();
    msg.toUpperCase();
    if (msg.indexOf("GPS") >= 0) {
      if (gps.location.isUpdated()) {
        String link = "https://maps.google.com/?q=" + 
                       String(gps.location.lat(), 6) + "," + 
                       String(gps.location.lng(), 6);
        sendSMS("📍 Location: " + link);
      } else {
        sendSMS("GPS not fixed yet. Try again.");
      }
    }
  }

  // 4. Update GPS parser
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  delay(200);
}
