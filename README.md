# Radar vs Camera Fall Detection

## Description
This repository hosts all firmware and scripts for our senior design project comparing fall detection between:
- **Camera**: OpenMV Cam H7 Plus  
- **Radar**: DFRobot C1001 human‑detection module  

Non‑camera code runs on a Swan/Cygnet STM32L4 MCU; other boards are untested.

---


## Hardware & Dependencies
- **MCU**: Swan/Cygnet (STM32L4 based)  
- **Camera**: OpenMV Cam H7 Plus  
- **Radar**: DFRobot C1001 Human Detection Module  
- **Firmware Libraries**:
  - [STM32LowPower](https://github.com/stm32duino/STM32LowPower)  
  - [DFRobot_HumanDetection](http://github.com/DFRobot/DFRobot_HumanDetection)  
- **Tools**:
  - OpenMV IDE  
  - PlatformIO CLI
  - Arduino IDE
 
  
---

## Setup & Usage

### Camera Module (through USB)
1. Connect the OpenMV Cam H7 Plus via USB.  
2. Copy `trained.tflite` and `labels.txt` to the device’s root.  
3. Upload `ei_object_detect.py` in the OpenMV IDE.  
4. Run the code and monitor the device screen for output. 


### Camera Module (without USB and using external power)
1. Power the OpenMV Cam H7 Plus via an external power source.   
2. Copy `trained.tflite`, `labels.txt`, and `ei_object_detect.py` to the device’s root.  
3. Power camera and monitor LED for fall status.  


### Radar Module (through USB connected to MCU and power through 3V3 pin)
1. Set up the radar module in the designated orientation.
2. Flash `C1001_main.ino` onto the MCU.
3. Power up the radar module and connect it to the MCU through the UART pins
4. Test the device and monitor the serial monitor on Arduino IDE for output. 


### Radar Module (without USB connected to MCU and powered through 3V3 pin)
1. Set up the radar module in the designated orientation.
2. Flash `C1001_main.ino` onto the MCU.
3. Power up the radar module and connect it to the MCU through the UART pins
4. Test the device and monitor the GPIO pin on the MCU or the fall status LED indicator on C1001.

---

### Notes
- lowPower.deepSleep() has been observed to be unstable; try other modes provided by the STM32LowPower library (e.g. sleep(), standby(), etc.) if you encounter issues.
- The radar code can be merged into the main.cpp with some minor tweaks and can be powered using a load switch, but we’ve kept it as a separate file for straightforward, device‑specific testing.
- You can adjust pin mappings and timing parameters as needed for your hardware setup.
- For further features, we suggest looking into some additional library dependencies found at https://github.com/stm32duino
