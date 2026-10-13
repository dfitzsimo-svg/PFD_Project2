# PFD_Project2

In this proam trying to make a 737 style standby PFD that can show real time speed, altitude, and attitude.
I am using an ESP32 along with a GPS module, barometer and IMU to measure the kinematics. A list of all my components is giving in the project files along with some notes on each. I started the project by first writing Python code in Virtual Studio to simulate the look of the real life 737 max standby horizon as much as possible. I started out using Python and a virtual screen at first as I was much more familiar with that language and being able to use a virtual screen meant I could quickly make edits, see how it looked and then move on without having to flash the code to a physical device each time.

After I was happy with how everything looked, I translated my code over to the Arduino IDE in order to flash it to my ESP. This is where I encountered a few problems and had to make large adjustments to my code. The biggest problem was the low amount of memory of the ESP. At only -500 Kb, I had a small budget of memory and had to try many optimisation techniques. The most beneficial of these was to create sprites for certain elements of the graphics and to seperate the screen into chunks depending on their required refresh rates.

Once I had sorted the graphical issues, I then moved on to incorporating all my sensors together onto a breadboard. This turned out to be very straightforward as I had done plenty of testing with my Python code with fake sensor data. The only issue I had here was from poor connection quality from the breadboard. The next addition was to power the device with a battery as it would be in my final version. This is something that turned out harder than I expected but forced me to learn a lot about battery tech, power regulation and general circuit design. I ended up using a simple 1S LiPo with a TP4056 for charge protection and an MT3608 for voltage regulation to maintain 5V supply to the ESP.

After I was happy with the breadboard prototype, I began by designing my PCB using KiCAD which I was new to but found it to be extremely intuitive and easy to learn. At this stage I have fully completed my schematic and am now trying to settle on a board layout for all the modules. 

## Hardware used

- Elegoo ESP32
- ILI9488 3.5" SPI screen, 480x320
- NEO-6M GPS - speed
- BMP280 - altitude (want to add a rotary encoder for QNH later)
- MPU6050 - pitch/roll

## Sidenotes
I originally began by using a Raspberry Pi that I had laying around but the pi lacked the RAM to push detailed graphics to the screen via SPI and also lacked a ready to use driver. This is a known limitation of the chip itself, not something specific to the Pi. I had to write my own driver since there was no existing Linux library that handled this properly. Even after that, the performance was poor and the frame rate was slow.
## ESP32 version notes

- Same layout/logic as the Python version, ported to C++
- Drawing straight to the screen caused bad flicker
- Tried a full-screen buffer to fix it but ran out of contiguous RAM
- Fixed it by drawing in 4 horizontal bands instead, each with its own small buffer
