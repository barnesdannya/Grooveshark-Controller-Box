// Craftoe: the sound box

// Volume display init
const int LEDcount = 4;
const int volumeLEDs[LEDcount] = {2, 3, 4, 5}; //the LED pins

// Ultrasound sensor init
const int ultraSoundSignal = 10;

// Servo init
#include <Servo.h>
int pos;
Servo myservo;
const int arraySize = 20;
int arrayPosition;
int array[arraySize];

// Pushbutton init
const bool PLAY = true;
const bool PAUSE = false;
bool playPauseState;
const int playPausePin = 8;

void setup() 
{ 
  // Volume display: 4 LEDs
  for(int i=0; i<LEDcount; i++) {
    pinMode(volumeLEDs[i], OUTPUT);
    digitalWrite(volumeLEDs[i], LOW);
  }

  // Ultrasound sensor: tell how far person is from speaker
  pinMode(ultraSoundSignal, INPUT);
  Serial.begin(9600); // for debugging

  arrayPosition = 0;
  for(int i=0; i<arraySize; i++) {
    array[i] = 0;
  }

  // Servo output: position indicates sensor reading
  pos = 0;
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 

  // Pushbutton: play/pause
  pinMode(playPausePin, INPUT);
  playPauseState = PAUSE;
} 

void loop() 
{
  // Ultrasound sensor: tell how far person is from speaker
  if(arrayPosition == arraySize) arrayPosition = 0;
  pos = ping();
  pos = map(pos, 0, 21405, 0, 180); //map the min and max sensor values to the range of the servo
  array[arrayPosition] = pos;
  arrayPosition++; //using an array to store value history, used to move the servo based
                    //on the average of the last 20 sensor values to make movement less erratic

  // Servo output: position indicates sensor reading
  pos = 180 - average_of_array();
  myservo.write(pos);
  
  // Volume display: 4 LEDs
  //read the number of LEDs to turn on,
  //this is sent from a python script on the computer
  Serial.print('D'); //send the distance message signal
  Serial.println(pos); //send the detected distance to the python script
  while(Serial.available() <= 0) ; //wait for the response
  int data = Serial.parseInt(); //get the response
  for(int i=LEDcount; i>data; i--) {
    digitalWrite(volumeLEDs[i], LOW); //turn off the necessary LEDs
  }
  for(int i=0; i<data && i<LEDcount; i++) {
    digitalWrite(volumeLEDs[i], HIGH); //turn on the necessary LEDs
  }

  // Pushbutton: play/pause
  if(digitalRead(playPausePin) == LOW) { //if the button is down
    while(digitalRead(playPausePin) == LOW) ; //wait for button up
    if(playPauseState == PLAY) playPauseState = PAUSE;
    else playPauseState = PLAY; //toggle the playPauseState
    /* call to the appropriate API function
    needs
    to
    go
    here */
  }
} 

// Get data from ultrasound sensor
unsigned long ping() {
  pinMode(ultraSoundSignal, OUTPUT); // Switch signalpin to output
  digitalWrite(ultraSoundSignal, LOW); // Send low pulse
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
  delayMicroseconds(5); // Wait for 5 microseconds
  digitalWrite(ultraSoundSignal, LOW); // Holdoff
  pinMode(ultraSoundSignal, INPUT); // Switch signalpin to input
  digitalWrite(ultraSoundSignal, HIGH); // Turn on pullup resistor
  int value = pulseIn(ultraSoundSignal, HIGH); //Listen for echo
  if(value == 0) return ping();
  return value;
}

unsigned long average_of_array() {
  int sum = 0;
  for(int i=0; i<arraySize; i++) {
    sum += array[i];
  }
  return sum / 20;
}