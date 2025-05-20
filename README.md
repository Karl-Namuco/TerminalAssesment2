  Smart Home - Mini Assistant

A web-based smart home assistant that enables remote control of household lights and monitors gas and air quality using IoT devices and Firebase.

Project Overview

This project integrates **IoT** and **Cloud Computing** to automate basic home functions and monitor environmental safety parameters in real-time.

Features

- Toggle lights in multiple rooms (Outdoor, Living Room, Bedroom 1 & 2)
- View real-time readings from:
  - MQ2 Sensor (flammable gas detection)
  - MQ135 Sensor (air quality monitoring)
  - status updates for:
  - Gas Danger Level
  - Air Cleanliness
- Responsive UI with switch controls
- Serverless backend using Firebase Realtime Database


 How It Works

System Architecture

1. ESP32 collects data from MQ2 and MQ135 sensors.
2. Sensor data and light states are sent to **Firebase Realtime Database**.
3. A **web interface** (HTML/CSS/JS) reads and writes data to Firebase.
4. Users can toggle switches from the browser to turn on/off lights.
5. The system updates UI in real-time based on sensor status.


