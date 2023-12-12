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
const int magnetButton = 5;
int buttonState = LOW;
int lastButtonState = LOW;
bool magnetState = false; 
  

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

// The various roles supported by this sketch @X
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles @X
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch @X
role_e role = role_ping_out;


void setup(void)
{
  pinMode(magnetButton, INPUT_PULLUP);

  Serial.begin(57600);
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
    joystick[2] = magnetState;

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
    joystick[0] = map(valueY, 530, 1023, 1500, 1700);
    joystick[1] = map(valueY, 530, 1023, 1500, 1300);
  }
  else if(valueY < 500)
  {
    Serial.println("Down");
    joystick[0] = map(valueY, 500, 0, 1500, 1300);
    joystick[1] = map(valueY, 500, 0, 1500, 1700);
  }
  //TODO: LEFT AND RIGHT MAPING
  else if (valueX >= 530)
  {
    Serial.println("Left");
    turning = map(valueX, 530, 1023, 0, 200);
    joystick[1] -= turning;
  }
  else if(valueX < 500)
  {
    Serial.println("Right");
    turning = map(valueX, 500, 0, 0, 200);
    joystick[0] += turning;
  }
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  // The following reads the state of each button
  // left_currentState = digitalRead(LEFT_BUTTON);
  // right_currentState = digitalRead(RIGHT_BUTTON);

  if (role == role_ping_out)
  {
    radio.stopListening();

    // Read joystick values, send them across.  This will block until complete
    // values[0] = valueY; //UP AND DOWN, (VRY) - Yellow wire (ValueY)
    // values[1] = valueX; //LEFT and DOWN, (VRX) - Yellow wire (ValueX)
    // // printf("Now sending %lu..." + String(values[0]) + "and" + String(values[1]));
    // Serial.print("servoLeftPW: ");
    // // Serial.println(servoLeftPW);
    // Serial.print("servoRightPW: ");
    // Serial.println(servoRightPW);
    //LeftandRight = (long(servoLeftPW) * 10000) + long(servoRightPW);
    Serial.println("LEFT: "+ String(joystick[0]));
    Serial.println("RIGHT: "+ String(joystick[1]));
    for (int i = 0; i < sizeof(joystick) / sizeof(joystick[0]); i++) {
      Serial.print("Now sending: ");
      Serial.println(joystick[i]);
    }
    Serial.println();
    // Serial.print(LeftandRight);
    // COMBINED = (String(servoLeftPW) + String(servoRightPW));
    // values[0] = servoLeftPW;
    // values[1] = servoRightPW;
    // Serial.println(values[0]);
    // Serial.println(values[1]);
    

    //printf("Now sending %li...",LeftandRight);
    bool check = radio.write( joystick, sizeof(joystick));  // True of False
    
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