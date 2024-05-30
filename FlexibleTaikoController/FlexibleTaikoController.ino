#define ESP32
#define ENABLE_DEBUG
#define ENABLE_KEYBOARD
//#define ENABLE_JS
#define ADJ_POTS
//#define ADJ_COLOR
#define ENABLE_LEDS

//pin definitions, place in the order of left ka, left don, right don, right ka
const int sensorPins[] {A7, A5, A4, A6};
const int potPins[] {A3, A2, A0, A1}; //
const int ledPins[] {D5, D4, D2, D3}; //3 4 5 2

//logic variables
unsigned long loopTime = 300; //How often to loop in micro seconds
int defaultThreshold[] = {200, 200, 200, 200};
int threshold[] = {200, 200, 200, 200}; //The threshold
const float cd_length = 20; //How long a sensor is on cooldown after being hit in milliseconds
const float k_threshold = 4; //How much the threshold gets bumped when an input happens
const float k_decay = 0.98; //How much to decay the threshold by each tick
float sens[4] = {1.0, 1.0, 10.0, 1.0}; //Sensor sensitivity modifiers
int raw[4] = {0, 0, 0, 0}; 
float level[4] = {0, 0, 0, 0};
long cd[4] = {0, 0, 0, 0}; //Sensor cooldown
bool down[4] = {false, false, false, false};
int so[] = {1, 2, 3, 0}; //Sensor order
int si = 0; //Sensor index
unsigned long lastDebug = 0;

//libraries and variables needed for certain settings
#ifdef ESP32
  #include <driver/adc.h>
#endif
#ifdef ENABLE_KEYBOARD
  #include "USB.h"
  #include "USBHIDKeyboard.h"
  USBHIDKeyboard Keyboard;
  //output definitions
  const char key[] {'s', 'c', 'm', 'l'};
#endif
#ifdef ADJ_COLOR
  const int donColorPins[] {0, 0, 0}; //Place in the order RGB
  const int kaColorPins[] {0, 0, 0};
#endif

//sample a single sensor
void sampleSingle(int i) {
  int prev = raw[i];
  raw[i] = analogRead(sensorPins[i]);
  level[i] = abs(raw[i] -prev) * sens[i];
}

void setup() {
  #ifdef ESP32
    analogReadResolution(10);
  #endif
  #ifdef ENABLE_LEDS
    for (int i = 0; i < 4; ++i) { pinMode(ledPins[i], OUTPUT); }
  #endif
  #ifdef ENABLE_KEYBOARD
    Keyboard.begin();
    USB.begin();
  #endif
  Serial.begin(115200);
}

void loop() {
  control();
}

//Main, Evaluate sensors and determine their state
void control(){
  //ORDER OF OPERATIONS:
  //if button is on:
  //  if cooldown is over:
  //    button off
  //    light off
  //  else
  //    cooldown --
  //else:
  // if threshold is not below the default:
  //   reduce threshold *= k_decay
  // else:
  //   threshold = defaultThreshold
  //
  ////Read current sensor <-- big change were not reading the loudest sensor anymore
  //if level > threshold:
  //  if button is not on:
  //    button on
  //    light on
  //    cooldown[si] = cooldown
  //  if button is on:
  //    increase threshold *= k_threshold
  //
  //  calc loop time such that delay is constant to loopTime
  unsigned long st = micros(); //start time

  si = so[si]; //iterate to next sensor

  #ifdef ADJ_POTS
    defaultThreshold[si] = analogRead(potPins[si]) + 11;
  #endif

  if(down[si]){
    if(cd[si] <= 0){
      sendControl(1, si);
      down[si] = false;
    }
    else{
      cd[si] -= loopTime / 1000.0;
    }
  }
  else{
    if(threshold[si] >= defaultThreshold[si]){
        threshold[si] *= k_decay;
      }
    else{
        threshold[si] = defaultThreshold[si];
    }
  }

  sampleSingle(si);

  if(level[si] > threshold[si]){
    if(!down[si]){
      sendControl(0, si);
      down[si] = true;
      cd[si] = cd_length;
    }
    else{
      threshold[si] *= k_threshold;
    }
  }

  #ifdef ENABLE_DEBUG
    if(lastDebug < millis()){
      lastDebug = millis() + 5;
      Serial.println(
        "0 " + String(threshold[0]) + " Down:" + String(down[0]) + " Cd:" + String(cd[0])+
        " 1 " + String(threshold[1]) + " Down:" + String(down[1])+ " Cd:" + String(cd[1])+
        " 2 " + String(threshold[2]) + " Down:" + String(down[2])+ " Cd:" + String(cd[2])+
        " 3 " + String(threshold[3]) + " Down:" + String(down[3])+ " Cd:" + String(cd[3])
      );
    }
  #endif

  unsigned long et = micros(); //end time
  unsigned long dt = loopTime - (et - st); //delay time
  if(dt > 0)
    delayMicroseconds(loopTime);
}

//0 = On 1 = Off
void sendControl(int action, int sensor) {
  #ifdef ENABLE_KEYBOARD
    sendControlKB(action, sensor);
  #endif
  #ifdef ENABLE_LEDS
    sendControlLED(action, sensor);
  #endif
}

void sendControlKB(int action, int sensor) {
  switch(action){
    case 1:
      Keyboard.release(key[sensor]);
      break;
    case 0:
      Keyboard.press(key[sensor]);
      break;
  }
}

void sendControlLED(int action, int sensor) {
  switch(action){
    case 1:
      digitalWrite(ledPins[sensor], LOW);
      break;
    case 0:
      digitalWrite(ledPins[sensor], HIGH);
      break;
  }
}

