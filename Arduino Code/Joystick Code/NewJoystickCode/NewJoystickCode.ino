//INITIALISING VARIABLES
#include <Servo.h>
Servo servoLeft;
Servo servoRight;
int valueY;
int valueX;
int servoLeftPW;
int servoRightPW;
int turning;

void setup(){
  Serial.begin(9600);
  Serial.println("SETUP------------------");
  servoLeft.attach(12);                
  servoRight.attach(13);
}

void loop()
{
  //TODO: STAYS STILL IF NOT DISPLACING ON JS
  servoLeftPW = 1500;
  servoRightPW = 1500;
  turning = 0;


  valueY = analogRead(A0); //UP AND DOWN, (VRY) - Yellow wire
  valueX = analogRead(A1); //LEFT and DOWN, (VRX) - Yellow wire
  //Orientation: GND bottom - SW Top
  //Deadband: 500-530



  //Maping JS to Servos
  //TODO: UP AND DOWN MAPING
  if (valueY >= 530)
  {
    Serial.println("Up");
    servoLeftPW = map(valueY, 530, 1023, 1500, 1700);
    servoRightPW = map(valueY, 530, 1023, 1500, 1300);
  }
  else if(valueY < 500)
  {
    Serial.println("Down");
    servoLeftPW = map(valueY, 500, 0, 1500, 1300);
    servoRightPW = map(valueY, 500, 0, 1500, 1700);
  }
  //TODO: LEFT AND RIGHT MAPING
  else if (valueX >= 530)
  {
    Serial.println("Left");
    turning = map(valueX, 530, 1023, 0, 200);
    servoRightPW -= turning;
  }
  else if(valueX < 500)
  {
    Serial.println("Right");
    turning = map(valueX, 500, 0, 0, 200);
    servoLeftPW += turning;
  }


  //Outputs
  servoLeft.writeMicroseconds(servoLeftPW);    // Left wheel counterclockwise
  servoRight.writeMicroseconds(servoRightPW);   // Right wheel clockwise
  Serial.println("Joystick Y axis: " + String(valueY));
  Serial.println("Joystick X axis: " + String(valueX));
  Serial.println("Left Servo: " + String(servoLeftPW));
  Serial.println("Right Servo: " + String(servoRightPW));
  //Serial.println("turning: " + String(turning));
  Serial.println(" ");
  //delay(500);
}
