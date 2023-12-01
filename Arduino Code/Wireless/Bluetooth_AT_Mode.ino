#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(3,2); //TX and RX)

void setup() {
  Serial.begin(9600);
  Bluetooth.begin(38400);
  Serial.println("Setup");
}

void loop() {
  // Read from HC-05 and send data to the Arduino Serial Monitor
  if (Bluetooth.available())
    Serial.write(Bluetooth.read());
  // Read from Arduino Serial Monitor and send it to the HC-05
  if (Serial.available())
    Bluetooth.write(Serial.read());  
}

//+ADDR:98D3:31:F5FA95
