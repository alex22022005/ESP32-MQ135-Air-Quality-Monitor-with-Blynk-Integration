# 🌳 ESP32 MQ135 Air Quality Monitor with Blynk

An IoT project that uses an ESP32 and an MQ135 gas sensor to monitor air quality. It measures CO2 and Benzene concentrations (PPM) and sends the data to the Blynk IoT platform for easy remote monitoring.

!

## Features

* **Real-time Monitoring:** Tracks CO2 and Benzene levels in Parts Per Million (PPM).
* **Air Quality Status:** Classifies the air as "Good," "Moderate," or "Poor."
* **Blynk Integration:** Sends all data (CO2, Benzene, Status) to the Blynk cloud.
* **Automatic Calibration:** Includes a function to automatically calibrate the sensor's baseline resistance (`Ro`) in clean air on startup.

---

## 🛠️ Hardware Requirements

* ESP32 Development Board
* MQ135 Gas Sensor Module
* Jumper Wires

## ⚙️ Software & Libraries

* [Arduino IDE](https://www.arduino.cc/en/software) or [VS Code with PlatformIO](https://platformio.org/)
* [Blynk Account](https://blynk.io/)
* **Arduino Libraries:**
    * `WiFi.h` (Comes with ESP32 board package)
    * `BlynkSimpleEsp32.h` (Install via Library Manager)

---

## 🔌 Hardware Setup

Connect the MQ135 sensor to your ESP32 as follows.

| MQ135 Pin | ESP32 Pin |
| :--- | :--- |
| **VCC** | `3.3V` |
| **GND** | `GND` |
| **AOUT** | `34` (or any ADC pin) |
| **DOUT** | *Not Used* |

---

## 🚀 Getting Started

### 1. Set up Blynk

1.  Log in to your [Blynk Account](https://blynk.io/).
2.  Create a new **Template** (e.g., "Air Quality Monitor").
3.  In the template, create three **Datastreams**:
    * **V0:** `CO2 Level` (Type: `Integer`, Units: `PPM`, Min: `0`, Max: `5000`)
    * **V1:** `Benzene Level` (Type: `Float`, Units: `PPM`, Min: `0`, Max: `100`)
    * **V2:** `Air Status` (Type: `String`)
4.  Create a **Device** from your new template.
5.  Your **Blynk Template ID**, **Template Name**, and **Auth Token** will be available on the "Device Info" tab.

### 2. Configure the Code

Open the `.ino` project sketch and update the following credentials at the top of the file.

**Update Blynk Credentials:**
```cpp
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
```
## Update WiFi Credentials:
```cpp
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";
```
### 3. Upload and Calibrate

1.  Warm-up: When you first power on the sensor, it needs to warm up for 5-10 minutes to get stable readings.

2.  Calibration:

    Place the sensor in an area with clean, fresh air (preferably outdoors).

    Power on or reset the ESP32.

    The setup() function will run the calibrateSensor() routine. It takes 50 samples, averages them, and calculates the sensor's base resistance (Ro) in clean air.

    Monitor the Serial Monitor (baud rate 115200) to see the calibration process and the final Ro value.

3.  Run: Once calibrated, the device will start measuring and sending data to Blynk every 2 seconds. The device will recalibrate every time it reboots.
---
## 📄 License
This project is licensed under the MIT License.
