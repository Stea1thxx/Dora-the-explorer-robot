// Reciever
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <stdio.h>
#include "printf.h"
#include <Servo.h>

const int electroMagnet = 5;
Servo servoLeft;
Servo servoRight;
int joystick[3];
int servoLeftPW;
int servoRightPW;
bool magnetState;

// nRF24L01 radio set up on SPI bus plus pins 7 and 8 
RF24 radio(7,8);

// pipe addresses - allowing communication between 2 nodes
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// The various roles supported by this sketch @X
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles @X
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch @X
role_e role = role_pong_back;

void setup() {
  Serial.begin(57600);
  //electromagnet output
  pinMode(electroMagnet, OUTPUT);
  digitalWrite(electroMagnet, LOW);

  // ATTACH SERVOS TO PINS 9 AND 10
  servoLeft.attach(9);                
  servoRight.attach(10);
  printf_begin();
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  radio.begin();
  radio.setAutoAck(false);
  // radio.setChannel(uint8_t channel)

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  if (role == role_ping_out)
  {
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1,pipes[1]);
  }
  else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  radio.startListening();

  // data to ensure radios are working:
  radio.printDetails();

}

void loop() {
  
  if ( role == role_pong_back )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( joystick, sizeof(joystick) );

        // Spew it
        for (int i = 0; i < sizeof(joystick) / sizeof(joystick[0]); i++) {
          Serial.print("Got payload: ");
          Serial.println(joystick[i]);
        }
        Serial.println();
        
        // extract left and right values:
        servoLeftPW = joystick[0];
        servoRightPW = joystick[1];
        magnetState = joystick[2];
        
        Serial.println("LEFT: "+ String(servoLeftPW));
        Serial.println("RIGHT: "+ String(servoRightPW));
        Serial.println("Magnet state is" + String(magnetState));

        // Servo Outputs
        servoLeft.writeMicroseconds(servoLeftPW);    // Left wheel counterclockwise
        servoRight.writeMicroseconds(servoRightPW);   // Right wheel clockwise
        Serial.println("Left Servo: " + String(servoLeftPW));
        Serial.println("Right Servo: " + String(servoRightPW));
        //Serial.println("turning: " + String(turning));
        Serial.println(" ");

        //electromagnet switch on/off
        if (magnetState == 0 ) {
          digitalWrite(electroMagnet, LOW);
        } else if (magnetState == 1) {
          digitalWrite(electroMagnet, HIGH);
        } else {
          Serial.print("ElectroMagnet package error, kys");
        }


        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();
      unsigned long time_taken = millis();

      // Send the final one back.
      radio.write( &time_taken, sizeof(unsigned long) );
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  }
}
