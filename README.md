# EcoAware

A smart home hackathon project designed for dynamic home balance — improving eco-awareness, automation, and comfort.

## Project Overview

EcoAware lets users monitor and balance home conditions by integrating sensors and intelligent automation. Born from a hackathon, this project uses Arduino-compatible microcontrollers and IoT principles to keep environments comfortable and energy-efficient.

## Features

- Sensor Integration: Supports temperature, humidity, light, and other smart sensors.
- Automation: Triggers alerts or actuators (like fans, lights, buzzers) for balance.
- Expandable: Add new sensors, outputs, or automations as needed.

## Installation

1. Clone this repo or download the ZIP:
    ```bash
    git clone https://github.com/rockdude129/ecoaware.git
    ```
2. Open `ecoaware_oct19a.ino` in the Arduino IDE.
3. Copy `arduino_secrets.h` and configure your Wi-Fi and keys as needed.
4. Ensure all required libraries are installed (see `thingProperties.h` for clues).

## Circuit Assembly

- Assemble the circuit using the included schematic (`schematics.png`) and layout (`layout.png`) if available.
- The system typically uses Arduino-compatible boards, DHT11 sensors, buzzers, and LEDs (see BOM below).

## Usage

- Upload the sketch to your microcontroller.
- The device will begin monitoring and balancing.
- Review serial output for real-time logs, or connect to your IoT dashboard if available.

## Folder Structure

```
ecoaware/
│── ecoaware_oct19a.ino     # Main Arduino code
│── arduino_secrets.h       # User secrets/config
│── thingProperties.h       # Arduino IoT Cloud props
│── sketch.json             # Arduino Web Editor project file
│── [schematics.png]        # Schematic diagram (if present)
│── [layout.png]            # Circuit layout (if present)
│── README.md               # You're here!
│── ReadMe.adoc             # AsciiDoc version of README
```

## Bill of Materials (BOM)

| ID  | Part Name        | Part Number  | Quantity |
|-----|------------------|--------------|----------|
| R1  | 10k Resistor     | 1234-abcd    | 10       |
| L1  | Red LED          | 2345-asdf    | 5        |
| A1  | Arduino Zero     | ABX00066     | 1        |
| S1  | DHT11 Sensor     | [generic]    | 1        |
| BZ1 | Passive Buzzer   | HW-508       | 1        |

## License

This project is released under the Public Domain License.

## Contributing

Open an issue or pull request. For questions, contact the project maintainer.

---

_Smart home balance for a greener tomorrow!_
