// Craftoe: the sound box

// Volume display init
const int volumeLEDs[4] = {1, 2, 3, 4}; //the LED pins

// Ultrasound sensor init
const int ultraSoundSignal = 10;

// Servo init
#include <Servo.h>
int pos;
Servo myservo;
int arrayPosition;
int array[20];

// Pushbutton init

void setup() 
{ 

  // Volume display: 4 LEDs

  // Ultrasound sensor: tell how far person is from speaker
  pinMode(ultraSoundSignal, INPUT);
  Serial.begin(9600); // for debugging

  arrayPosition = 0;
  for(int i=0; i<20; i++) {
    array[i] = 0;
  }


  // Servo output: position indicates sensor reading
  pos = 0;
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 

  // Pushbutton: play/pause

} 


void loop() 
{

  // Ultrasound sensor: tell how far person is from speaker
  if(arrayPosition == 20) arrayPosition = 0;
  pos = ping();
  pos = map(pos, 0, 21405, 0, 180);
  array[arrayPosition] = pos;
  arrayPosition++;

  // Servo output: position indicates sensor reading
  pos = 180 - average_of_array();
  myservo.write(pos);
  
  // Volume display: 4 LEDs
  //read the number of LEDs to turn on,
  //this is sent from a python script on the computer
  Serial.write(pos); //send the detected distance to the python script
  if(Serial.available()>0)
  {
    int data = Serial.read(); //get the number of LEDs to turn on from the python script
    for(int i=0; i<4; i++) {
      digitalWrite(volumeLEDs[i], LOW); //turn off the LEDs
    }
    for(int i=0; i<data; i++) {
      digitalWrite(volumeLEDs[i], HIGH); //turn on the necessary LEDs
    }
  }

  // Pushbutton: play/pause

} 

// Get data from ultrasound sensor
unsigned long ping(){
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
   for(int i=0; i<20; i++) {
     sum += array[i];
   }
   return sum / 20;
 }