/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int upper = 120; // P : / Y:   upper must be greater than lower...
int lower = 80; //P :  / Y :  
int a = 0;                                   

void setup() {
  pinMode(8,INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  delay(50);
}

void loop() {
  if(digitalRead(8)){
    upper = 120;
    lower = 80;
  }
  else{
    upper = 140;
    lower = 100;
  }
  for (pos = lower; pos <= upper; pos += 1) { // goes from 0 degrees to 180 degrees
     // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(9);                       // waits 15ms for the servo to reach the position
  }
  for (pos = upper; pos >= lower; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(9);                       // waits 15ms for the servo to reach the position
  }
 
  //myservo.write(90);
// myservo.write(upper);
// delay(1500);
// myservo.write(lower);
// delay(1500);
}
