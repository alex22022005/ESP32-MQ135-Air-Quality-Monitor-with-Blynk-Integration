// ------------------------------------
// BLYNK and WiFi Credentials
// ------------------------------------
#define BLYNK_TEMPLATE_ID "TMPL3iOfCYLMh"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitor"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "MQ135.h"

char auth[] = "hlLJdmYSjKWRKH0dFCOrWIIUNQD_EziC";
char ssid[] = "realme 3i";
char pass[] = "PIMWAEU1";

// ------------------------------------
// Pin Configuration
// ------------------------------------
#define MQ135_PIN 34    // Analog pin connected to MQ135 Sensor

// Create sensor object
MQ135 gasSensor(MQ135_PIN);

// Variable for manually setting Ro after calibration
float Ro = 10.0;  // Will update later after calibration

// Constants for sensor calculation
const float RL = 10.0;    // Load resistance in kilo-ohms
const float ADC_MAX = 4095.0; // ADC max value for ESP32
const float VCC = 3.3;    // ESP32 operating voltage

bool isCalibrated = false;

// ------------------------------------
// Setup Function
// ------------------------------------
void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Starting Setup...");

  Blynk.begin(auth, ssid, pass);
  pinMode(MQ135_PIN, INPUT);

  Serial.println("Wait for calibration in CLEAN air...");
  delay(5000); // Give sensor time to warm up

  Serial.println("\n=== Air Quality Levels ===");
  Serial.println("Good Quality     : 0 - 100 PPM");
  Serial.println("Moderate Quality : 101 - 300 PPM");
  Serial.println("Poor Quality     : 301+ PPM\n");

  // Start calibration
  calibrateSensor();
}

// ------------------------------------
// Main Loop Function
// ------------------------------------
void loop() {
  Blynk.run();
  
  if (!isCalibrated) {
    return; // Wait until calibration is done
  }

  // Read analog value
  float sensorValue = analogRead(MQ135_PIN);

  // Calculate Rs
  float Vout = (sensorValue / ADC_MAX) * VCC;
  float Rs = ((VCC - Vout) / Vout) * RL; // Ohm's law for sensor

  // Calculate gas concentration
  float ratio = Rs / Ro;  // Rs/Ro
  float ppm = 116.6020682 * pow(ratio, -2.769034857); // Default curve for CO2
  float ben = ppm*0.2957;
  
  // Display on Serial
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  Serial.print("CO2 Concentration (PPM): ");
  Serial.println(ppm, 2);

  Serial.print("Benzene Concentration (PPM): ");
  Serial.println(ppm, 2);

  // Air Quality Status
  String airStatus;

  if (ppm>0 && ppm <= 100) {
    airStatus = "Good";
  } else if (ppm>100 && ppm <= 300) {
    airStatus = "Moderate";
  } else {
    airStatus = "Poor";
  }

  Serial.print("Air Status: ");
  Serial.println(airStatus);

  Serial.print("CO2 Level: ");
  Serial.print(ppm, 2);
  Serial.println(" PPM");

  Serial.print("Benzene Level: ");
  Serial.print(ppm, 2);
  Serial.println(" PPM");

  Serial.println("----------------------------------");

  // Send to Blynk
  Blynk.virtualWrite(V0, ppm); 
  Blynk.virtualWrite(V1, ben);
  Blynk.virtualWrite(V2, airStatus);

  delay(2000);
}

// ------------------------------------
// Calibration Function
// ------------------------------------
void calibrateSensor() {
  float avgSensorValue = 0;
  int samples = 50; // Take 50 samples for better average

  for (int i = 0; i < samples; i++) {
    avgSensorValue += analogRead(MQ135_PIN);
    delay(100);
  }
  avgSensorValue /= samples;

  Serial.print("Average Sensor Value (clean air): ");
  Serial.println(avgSensorValue);

  // Calculate Rs
  float Vout = (avgSensorValue / ADC_MAX) * VCC;
  float Rs = ((VCC - Vout) / Vout) * RL;

  Serial.print("Calculated Rs: ");
  Serial.print(Rs);
  Serial.println(" KOhms");

  // According to datasheet, in clean air Rs/Ro = 3.6
  Ro = Rs / 3.6;

  Serial.print(">>> Calibrated Ro: ");
  Serial.print(Ro);
  Serial.println(" KOhms");

  Serial.println("Calibration Done!\n\n");

  isCalibrated = true;
}