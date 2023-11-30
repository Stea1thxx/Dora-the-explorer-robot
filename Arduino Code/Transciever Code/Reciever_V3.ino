//Reciever 

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>                      // Include servo library
Servo servoLeft;                        // Declare left and right servos
Servo servoRight;

RF24 radio(7, 8); // CE, CSN

struct data_package
{
  int msg;
};
const byte addresses[][6] = {"00011"};
typedef struct data_package Package;
Package data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  // radio.startListening();
  radio.setChannel(115);  //115 band above WIFI signals
  // radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate( RF24_250KBPS ) ;  //Minimum speed

  servoLeft.attach(9);                
  servoRight.attach(10);

  Serial.print("Setup Initialized");
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


void WriteData()
{
  radio.stopListening(); //Stop Receiving and start transminitng
  radio.openWritingPipe(0xF0F0F0F066);//Sends data on this 40-bit address
  radio.write(&data, sizeof(data));
  Serial.print("\nSent:");
  Serial.println(data.msg);
  delay(300);
}

void ReadData()
{
radio.openReadingPipe(1, 0xF0F0F0F0AA); //Which pipe to read, 40 bit Address
  radio.startListening(); //Stop Transminting and start Reveicing
  if ( radio.available())
  {
    while (radio.available())
    {
      radio.read( &data, sizeof(data) );
    }
    Serial.print("\nReceived:");
    Serial.println(data.msg);
  }
}
