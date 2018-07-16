//Lora SENDER
#include <Arduino.h>
#include <LoRa.h>
#define ON_CNFRM 4
#define OFF_CNFRM 5
#define PING_PIN 6
long lastSendTime=0;
int  interval=50;
long lastSendTime1=0;
int  interval1=50;
bool state = false;
void check_lora_connection(void);
String send_data(String);
String receive_data(void);
const int buttonPin_NO = 2;
const int buttonPin_NC = 3;
void onReceive(int);
int buttonState_NO = 0;
int buttonState_NC = 0;
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin_NO, INPUT_PULLUP);
  pinMode(buttonPin_NC, INPUT_PULLUP);
  pinMode(ON_CNFRM, OUTPUT);
  pinMode(OFF_CNFRM, OUTPUT);
  pinMode(PING_PIN, OUTPUT);
  Serial.println("STARTING-LORA-SEND");
  check_lora_connection();
  digitalWrite(ON_CNFRM, LOW);
  digitalWrite(OFF_CNFRM, HIGH);
  Serial.println("LORA Connected");
  send_data("hello testing sender here ");
}

void loop()
{
  onReceive(LoRa.parsePacket());
 if (millis() - lastSendTime1 > interval1)
 {
  buttonState_NO = digitalRead(buttonPin_NO);
    if (buttonState_NO == LOW)
    {
    // turn LED on:
    send_data("ON");
    }
  lastSendTime1 = millis();
}
if (millis() - lastSendTime > interval)
{
  buttonState_NC = digitalRead(buttonPin_NC);
  if (buttonState_NC == LOW)
  {
    // turn LED on:
    send_data("OFF");
  }
  lastSendTime = millis
}
}

String send_data(String data)
{
    String temp_data = "send successfull";
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();
    return temp_data;
  }

void check_lora_connection()
{
  if (!LoRa.begin(433E6)) {
   Serial.println("Starting LoRa failed!");
   while (1);
 }
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;
  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println();
  if(incoming =="Motor Started successfully" || incoming=="Already On")
  {
    digitalWrite(ON_CNFRM, HIGH);
    digitalWrite(OFF_CNFRM, LOW);
  }
  if(incoming =="Motor Stopped successfully" || incoming=="Already OFF")
  {
    digitalWrite(ON_CNFRM, LOW);
    digitalWrite(OFF_CNFRM, HIGH);
  }
}
