//Lora REC
#include <Arduino.h>
#include <LoRa.h>
#define NO_PIN 4
#define NC_PIN 5

bool NO_STATE = false;
bool NC_STATE = true;

int NO_PERIOD = 2000;
int NC_PERIOD = 2000;
unsigned long time_now_NC = 0;
unsigned long time_now_NO = 0;
void(* resetFunc) (void) = 0;
void check_lora_connection(void);
void Start_Motor(void);
void Stop_Motor(void);
String send_data(String);
bool check_water_avl(void);
bool check_water_avl1(void);
void send_feedback(String);
void check_event(String);
void onReceive(int);

void setup() {
  Serial.begin(9600);
  pinMode(NO_PIN, OUTPUT);
  pinMode(NC_PIN, OUTPUT);
  Serial.println("STARTING-LORA-REC");
  check_lora_connection();
  Serial.println("LORA Connected");
  digitalWrite(NC_PIN, LOW);
  digitalWrite(NO_PIN, LOW);
}

void loop()
{
  //time_now_NC = millis();
  //time_now_NC = millis();
onReceive(LoRa.parsePacket());
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
  if (!LoRa.begin(433E6))
  {
   Serial.println("Starting LoRa failed!");
   while (1);
 }
}
void send_feedback(String temp)
{
  delay(500);
  send_data(temp);
  Serial.println("sending feedback:" + temp);
}
void onReceive(int packetSize)
{
  if (packetSize == 0) return;
  String incoming = "";
  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println();
  send_feedback("Msg_received by receiver");
  check_event(incoming);
}

void check_event(String temp_event)
{
  if (temp_event == "ON")
  {
    if(!NO_STATE)
    {
      Start_Motor();
      if(check_water_avl())
      {
        NO_STATE = true;
        NC_STATE = false;
       send_feedback("Motor Started successfully");
       temp_event ="";
       return;
      }
      if(!check_water_avl())
        {
          send_feedback("Motor Started Failled");
          temp_event ="";
          return;
        }
    }
    if(NO_STATE)
    {
      send_feedback("Already On");
    }
  }

  if (temp_event == "OFF")
  {
    if(!NC_STATE)
    {
      Stop_Motor();
      if(!check_water_avl1())
        {
          NC_STATE = true;
          NO_STATE = false;
          send_feedback("Motor Stopped successfully");
          temp_event ="";
          return;
        }
        if(check_water_avl1())
          {
            send_feedback("Motor Stopped Failled");
            temp_event ="";
            return;
          }
      }
    if(NC_STATE)
      {
        send_feedback("Already OFF");
      }
  }
  else
  {
    return;
  }
}

void Start_Motor()
{
digitalWrite(NO_PIN, HIGH);
delay(NO_PERIOD);
digitalWrite(NO_PIN, LOW);
}

void Stop_Motor()
{
digitalWrite(NC_PIN, HIGH);
delay(NC_PERIOD);
digitalWrite(NC_PIN, LOW);
}

bool check_water_avl()
{
return true;
}
bool check_water_avl1()
{
return false;
}
void reset_arduino()
{
  resetFunc();
}
