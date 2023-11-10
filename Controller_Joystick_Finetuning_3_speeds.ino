//INITIALISATION
int UD = 0;
int LR = 0;
int speedValue = 1;
#include <Servo.h>                      // Include servo library
Servo servoLeft;                        // Declare left and right servos
Servo servoRight;

int initialValuesUD = 0;

void setup() {
  UD = analogRead(A0); //Blue wire
  LR = analogRead(A1); //Yellow wire

  Serial.begin(9600);
  initialValuesUD = UD;
  Serial.println("SETUP------------------");
  Serial.println(initialValuesUD);
}

void loop() {
  UD = analogRead(A0); //Blue wire
  LR = analogRead(A1); //Yellow wire

//FORWARD AND BACKWARD ---------------------------------
  if ((UD >= 0) && (UD < 100)){
    Serial.println("Forward Max");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }
  
  else if ((UD > 100) && (UD < 400)){
    Serial.println("Forward Med");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }

    else if ((UD > 400) && (UD < 500)){
    Serial.println("Forward Min");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }
    else if ((UD >= 550) && (UD < 720)){
    Serial.println("Backward Min");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }
  
  else if ((UD > 720) && (UD < 900)){
    Serial.println("Backward Med");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }

    else if ((UD > 900) && (UD < 10123)){
    Serial.println("Backward Max");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }




//LEFT AND RIGHT ---------------------------------
  if ((LR >= 0) && (LR < 100)){
    Serial.println("Right Max");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }
  
  else if ((LR > 100) && (LR < 400)){
    Serial.println("Right Med");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }

   else if ((LR > 400) && (LR < 500)){
    Serial.println("Right Min");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }
   else if ((LR >= 550) && (LR < 720)){
    Serial.println("Left Min");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }
  
  else if ((LR > 720) && (LR < 900)){
    Serial.println("Left Med");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }

    else if ((LR > 900) && (LR < 10123)){
    Serial.println("Left Max");
//    servoLeft.writeMicroseconds(1700);    // Left wheel counterclockwise
//    servoRight.writeMicroseconds(1300);   // Right wheel clockwise
  }







  else{
    Serial.println("Rest");
    //servoLeft.writeMicroseconds(1500);    //Stay Still
    //servoRight.writeMicroseconds(1500);
  }


  Serial.print("UD = ");
  Serial.print(UD);
  Serial.print(", LR = ");
  Serial.println(LR);  
  delay(900);


  
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

/*TODO: Work on making robot move using percentages accoridng to base value
- Work on implementing the potentiometers?
- You coudl have a slalom vs normal driving mode

*/
