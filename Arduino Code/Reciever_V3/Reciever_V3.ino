//Reciever 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>                      // Include servo library
Servo servoLeft;                        // Declare left and right servos
Servo servoRight;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00011";

void setup() {
  Serial.begin(9600);
  servoLeft.attach(9);                
  servoRight.attach(10);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text;
    radio.read(&text, sizeof(text));
    Serial.print("Message: ");
    Serial.println(text);
    if (text == 0x4C)
    {
      // Serial.println("LEFT");
      turnLeft(300);
      delay(1000);
    }
    else if (text == 0x52)
    {
      // Serial.println("RIGHT");
      turnRight(300);
      delay(1000);
    }
    // Serial.println(code);
    delay(500);
  }
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