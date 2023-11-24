//INITIALISING VARIABLES
#include <Servo.h>
Servo servoLeft;                        // Declare left and right servos
Servo servoRight;
int value;
int servoLeftPW;
int servoRightPW;
bool reverse;

void setup(){
  Serial.begin(9600);
  Serial.println("SETUP------------------");
  servoLeft.attach(9);                
  servoRight.attach(10);
}

void loop()
{
  value = analogRead(A0); //UP AND DOWN, (VRY) - Yellow wire

  //Maping JS to Servos
  if (value >= 530)
  {
    servoLeftPW = map(value, 530, 1023, 1500, 1700);
    servoRightPW = map(value, 530, 1023, 1500, 1300);
  }
  else
  {
    //Stays still
    servoLeftPW = 1500;
    servoRightPW = 1500;
  }

  servoLeft.writeMicroseconds(servoLeftPW);    // Left wheel counterclockwise
  servoRight.writeMicroseconds(servoRightPW);   // Right wheel clockwise
  Serial.println("Left: " + String(servoLeftPW));
  Serial.println("Right: " + String(servoRightPW));
  //delay(500);
}
