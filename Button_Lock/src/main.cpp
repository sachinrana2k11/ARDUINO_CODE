#define ButtonPin1 2
#define ButtonPin2 3
#define LedPin 13
#include <Arduino.h>
int pin_reading1 = 0;
int pin_reading2 = 0;
int a = 0;
void setup()
{
  pinMode(ButtonPin1,INPUT_PULLUP);
  pinMode(ButtonPin2,INPUT_PULLUP);
  pinMode(LedPin,OUTPUT);
}
void loop()
{
  pin_reading1 = digitalRead(ButtonPin1);
  pin_reading2 = digitalRead(ButtonPin2);
  if(pin_reading1 == LOW)
  {
    if(a<1)
    {
      a = a+1;
    }
  }
  if(pin_reading2 == LOW)
  {
    if(a>0)
    {
      a = a-1;
    }
  }
  if(a>0)
  {
      digitalWrite(LedPin,HIGH);
  }
  if(a<1)
  {
      digitalWrite(LedPin,LOW);
  }
  else
  {
    digitalWrite(LedPin,LOW);
  }
}
