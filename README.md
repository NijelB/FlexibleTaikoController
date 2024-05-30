# FlexibleTaikoController
A remake of LuiCats' ArduinoTaikoController that allows for way easier code modification. Originally this was just a fork, but so many things ended up changing that I deleted the fork and published it in its own repo.

# List of changes:

## Pros:
- Reduced line count
- Increased readability with explanations for almost every variable
- Easily changable output, E.G adding XInput, Joystick, etc... requires very little modification
- Added sensitivity knobs feature

## Nuetral:
- Switched to a "fixed" threshold based system
- Changes in the logic that determines a hit (no longer reads loudest sensor)

## Cons:
- Axed Switch support (for now)
- Axed analogReadNow

This will work with basically every microcontroller that has analog pins and support for HID output (provided you set up said micro controllers HID library). For instance the Teensy 4.0 has a joystick and keyboard library but they dont use the same syntax as the Arduino Pro Micro and ESP32s2 / ESP32s3s' joystick and keyboard Libraries. The current ino file is setup for an ESP32S3, by adding a // infront of the #define ESP32 line at the top, it will work with an Arduino Micro.

# Setup 
Almost identical to LuiCats original code so go read that here, the main difference is these adjustable variables below: https://github.com/LuiCat/ArduinoTaikoController/tree/master

# Adjustable Variables
```
loopTime = 300;
```
 - How often to refresh the loop in micro seconds (if the micro controller isn't fast enough to preform this it will go to zero).
```
defaultThreshold[] = {20, 20, 20, 20}
```
 - Place your threshold variables here.
```
cd_length = 20;
```
- How much longer you need to wait in ms before hitting the same Don or Ka again.
```
k_threshold = 4; 
```
- If you are detecting a hit while the Don or Ka is already pressed, the threshold gets multiplied by this number. The piezo sensors output will build in intensity even after it triggers an input so the threshold value is raised after an input to prevent unintentional rolls. If you are getting unintentional rolls increase this number, if you are missing intentional rolls decrease it slightly.
```
k_decay = 0.98; 
```
- This is how fast the threshold decays after being increased, a higher number means it will decay slower, lower means it will decay faster. If you are missing intentional drum rolls, decrease by 0.01 until the problem is solved. (MUST ALWAYS BE LESS THAN 1).
```
float sens[4] = {1.0, 1.0, 1.0, 1.0}; 
```
- A multiplier for the output of a sensor, if a sensor is too strong and triggers while hitting a different note lower its sensitivity.


# How to add Syntax for other boards???
(To be added at a later date)



