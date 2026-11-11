# BlindDistanceDetector

**Distance Detector for the Blind**

## Suggested Components:

- ESP32-C3 Super Mini
- HC-SR04 Ultrasonic Sensor
- SG90 Micro Servo
- 5 V Step-Down Voltage Regulator (Mini560, MP1584, or similar)
- 2 × 18650 Batteries
- 2S Battery Holder for 18650 Batteries
- Charger/Protection Module for 2S 18650 Battery Configuration
- 2 Resistors for a voltage divider on the HC-SR04 ECHO signal (to adapt the voltage to the ESP32's 3.3 V logic level)
- 1 Push Button
- 3D-Printed Enclosure
- Rotating Cap attached to the servo shaft, featuring a tactile indicator

* Version 2: VL53L0X

## How It Works

When the button is pressed, the device measures the distance five times and uses the shortest measured distance to calculate the corresponding servo position as follows:

### Version 1:
- **For distances below 50 cm:** The servo moves from 180° (0 cm) to 90° (50 cm).
- **For distances above 50 cm:** The servo moves from 90° (50 cm) to 0° (200 cm or more).
### Version 2:
- **For distances below 50 cm:** The servo moves from 180° (0 cm) to 140° (50 cm).
-  **For distances between 50 cm to 150cm:** The servo moves from 140° (50 cm) to 30° (150 cm).
- **For distances above 150 cm:** The servo moves from 30° (150 cm) to 0° (300 cm or more).

### YouTube

- Version 1: https://youtu.be/0mqLhk4pwrI
- Version 2: https://www.youtube.com/watch?v=io_nqs0bMak

### Tinkercad

https://www.tinkercad.com/things/elehajj8ofC-detector-de-distancias-para-ciegos

