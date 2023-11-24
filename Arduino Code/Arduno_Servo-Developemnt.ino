//INITIALISATION
int UD = 0;
int LR = 0;
int speedValue = 1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  UD = analogRead(A0); //Blue wire
  LR = analogRead(A1); //Yellow wire

  if(UD < 10){
    Serial.println("Forward");
  }
  else if(UD > 1000){
    Serial.println("Backward");
  }
  else if(LR < 10){
    Serial.println("Right");
  }
  else if(LR > 1000){
    Serial.println("Left");
  }
  
  delay(500);
}


/*
Serial.print("UD = ");
Serial.print(UD);
Serial.print(", LR = ");
Serial.println(LR);  
*/ 

void forward(int time)                  // Forward function
{
  servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  delay(time);                          // Maneuver for time ms
}

void turnLeft(int time)                 // Left turn function
{
  servoLeft.writeMicroseconds(1300);    // Left wheel clockwise
  servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  delay(time);                          // Maneuver for time ms
}

void turnRight(int time)                // Right turn function
{
  servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700);   // Right wheel counterclockwise
  delay(time);                          // Maneuver for time ms
}

void backward(int time)                 // Backward function
{
  servoLeft.writeMicroseconds(1300);    // Left wheel clockwise
  servoRight.writeMicroseconds(1700);   // Right wheel counterclockwise
  delay(time);                          // Maneuver for time ms
}
