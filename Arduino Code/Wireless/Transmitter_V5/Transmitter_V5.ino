// Transmitter
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <stdio.h>
#include "printf.h"

// initialise variables:
int valueY;
int valueX;
int servoLeftPW;
int servoRightPW;
long LeftandRight;
int turning;
int joystick[3];
const int magnetButton = 4;
int buttonState = LOW;
int lastButtonState = LOW;
bool magnetState = false;

const int LEFT_BUTTON = 5;
const int RIGHT_BUTTON = 4;   

// previous states:
int left_lastState = LOW;
int right_lastState = LOW;            

// current states:
int left_currentState;
int right_currentState; 

// nRF24L01 radio set up on SPI bus plus pins 7 and 8 
RF24 radio(7,8);

// pipe addresses - allowing communication between 2 nodes
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// what role? 
typedef enum { transmit = 1, recieve } Role;

// const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// Tset the role of this sketch
Role role = transmit;


void setup(void)
{
  pinMode(magnetButton, INPUT_PULLUP);

  Serial.begin(57600);
  printf_begin();
  printf("ROLE: %s\n\r",role);


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
  delay(5000); // for debugging
}


void loop(void)
{

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  servoLeftPW = 1500;
  servoRightPW = 1500;
  turning = 0;

  buttonState = digitalRead(magnetButton);
  if (buttonState == LOW && lastButtonState == HIGH){
    magnetState = !magnetState;

    if (joystick[2]) {
      Serial.println("MAGNET ON");
    } else {
      Serial.println("MAGNET OFF");
    }
  }

  lastButtonState = buttonState;


  valueY = analogRead(A0); //UP AND DOWN, (VRY) - Yellow wire
  valueX = analogRead(A1); //LEFT and DOWN, (VRX) - Yellow wire
  //Orientation: GND bottom - SW Top
  //Deadband: 500-530
  // Serial.println(valueY);
  // Serial.println(valueX);



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
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  // The following reads the state of each button
  // left_currentState = digitalRead(LEFT_BUTTON);
  // right_currentState = digitalRead(RIGHT_BUTTON);

  if (role == transmit)
  {
    radio.stopListening();


    LeftandRight = (long(servoLeftPW) * 10000) + long(servoRightPW);
    Serial.println("LEFT: "+ String(servoLeftPW));
    Serial.println("RIGHT: "+ String(servoRightPW));
    Serial.println("DEBUG: "+ String(LeftandRight));
    

    printf("Now sending %li...",LeftandRight);
    bool check = radio.write(&LeftandRight, sizeof(LeftandRight));  // True of False
    
    if (check)
    {
      printf("Check success");
    }
    else 
    {
      printf("Failed. \n\r");
    }

    radio.startListening();

    // Wait here until we get a response, or timeout (250ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 200 )
        timeout = true;

    // Describe the results
    if ( timeout )
    {
      printf("Failed, response timed out.\n\r");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned long time_taken;
      radio.read( &time_taken, sizeof(unsigned long) );

      // Spew it
      printf("round-trip delay: %lu\n\r",millis()-time_taken);
  
  }
  delay(500);
  }}