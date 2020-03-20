# FPGBApp - C++ GBA Emulator 

Reference plattform for my FPGA GBA implementation. 
Conversion from C# to C++, about double speed.

### Attention
This emulator is not made for good playing experience, but as a research project!
While it can play almost every game, it has quiet some shortcomings.

- Some features are only implemented to the point they are understood and can be implemented in FPGA logic
- while it's using SDL and should be plattform independent, only a Visual Studio project for Windows exists currently

# Features (some not yet available via UI or Keys!)
- all videomodes including affine and special effects
- all soundchannels
- saving as in GBA
- Savestates(compatible with FPGA implementation)
- FastForward - Speed depends on your system
- adjustable CPU Under/Overclocking
- Flickerblend - turn on for games like F-Zero, Mario Kart to prevent flickering effects
- Affine Layer (Mode 7) double Resolution  
- Affine Layer (Mode 7) 2x2 Antialiasing

# Features missing
- Fixed Keyboard/Gamepad Keys(see below)
- Asynchronous screen update(tearing!)
- No support of CPU Pipeline prefetch(Classic NES games don't work)
- Gamepak Features like RTC, Solar Sensor, Gyro, Tilt

# Input
Keyboard:
- OSD Menu - Escape
- DPad - Cursor Keys
- A - A (D for Turbo fire)
- B - S (F for Turbo fire)
- L - Q
- R - W
- Start - X
- Select - Y
- Quicksave - F5
- Saveram to Disk - F6
- Quicksave to Disk - F7
- Quickload - F9
- Quickload from Disk - F10
- FastForward - Space = Hold, 0 = on

Gamepad: fixed buttons

# Bios
No Bios included, please add Bios as gba_bios.bin

# Accuracy

(Status 03.02.2020)

>> Attention: the following comparisons are NOT intended for proving any solution is better than the other.
>> This is solely here for the purpose of showing the status compared to other great emulators available.
>> It is not unusual that an emulator can play games fine and still fail tests. 
>> Furthermore some of these tests are new and not yet addressed by most emulators.

There is great testsuite you can get from here: https://github.com/mgba-emu/suite
It tests out correct Memory, Timer, DMA, CPU, BIOS behavior and also instruction timing. It works 100% on the real GBA.
The suite itself has several thousand single tests.

Testname      | TestCount | FPGBApp    | FPGBA FPGA | mGBA | VBA-M | Higan
--------------|-----------|------------|----------- |------|-------|-------
Memory        |      1552 |  1552      |   1552     | 1552 |  1338 | 1552
IOREAD        |       123 |   123      |      123   |  116 |   100 |  123
Timing        |      1660 |  1554      |     1554   | 1540 |   692 | 1424
Timer         |       936 |   339      |      445   |  610 |   440 |  457
Timer IRQ     |        90 |    65      |       65   |   70 |     8 |   36
Shifter       |       140 |   140      |      140   |  140 |   132 |  132
Carry         |        93 |    93      |       93   |   93 |    93 |   93
BIOSMath      |       625 |   625      |      625   |  625 |   625 |  625
DMATests      |      1256 |  1248      |     1248   | 1232 |  1032 | 1136
EdgeCase      |        10 |     1      |        3   |    7 |     3 |    1
Layer Toggle  |         1 |  pass      |     pass   | pass |  pass | fail 
OAM Update    |         1 |  fail      |     fail   | fail |  fail | fail


A complex CPU only testuite can be found here: https://github.com/jsmolka/gba-suite

Testname | FPGBApp    | FPGBA FPGA| mGBA | VBA-M | Higan
---------|------------|-----------|------|-------|-------
ARM      |  Pass      |  Pass     | Fail |  Fail |  Fail
THUMB    |  Pass      |  Pass     | Fail |  Fail |  Fail


# Bugs

Bugs can be commited, but have very low priority if they don't occur in the FPGA version.
Please don't report feature requests for UI or Input handling, as this is no goal!
