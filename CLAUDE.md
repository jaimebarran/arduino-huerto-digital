# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project state

"Huerto Digital" (digital vegetable garden) is an Arduino project that monitors plant conditions (temperature, humidity, light) and shows them on an LCD, LEDs and a buzzer. Code comments and serial/LCD strings are in Spanish.

The sketches live at the repo root, one folder per sketch (`<Name>/<Name>.ino`, which the Arduino IDE requires):

- `Huerto_Digital_Temperatura`: the main sketch. It uses a DHT11 for temperature and humidity, a photoresistor and a 16x2 LCD, with LEDs for temperature alerts and an RGB LED for darkness.
- `Prueba_Buzzer`, `Prueba_Buzzer_2`: earlier test sketches ("prueba" means test). They use an analog TMP36 temperature sensor plus a buzzer. There's no buzzer on the current board; the code is kept in case one is added.

A copy of the sketches also still exists in `G:\My Drive\Projects\Arduino\Huerto Digital\`; this repo is the source of truth now. The old vendored DHT v1.3.0 in `G:\My Drive\Projects\Arduino\DHT_sensor_library\` is not part of the repo. Sketches build against the Library Manager version instead.

## Build / upload

There are no tests or linter. `arduino-cli` (installed at `C:\Program Files\Arduino CLI\arduino-cli.exe`) shares its libraries with the Arduino IDE in `~/Documents/Arduino/libraries`. The installed libraries are `DHT sensor library` 1.4.7, `Adafruit Unified Sensor` and `LiquidCrystal`, plus the `arduino:avr` core.

```sh
arduino-cli compile --fqbn arduino:avr:uno Huerto_Digital_Temperatura
arduino-cli upload  --fqbn arduino:avr:uno -p COM5 "<sketch folder>"
arduino-cli board list
```

The board is an Uno-compatible board from a SparkFun kit. It has an FTDI USB-serial chip (VID 0403/PID 6001), so `board list` shows it as "Unknown" on COM5. Use `arduino:avr:uno` for it. Running `arduino-cli monitor` non-interactively exits immediately. To read serial output from a script, open COM5 at 9600 baud with .NET `System.IO.Ports.SerialPort` in PowerShell.

## Hardware wiring (shared across sketches)

All sketches assume the same physical rig, so pin numbers are hardware facts. Don't change them casually:

- LCD (`LiquidCrystal lcd(7,6,5,4,3,2)`): RS=7, EN=6, D4–D7=5,4,3,2; 16x2.
- Main sketch: DHT11 data on pin 8, photoresistor on A0 (dark threshold 500), potentiometer on A1 (declared but unused), red LED on 13, yellow LED on 12, RGB LED on PWM pins 9/10/11.
- Test sketches: TMP36 on A0, light sensor on A1, buzzer on 9, LEDs on 10. These overlap with the main sketch's RGB pins, which reflects an older wiring.
- Serial runs at 9600 baud.

## Main sketch behavior

`loop()` runs every 2 s. It reads the DHT as `float` and, if a reading fails (NaN), shows an error on the LCD and retries. Otherwise it prints to serial and rewrites the whole of LCD row 1 as one fixed-width 16-char line (`"23C    45%  78% "`), so leftover digits never remain. The labels on row 0 are drawn once in `setup()`, and their columns (0, 6, 11) must stay aligned with that line. Temperature is shown in whole degrees because this DHT11 only measures whole degrees. AVR `snprintf` has no `%f`, so floats need `lround` or `dtostrf`. Yellow LED ≥25 °C, red LED ≥28 °C, and the RGB LED turns cyan when the raw light reading is below `threshold`. Luminosity % is `map(analogRead, 0, 1023, 0, 100)`.

The sketches build against the Library Manager DHT version (1.4.7). The DHT11 on this rig only reports whole degrees, so decimals would need a DHT22 (`#define DHTTYPE DHT22`).
