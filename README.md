# 🔧 Predictive Maintenance of Machines using IoT

<p align="center">

<img src="https://img.shields.io/badge/ESP32-IoT-blue?style=for-the-badge&logo=espressif"/>

<img src="https://img.shields.io/badge/ThingSpeak-Cloud-green?style=for-the-badge"/>

<img src="https://img.shields.io/badge/Arduino-IDE-teal?style=for-the-badge&logo=arduino"/>

<img src="https://img.shields.io/badge/IoT-Embedded%20Systems-orange?style=for-the-badge"/>

</p>

---

## 📌 Project Overview

This project presents an **IoT-based Predictive Maintenance System** designed to monitor industrial machines in real time using vibration and temperature sensors.

The system continuously collects machine condition data and uploads it to the **ThingSpeak cloud platform** using ESP32 Wi-Fi communication. By analyzing temperature and vibration levels, the system helps identify abnormal machine behavior before failures occur.

This improves:
- Industrial safety
- Machine reliability
- Predictive maintenance capability
- Operational efficiency

---

## 🚀 Features

✅ Real-time machine monitoring  
✅ Temperature sensing using DHT11  
✅ Vibration analysis using SW-420  
✅ ESP32 Wi-Fi cloud communication  
✅ ThingSpeak IoT dashboard integration  
✅ Early fault detection system  
✅ Live graphical monitoring  

---

## 🛠️ Hardware Components

| Component | Description |
|---|---|
| ESP32 Type-C | Main IoT controller |
| DHT11 Sensor | Temperature monitoring |
| SW-420 Sensor | Vibration detection |
| Breadboard | Circuit prototyping |
| Jumper Wires | Hardware connections |

---

## ⚙️ Software & Technologies Used

- Arduino IDE
- Embedded C++
- ESP32 Wi-Fi
- ThingSpeak Cloud Platform
- MATLAB Analytics
- IoT Monitoring System

---

## 🔌 Circuit Diagram

<p align="center">
<img src="images/circuit_diagram.png" width="700"/>
</p>

---

## 📊 ThingSpeak Monitoring Output

<p align="center">
<img src="images/thingspeak_output.png" width="700"/>
</p>

---

## 📈 MATLAB Analysis Output

<p align="center">
<img src="images/matlab_analysis.png" width="700"/>
</p>

---

## 🖥️ Serial Monitor Output

<p align="center">
<img src="images/serial_monitor.png" width="700"/>
</p>

---

## ⚡ Working Principle

1. The DHT11 sensor continuously monitors machine temperature.
2. The SW-420 sensor detects abnormal machine vibrations.
3. ESP32 processes sensor data in real time.
4. Sensor readings are uploaded to ThingSpeak through Wi-Fi.
5. The cloud dashboard visualizes machine condition data.
6. Abnormal patterns help predict possible machine failures.

---

## 🔮 Future Improvements

- AI-based predictive analytics
- Mobile application integration
- Raspberry Pi edge AI deployment
- Industrial automation integration
- Multi-machine monitoring system
- Advanced vibration spectrum analysis

---

## 📂 Repository Structure

```bash
Predictive-Maintenance-Using-IoT/
│
├── README.md
├── predictive_maintenance.ino
│
├── images/
│   ├── circuit_diagram.png
│   ├── thingspeak_output.png
│   ├── matlab_analysis.png
│   └── serial_monitor.png
