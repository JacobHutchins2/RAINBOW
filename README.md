# RAINBOW
**Regulating Automatic Irrigation with a Nature-Based Optimized Watering System**

RAINBOW is a bare-metal Raspberry Pi (Pi 1B+) system that controls an automated irrigation pump using soil moisture sensing, real-time scheduling, and user-selectable presets. The system features a button-driven LCD interface, optional serial debug shell, and precise pump control via PWM.

---

## Features

- Bare-metal kernel (no Linux, no runtime OS)
- Soil moisture sensing over I²C
- PWM-controlled irrigation pump
- Real-time clock and timer interrupts
- Automatic watering presets
- Button-driven LCD menu interface
- TFT display support (SPI)
- UART debug shell
- Heartbeat LED system indicator

---

## Hardware

- Raspberry Pi 1B+
- I²C soil moisture sensor
- Water pump (PWM controlled)
- 16×2 LCD with I²C backpack
- TFT display (SPI)
- Push buttons (GPIO)
- Status LED (GPIO)

---

## System Startup Sequence

1. UART initialization  
2. I²C initialization  
3. LCD initialization and status messages  
4. Heartbeat LED start  
5. Timer initialization  
6. Interrupts enabled  
7. PWM initialization (pump OFF by default)  
8. TFT initialization  
9. SPI initialization  
10. Enter RAINBOW shell (menu UI)

---

## Menu Interface

Navigation is handled entirely through GPIO push buttons and a 16×2 LCD.

### Menu Options

1. **Read Moisture**  
   Displays live soil moisture readings.

2. **Cycle Pump**  
   Manually runs the pump for a fixed duration.

3. **Presets**  
   Select automatic watering behavior:
   - **Preset 1:** Water only when moisture is below threshold  
   - **Preset 2:** Water once per day at a scheduled time  

4. **Set Time**  
   Sets the system clock used for scheduled watering.

5. **TFT Sleep**  
   Power-cycles the TFT display.

6. **Debug Mode**  
   Enables a UART-based command shell for testing and diagnostics.

---

## Automatic Watering Presets

Presets are checked continuously while the system is running.

### Preset 1
- Reads soil moisture
- Waters only if soil is dry

### Preset 2
- Waters at a scheduled time
- Runs once per day

Preset execution is driven by timer interrupts and real-time clock data.

---

## Debug Mode

Debug mode provides:
- UART interactive shell
- Manual PWM testing
- Button-triggered diagnostics
- Direct command parsing (no stdio)

This mode is intended for development and validation only.

---

## Notes

- Designed for embedded systems and bare-metal development
- All peripherals use custom drivers
- No operating system, filesystem, or dynamic memory allocation
- Timing handled via hardware timers and interrupts
