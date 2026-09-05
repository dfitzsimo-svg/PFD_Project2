# PFD_Project2

Car PFD (Primary Flight Display) project - I am trying to make an aircraft-style instrument panel for a car, showing speed, altitude, and attitude (pitch/roll).
I began by using a Raspberry Pi that I had laying around but the ILI9488 screen does not support 16-bit colour (RGB565) over SPI. This is a known limitation of the chip itself, not something specific to the Pi. I had to run the screen in 18-bit colour and write my own driver since there was no existing Linux library that handled this properly. Even after that, the performance was poor and the frame rate was slow.

I then switched to an ESP32 which has libraries like TFT_eSPI that handle everything on the driver side although I still have had to make a lot of tuning to my code to get the screen to be smooth.

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

