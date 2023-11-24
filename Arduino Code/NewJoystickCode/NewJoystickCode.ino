#include <Servo.h>

int value;
bool reverse;

//Map 0-1023 joystick value to 0-254. hence 127 is the center value which we send.
//It also adjust the deadband in joystick as it's center value is not always 511. It is little different.
//So we need to add some deadband to center value. in our case 500-530. Any value in this deadband range is mapped to center 127.
//Jotstick values range from 0-1023. 
int mapAndAdjustJoystickDeadBandValues(int value, bool reverse)
{
  if (value >= 530)
  {
    value = map(value, 530, 1023, 127, 254);
  }
  else if (value <= 500)
  {
    value = map(value, 500, 0, 127, 0);  
  }
  else
  {
    value = 127;
  }

  if (reverse)
  {
    value = 254 - value;
  }
  return value;
}

void setup(){
  
  Serial.begin(9600);
  Serial.println("SETUP------------------");
}

void loop()
{
  value = analogRead(A0);
  Serial.println(mapAndAdjustJoystickDeadBandValues(value,reverse));
  delay(500);
}

