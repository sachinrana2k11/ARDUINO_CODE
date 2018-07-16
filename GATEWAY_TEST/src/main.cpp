#include <Arduino.h>
#include <EDB.h>
#include <String.h>
#include <EEPROM.h>
#define TABLE_SIZE 512
#define RECORDS_TO_CREATE 10
int recno_read,recno_write;
long lastSendTime=0;
int  interval=500;
long lastSendTime1=0;
int  interval1=1000;
bool state = false;
struct LogEvent {
  int id;
  int temperature;
  String name;
}
logEvent;
void writer(unsigned long address, byte data)
{
  EEPROM.write(address, data);
}

byte reader(unsigned long address)
{
  return EEPROM.read(address);
}
EDB db(&writer, &reader);

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  Serial.println("Extended Database Library + Arduino Internal EEPROM Demo");
  Serial.println();
  db.create(0, TABLE_SIZE, sizeof(logEvent));
  Serial.print("Record Count: "); Serial.println(db.count());
  Serial.println("Creating Records...");



}

void loop()
{
  if (millis() - lastSendTime > interval)
  {
      logEvent.id = recno_write;
      logEvent.temperature = recno_write * 2;
      logEvent.name = "sachin rana";
      db.appendRec(EDB_REC logEvent);
      recno_write++;
      lastSendTime = millis();
      digitalWrite(13,state);
      state = !state;

 }
 if (millis() - lastSendTime1 > interval1)
 {
     db.readRec(recno_read, EDB_REC logEvent);
     Serial.print("ID: "); Serial.println(logEvent.id);
     Serial.print("Temp: "); Serial.println(logEvent.temperature);
     Serial.print("name: "); Serial.println(logEvent.name);
     recno_read++;
     lastSendTime1 = millis();
}
}
