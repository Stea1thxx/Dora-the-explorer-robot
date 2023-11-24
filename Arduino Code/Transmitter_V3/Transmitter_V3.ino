//transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SoftwareSerial.h> //may not need

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00011";

// const int UP_BUTTON = 0; //CHANGE
// const int DOWN_BUTTON = 0; //CHANGE
const int LEFT_BUTTON = 5;
const int RIGHT_BUTTON = 4;   

// previous states:
// int up_lastState = LOW;
// int down_lastState = LOW;
int left_lastState = LOW;
int right_lastState = LOW;            

// current states:
// int up_currentState;
// int down_currentState;
int left_currentState;
int right_currentState; 


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  // The following sets each button as a pull-up input
  // HIGH when switch is open and LOW if the switch is closed
  // pinMode(UP_BUTTON, INPUT_PULLUP);
  // pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
}

void loop() {
  char text;
  // The following reads the state of each button
  // up_currentState = digitalRead(UP_BUTTON);
  // down_currentState = digitalRead(DOWN_BUTTON);
  left_currentState = digitalRead(LEFT_BUTTON);
  right_currentState = digitalRead(RIGHT_BUTTON);

  // Left button:
  if(left_lastState == HIGH && left_currentState == LOW)
  {
    Serial.println("left button is pressed");
    text = "L";
    radio.write(&text, sizeof(text));
  }
  else if(left_lastState == LOW && left_currentState == HIGH)
  {
    // Serial.println("left button is released");
  }

  // Right button:
  if(right_lastState == HIGH && right_currentState == LOW)
  {
    Serial.println("right button is pressed");
    text = "R";
    radio.write(&text, sizeof(text));
  }
  else if(right_lastState == LOW && right_currentState == HIGH)
  {
    // Serial.println("right button is released");
  }
  else
  {
    text = "";
  }

  // The following saves the last state of each button
  // up_lastState = up_currentState;
  // down_lastState = down_currentState;
  left_lastState = left_currentState;
  right_lastState = right_currentState;


  delay(100);
}
