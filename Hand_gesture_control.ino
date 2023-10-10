/*
 * Project      : Hand Gesture and Joystick Control
 * Board        : Arduino Leonardo
 * Components   : Joystick Module, IR Sensor Modules (4), 
 *                HC-SR04 Ultrasonic Sensor, Breadboard,
 *                Resistors (4*220 ohm, 1*10K ohm),
 *                Small Button, LEDs (5), F-M Dupont wires,
 *                Jumper wires, Arduino Loonardo Board-Cable-
 *                PC.
 * Description  : It is a controller for PC/laptop,
 *                which can be used either by a Joystick 
 *                or hand gestures upon IRs to control  
 *                a video (for now arrow keys; forward, 
 *                backward, volume up and down, space key
 *                for pause and play, enter with button
 *                for making full screen or small screen),
 *                or a game with arrow keys, space and enter
 *                for basic game controls for games 
 *                such as car racing games, snake, pacman etc.  
 *                Ultrasonic sensor will limit the max distance
 *                for which IR sensors will detect gesture.
 *                LEDs will show the direction for both
 *                Joystick move or hand gestures for IR sensors.
 * Author       : prgrmcode

*/

// Joystick:  
#include <Keyboard.h> // Library to use joystick module as keyboard   
// Built-in Library by Arduino-Version 1.0.2 reference: https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
#define joyX A0 // X axis of the Joystick connected to Analog pin
#define joyY A1 // Y axis of the Joystick connected to Analog pin
#define joyButton 2 // The switch button output of the Joystick is connected to digital Pin 2
#define yellowLED 3 // Yellow LED is attached to digital pin 3
#define redLED 4
#define greenLED 5
#define blueLED 6
#define whiteLED 7 // White LED is attached to pin 7 for joystick switch
#define BUTTON 8 // push button on digital pin 8
#define BOUNCETIME 20 // for push button
#define DELAY 50 // delay of IR reading

int xVal, yVal;
int buttonState = 0; // The beginning value of buttonstate of joystick
int buttonState1 = 0; // The initial value of buttonstate1 of joystick

unsigned char reading_button, previous1 = LOW;
long int time, toggled = 0;

// IR gesture:
// got help on using the correct variable types https://create.arduino.cc/projecthub/najad/playing-nfs-by-hand-gesture-control-ba3d8a
#define IR_UR 9 // IR senson on up right position connected to digital PIN 9
#define IR_UL 10 // IR sensor on up left position
#define IR_DR 11 // IR sensor on down right position
#define IR_DL 12 // IR sensor on down left position
#define trig A3 // Ultrasonic Sensor trigger on analog pin A3
#define echo A4 // US sensor echo is wired on A4
float duration = 0; // US sensor time passed for echo
float distance = 0; // US sensor distance read
bool state_up_right, state_up_left, state_down_right, state_down_left; // IR states

//joystick:
int readButtonVal(int button) {
  // function to read input from push button
  reading_button = digitalRead(button);
  // my design for push button: https://www.tinkercad.com/things/35Fe85Z0aIH-swanky-habbi
  
  if (reading_button == HIGH && previous1 == LOW)
  {
    return 1;
    time = millis(); // to get the time for button press
  } else {
    return 0;
  }
  previous1 = reading_button;
  delay(125);
}

// Ultrasonic sensor;
int US_read() {
  // got help from https://circuitdigest.com/microcontroller-projects/arduino-ultrasonic-sensor-based-distance-measurement
  digitalWrite(trig, LOW); // writes low to ultrasonic sensor; the module is not sending any signal 
  // https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/ :
  delayMicroseconds(5);
  digitalWrite(trig, HIGH); // writes high to US sensor; it sends an ultrasonic signal.
  delayMicroseconds(11);
  digitalWrite(trig, LOW); // writes low to ultrasonic sensor; so that the module doesn't send any US signal 
  delayMicroseconds(3);
  // https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/ :
  duration = pulseIn(echo, HIGH); // got the signal we sent and determine the time passed
  distance = (duration / 2) * 0.0343; // distance = time * speed (speed of the sound = 0.0343 cm/µs)
  return distance;
}

// IR states;
int IR_states() {
  Serial.print("Sensor States:");
  state_up_right = digitalRead(IR_UR); 
  Serial.print(state_up_right);
  Serial.print("\t");
  state_up_left = digitalRead(IR_UL);
  Serial.print(state_up_left);
  Serial.print("\t");
  state_down_right = digitalRead(IR_DR);
  Serial.print(state_down_right);
  Serial.print("\t");
  state_down_left = digitalRead(IR_DL);
  Serial.print(state_down_left);
  Serial.print("\n");
}

// read the gestures with IR sensors;
int IR_gestures() {
  // keyboard keys list (keys-decimal): https://www.arduino.cc/en/Reference/KeyboardModifiers
  if (state_up_right == 0 && state_up_left == 0) {
    // up states are read from digital read
    Keyboard.press(KEY_UP_ARROW);
    digitalWrite(yellowLED, HIGH);
    Serial.print("UP\n");
    delay(DELAY);       
  } else{
    // https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardreleaseall/
    Keyboard.releaseAll();
    digitalWrite(yellowLED, LOW); 
  }

  if (state_up_right == 0 && state_down_right == 0) {
    // right states are read
    Keyboard.press(KEY_RIGHT_ARROW);
    digitalWrite(redLED, HIGH);
    Serial.print("RIGHT\n");
    delay(DELAY);
        
  } else{
    Keyboard.releaseAll();
    digitalWrite(redLED, LOW);
  }

  if (state_down_right == 0 && state_down_left == 0) {
    // down states are read
    Keyboard.press(KEY_DOWN_ARROW);
    digitalWrite(blueLED, HIGH);
    Serial.print("DOWN\n");
    delay(DELAY);
    
  } else{
    Keyboard.releaseAll(); 
    digitalWrite(blueLED, LOW);
  }

  if (state_up_left == 0 && state_down_left == 0) {
    // left states are read
    Keyboard.press(KEY_LEFT_ARROW);
    digitalWrite(greenLED, HIGH);
    Serial.print("LEFT\n");
    delay(DELAY);
     
  } else{
    Keyboard.releaseAll();
    digitalWrite(greenLED, LOW);
  }

  // define moving to 4 corners;
  if (state_up_right == 0 && state_up_left == 1 && state_down_right == 1 && state_down_left == 1) {
    // only up right state is read
    Keyboard.press(KEY_UP_ARROW);
    digitalWrite(yellowLED, HIGH);
    Keyboard.press(KEY_RIGHT_ARROW);
    digitalWrite(redLED, HIGH);
    Serial.print("UP RIGHT CORNER");
    delay(DELAY);
     
  } else{
    Keyboard.releaseAll();
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
  }

  if (state_up_left == 0 && state_up_right == 1 && state_down_right == 1 && state_down_left == 1) {
    // only up left state is read
    Keyboard.press(KEY_UP_ARROW);
    digitalWrite(yellowLED, HIGH);
    Keyboard.press(KEY_LEFT_ARROW);
    digitalWrite(greenLED, HIGH);
    Serial.print("UP LEFT CORNER");
    delay(DELAY);
     
  } else{
    Keyboard.releaseAll();
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }

  if (state_down_right == 0 && state_up_left == 1 && state_up_right == 1 && state_down_left == 1) {
    // only down right state is read
    Keyboard.press(KEY_DOWN_ARROW);
    digitalWrite(blueLED, HIGH);
    Keyboard.press(KEY_RIGHT_ARROW);
    digitalWrite(redLED, HIGH);
    Serial.print("DOWN RIGHT CORNER");
    delay(DELAY);
     
  } else{
    Keyboard.releaseAll();
    digitalWrite(blueLED, LOW);
    digitalWrite(redLED, LOW);
  }

  if (state_down_left == 0 && state_up_left == 1 && state_down_right == 1 && state_up_right == 1) {
    // only down left state is read
    Keyboard.press(KEY_DOWN_ARROW);
    digitalWrite(blueLED, HIGH);
    Keyboard.press(KEY_LEFT_ARROW);
    digitalWrite(greenLED, HIGH);
    Serial.print("DOWN LEFT CORNER");
    delay(DELAY);
    
  } else{
    Keyboard.releaseAll(); 
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, LOW);
  }

  if (state_down_left == 0 && state_up_left == 0 && state_down_right == 0 && state_up_right == 0) {
    // all states are read
    Keyboard.write(' '); // press the space key on keyboard
    Serial.println("SPACE\n");
    digitalWrite(whiteLED, HIGH);
    delay(DELAY);
  } else{
    digitalWrite(whiteLED, LOW);
  }

  // https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardend/ :
  Keyboard.end();
}

void setup() {
  // joystick:
  Serial.begin(9600); // baud rate
  Keyboard.begin();
  pinMode(yellowLED,OUTPUT); // Pin 3 on which the Yellow LED is, defined as output
  pinMode(redLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(blueLED,OUTPUT);
  pinMode(whiteLED,OUTPUT); // Pin 7 on which the white LED is, defined as output
  pinMode(joyButton,INPUT); // Pin 2 on which the switch of the joystick is, is defined as input
  digitalWrite(joyButton, HIGH); // Initial value of joystick button is High
  pinMode(BUTTON, INPUT); 
  
  // IR:
  pinMode(IR_UR, INPUT); // define 4 IR sensors as input
  pinMode(IR_UL, INPUT);
  pinMode(IR_DR, INPUT);
  pinMode(IR_DL, INPUT);
  pinMode(trig, OUTPUT); // trigger of US sensor defined as output
  pinMode(echo, INPUT);  // echo pin of US sensor defined as input
}
 
void loop() {
  // joystick:
  xVal = analogRead(joyX); // Reading x value of joy X pin-A0
  yVal = analogRead(joyY); // Reading y value of joy Y pin-A1
 
  Serial.print("xVal: "); // Printing the values read from joystick
  Serial.print(xVal); // Printing the values read from joystick
  Serial.print("\n");
  Serial.print("yVal: ");
  Serial.print(yVal);
  Serial.print("\n");
  
  buttonState = digitalRead(joyButton); // Reading the value of joystick button on pin 2
  Serial.print("buttonState: ");
  Serial.print(buttonState);
  Serial.print("\n");

  /*
  //test leds:
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, HIGH);
  digitalWrite(yellowLED, HIGH);
  digitalWrite(blueLED, HIGH);
  delay(2000);
  */  

  
  // for reference of keyboard keys list (keys-decimal): https://www.arduino.cc/en/Reference/KeyboardModifiers
  /*
    KEY_UP_ARROW 0xDA  218
    KEY_DOWN_ARROW  0xD9  217
    KEY_LEFT_ARROW  0xD8  216
    KEY_RIGHT_ARROW 0xD7  215
    KEY_BACKSPACE 0xB2  178
    KEY_TAB 0xB3  179
    KEY_RETURN  0xB0  176
   
  */

  // joystick movement in left -x direction
  if (xVal<=120)
  {
    digitalWrite(greenLED, HIGH);
    Keyboard.press(KEY_LEFT_ARROW); // left key move
  }
  else{
    digitalWrite(greenLED, LOW);
    Keyboard.release(KEY_LEFT_ARROW); // release the left arrow key
  }

  // joystick movement in right +x direction:
  if (xVal>=950)
  {
    digitalWrite(redLED, HIGH);
    Keyboard.press(KEY_RIGHT_ARROW); // right key move
  }
  else{
    digitalWrite(redLED, LOW);
    Keyboard.release(KEY_RIGHT_ARROW); // release the right arrow key
  }

  // joystick movement in up -y direction:
  if (yVal<=120)
  {
    digitalWrite(yellowLED, HIGH);
    Keyboard.press(KEY_UP_ARROW); // key moving to up
  }
  else{
    digitalWrite(yellowLED, LOW);
    Keyboard.release(KEY_UP_ARROW); // release the up arrow key
  }

  // joystick movement in down +y direction:
  if (yVal>=950)
  {
    digitalWrite(blueLED, HIGH);
    Keyboard.press(KEY_DOWN_ARROW); // key moving to down
  }
  else{
    digitalWrite(blueLED, LOW);
    Keyboard.release(KEY_DOWN_ARROW); // release the down arrow key
  }

  // press on pushbutton switch of the joystick:
  // If it is pressed white Led will turn on as notification and SPACE key will be pressed on keyboard
  if (buttonState == LOW)
  {
    Serial.println("SPACE; Switch = High, on");
    digitalWrite(whiteLED, HIGH);
    Keyboard.write(' '); // press the space key on keyboard
  }
  else{
    digitalWrite(whiteLED, LOW);
  }
  buttonState1 = digitalRead(whiteLED);
  Serial.print("button State1: ");
  Serial.print(buttonState1);
  Serial.print("\n");
  delay(100);

  int pressed = readButtonVal(BUTTON); // if the button is pressed:
  if(pressed == 1 && toggled == 0) {    
    Keyboard.println(); // press the enter key on keyboard
    delay(100);  
    toggled = 1;
  } else if(pressed == 1 && toggled == 1) {    
    toggled = 0;
  }

  // IR sensors:
  // US sensor distance value
  int sensorVal = US_read();
  delayMicroseconds(3);
  Serial.print("Distance: ");  
  Serial.print(sensorVal);
  Serial.print("cm\n"); // if distance of the hand to the sensors is less than 15 cm, then read IR values;
  if(sensorVal <= 15) {
    // read the states of IR sensors;
    IR_states();
    IR_gestures();
  }
  
}
