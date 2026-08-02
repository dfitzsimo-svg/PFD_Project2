# PFD_Project2

Car PFD (Primary Flight Display) project - basically an aircraft-style instrument panel for a car, showing speed, altitude, and attitude (pitch/roll).

## Two versions in here

**Root folder** - original Python/pygame version, built for a Raspberry Pi Zero 2W with an ILI9488 screen.

**esp32_version/** - moved over to an ESP32 (Elegoo board) since it has way better library support for this screen than the Pi does. This is the active version now.

## Pi version notes

- ILI9488 needs 18-bit colour mode, not 16-bit RGB565 - RGB565 just doesn't work right over SPI on this chip
- Fonts need to be created once, not inside the draw loop, or it lags badly
- ili9488.py is the driver, exposes init_display / fill_screen / set_window / push_to_display

## ESP32 version notes

- Same layout/logic as the Python version, ported to C++ using TFT_eSPI
- Drawing straight to the screen caused bad flicker
- Tried a full-screen buffer to fix it but ran out of contiguous RAM
- Fixed it by drawing in 4 horizontal bands instead, each with its own small buffer
- Still ironing out a couple of bugs from the band switch (triangles duplicating across bands)

## Hardware (ESP32 version)

- Elegoo ESP32
- ILI9488 3.5" SPI screen, 480x320
- NEO-6M GPS - speed
- BMP280 - altitude (want to add a rotary encoder for QNH later)
- MPU6050 - pitch/roll

## To do

- fix triangle duplication bug across bands
- wire up GPS
- wire up BMP280 + QNH encoder
- wire up MPU6050
- swap fake ramp values for real sensor data
- figure out power/mounting for the car
