# FlexibleTaikoController
A remake of LuiCats' ArduinoTaikoController that allows for way easier code modification. Originally this was just a fork, but so many things ended up changing that I deleted the fork and published it in its own repo.

# List of changes:

## Pros:
- Reduced line count
- Increased readability with explanations for almost every variable
- Easily changable output, E.G adding XInput, Joystick, etc... requires very little modification
- Added sensitivity knobs feature

## Cons:
- Axed Switch support (for now)
- Axed analogReadNow

This will work with basically every microcontroller that has analog pins and support for HID output (provided you set up said micro controllers HID library). For instance the Teensy 4.0 has a joystick and keyboard library but they dont use the same syntax as the Arduino Pro Micro and ESP32s2 / ESP32s3s' joystick and keyboard Libraries. The current ino file is setup for an ESP32S3, by adding a // infront of the #define ESP32 line at the top, it will work with an Arduino Micro.

# How to add Syntax for other boards???
(To be added at a later date)



