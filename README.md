# 🐝 Smart Pollination & Mist Control System

This repository contains two main parts of the project:

1. **ESP32-based Mist/Humidifier Control System** using an ultrasonic sensor
2. **Raspberry Pi 4 + YOLOv5 Bee Detection System** using a custom-trained model

The system is designed for **smart agriculture / pollination monitoring**, where bee presence is detected and a mist/humidifier is controlled automatically.

---

## 📁 Project Structure

```
├── esp32_mist_control/
│   ├── mist_control.ino
│   └── connections.png
│
├── raspberry_pi_yolo/
│   ├── detect7.py
│   ├── best.pt
│   └── pollen_log.txt
│
├── tutorial_mist_humidifier.docx
├── Tutorial_PI_model.docx
└── README.md
```

---

## 🔧 Part 1: ESP32 Mist / Humidifier Control

### 🧩 Components Required

* ESP32 Dev Board
* Ultrasonic Sensor (HC-SR04)
* Mist / Humidifier Module (digital control)
* 5V Power Supply
* Voltage Divider for ECHO pin

  * 1 kΩ resistor
  * 2 kΩ resistor
* Jumper wires

---

### 🔌 Pin Connections

#### HC-SR04 Ultrasonic Sensor

| HC-SR04 Pin | ESP32 Pin |
| ----------- | --------- |
| VCC         | 5V        |
| GND         | GND       |
| TRIG        | GPIO 27   |
| ECHO        | GPIO 26   |

#### Humidifier Module

| Humidifier Pin | ESP32 Pin |
| -------------- | --------- |
| SIGNAL         | GPIO 25   |
| VCC            | 5V        |
| GND            | GND       |

⚠️ **Important:** Use a voltage divider on the ECHO pin to protect the ESP32 (5V → 3.3V).

---

### ⚙️ Working Principle

* Ultrasonic sensor continuously measures distance
* If an object (bee) is detected **within 20 cm**
* ESP32 triggers a **non-blocking humidifier pulse sequence**
* Mist timing is precisely controlled using `millis()` (no delay blocking)

---

### ⏱️ Mist Timing Control

Default mist ON time:

```cpp
900 ms (0.9 seconds)
```

To change mist duration:

| Desired Mist Time | Value to Set |
| ----------------- | ------------ |
| 1 second          | 900          |
| 2 seconds         | 1900         |
| 3 seconds         | 2900         |
| 4 seconds         | 3900         |

> Only update the **WAIT time**, closing pulses remain 0.1 sec.

---

## 🧠 Part 2: Raspberry Pi + YOLOv5 Bee Detection

### 💾 Requirements

* Raspberry Pi 4
* 32GB SD Card
* Camera Module
* Monitor, Keyboard, Mouse

---

### 📥 Flash Raspberry Pi Image

1. Download image:

   ```
   polliguide_flower.img.zip
   ```

2. Extract to get:

   ```
   polliguide_flower.img
   ```

3. Install **Raspberry Pi Imager**

4. Select:

   * **Choose OS → Use Custom**
   * Select `polliguide_flower.img`

5. Choose SD Card → **WRITE**

6. Safely eject and boot Raspberry Pi

---

### 📷 Camera Test

```bash
rpicam-still -o test.jpg
```

Image will be saved in:

```
/home/ebmacs/
```

---

### 🤖 Run YOLOv5 Detection

Activate environment:

```bash
source ~/yolo_env/bin/activate
```

Navigate to YOLO folder:

```bash
cd ~/yolov5
```

Run detection:

```bash
python detect7.py --weights /home/ebmacs/model/best.pt --source 0
```

---

### 📂 Output Files

* `pollen_log.txt` → Detection logs
* `pollen_images/` → Images of detected pollen bees

Press **CTRL + C** to stop detection.

---

## ✅ Features

* Real-time bee detection using YOLOv5
* Automatic mist control based on proximity
* Non-blocking ESP32 control logic
* Custom-trained detection model
* Suitable for smart farming & pollination research

---

## 📌 Future Improvements

* Cloud data logging
* Mobile dashboard
* Battery-powered ESP32 version
* Integration with ESP32 via WiFi/BLE

---

## 👤 Author
**Muhammad Hassan**

