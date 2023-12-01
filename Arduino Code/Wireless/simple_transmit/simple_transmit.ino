// transmit
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8, 4000000); // CE, CSN

const byte address[1] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(true);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  Serial.println("SENT");
  delay(1000);
}