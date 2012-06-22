// Craftoe: the sound box

// Volume display init

// Ultrasound sensor init
const int ultraSoundSignal = 10;

// Servo init
#include <Servo.h>
int pos;
Servo myservo;
int arrayPosition;
int array[5];

// Pushbutton init

void setup() 
{ 

  // Volume display: 4 LEDs

  // Ultrasound sensor: tell how far person is from speaker
  pinMode(ultraSoundSignal, INPUT);
  Serial.begin(9600); // for debugging

  arrayPosition = 0;
  for(int i=0; i<5; i++) {
    array[i] = 0;
  }


  // Servo output: position indicates sensor reading
  pos = 0;
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 

  // Pushbutton: play/pause

} 


void loop() 
{ 

  // Volume display: 4 LEDs

  // Ultrasound sensor: tell how far person is from speaker
  if(arrayPosition == 5) arrayPosition = 0;
  pos = ping();
  Serial.println(pos);
  pos = map(pos, 0, 21405, 0, 180);
  array[arrayPosition] = pos;
  arrayPosition++;

  // Servo output: position indicates sensor reading
  pos = 180 - average_of_array();
  myservo.write(pos);

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
   Serial.print("array(");
   for(int i=0; i<5; i++) {
     sum += array[i];
     Serial.print(array[i]);
     Serial.print(" ");
   }
   Serial.println(")");
   return sum / 5;
 }