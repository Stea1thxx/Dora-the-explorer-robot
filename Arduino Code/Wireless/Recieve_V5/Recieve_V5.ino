// Reciever
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <stdio.h>
#include "printf.h"
#include <Servo.h>
Servo servoLeft;
Servo servoRight;
int servoLeftPW;
int servoRightPW;

// nRF24L01 radio set up on SPI bus plus pins 7 and 8 
RF24 radio(7,8);

// pipe addresses - allowing communication between 2 nodes
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// The various roles supported by this sketch @X
typedef enum { transmit = 1, recieve } Role;

// The debug-friendly names of those roles @X
// const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch @X
Role role = recieve;

void setup() {
  Serial.begin(57600);
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

  if (role == transmit)
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
  
  if ( role == recieve )
  {
    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      long gotLeftRight;
      bool done = false;
      while (!done)
      {
        // fetch payload, check if last one
        done = radio.read( &gotLeftRight, sizeof(long) );

        // print payload
        printf("Got payload %li...",gotLeftRight);
        
        // extract left and right values:
        servoLeftPW = gotLeftRight / 10000;
        servoRightPW = gotLeftRight - (long(servoLeftPW) * 10000);
        Serial.println("LEFT: "+ String(servoLeftPW));
        Serial.println("RIGHT: "+ String(servoRightPW));

        // Servo Outputs
        servoLeft.writeMicroseconds(servoLeftPW);    // Left wheel counterclockwise
        servoRight.writeMicroseconds(servoRightPW);   // Right wheel clockwise
        Serial.println("Left Servo: " + String(servoLeftPW));
        Serial.println("Right Servo: " + String(servoRightPW));
        //Serial.println("turning: " + String(turning));
        Serial.println(" ");

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
