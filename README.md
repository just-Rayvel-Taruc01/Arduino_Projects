# AutoCompost: Automated Composting Machine

AutoCompost is an Arduino-based automated composting system designed to transform vegetable waste into organic fertilizer.  
The system integrates sensors, actuators, and controllers to automate shredding, mixing, and moisture balancing of compost materials.

---

## 📂 Project Structure
```bash
Arduino_Projects
|  AutoCompost/
|  │── AutoCompost.ino    # Main Arduino sketch (system logic)
|  │── ServoControl.cpp   # Servo control implementation (Adafruit Servo Shield)
|  │── ServoControl.h     # Servo control header file
|──README.md              # Project documentation
```
---

## 🚀 Features
- **Automated composting process** – from shredding to mixing.  
- **Moisture monitoring** using a capacitive soil moisture sensor.  
- **Precise CRH (Carbonized Rice Hull) dropping** with a load cell + HX711.  
- **Servo-controlled mechanisms** for material loading/unloading and output doors.  
- **16x2 I2C LCD** for process monitoring.  
- **One-button operation** (start/stop).  
- **Relay & contactor control** for AC motors driving the shredder and mixer.  

---

## 🛠️ Hardware Components
- Arduino Mega & Arduino Uno  
- 16x2 I2C LCD Display  
- Capacitive Soil Moisture Sensor  
- Load Cell (5kg) with HX711 Module  
- MG996R Servo Motors (via Adafruit Servo Shield)  
- 2-Channel Relay Module & Magnetic Contactor  
- Arcade Button (Momentary Switch)  
- AC Motors for Shredder and Mixer  

---

## ⚡ How It Works
1. **Start** the process with the push button.  
2. Waste is **shredded** and loaded into the mixing drum.  
3. **CRH dropper** adds measured amounts of carbonized rice hull.  
4. **Moisture sensor** checks compost moisture. If above threshold, more CRH is added.  
5. The **mixer motor** ensures even blending until the ideal moisture (50–60%) is reached.  
6. The **output door servo** opens to release the finished compost.  

---

## 🔧 Setup & Usage
1. Open `AutoCompost.ino` in the **Arduino IDE**.  
2. Install required libraries:  
   - `Wire.h` (I2C communication)  
   - `LiquidCrystal_I2C.h` (LCD display)  
   - `HX711.h` (load cell)  
   - `Adafruit_PWMServoDriver.h` (servo shield)  
3. Connect hardware components as per wiring diagram (to be added).  
4. Upload code to your Arduino Mega/Uno.  
5. Press the **arcade button** to start the composting cycle.  

---

## 📜 License
This project is open-source under the MIT License.  

---
