#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "BLEBeacon.h"
#include <String.h>
BLEScan* pBLEScan;
int scanTime = 5; //In seconds
uint16_t beconUUID = 0xFEAA;
#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00)>>8) + (((x)&0xFF)<<8))
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("\n\n");
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      std::string strServiceData = advertisedDevice.getServiceData();
       uint8_t cServiceData[100];
       strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);
        if (advertisedDevice.haveManufacturerData()==true) {
          std::string strManufacturerData = advertisedDevice.getManufacturerData();

          uint8_t cManufacturerData[100];
          strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

          if (strManufacturerData.length()==25 && cManufacturerData[0] == 0x4C  && cManufacturerData[1] == 0x00 ) {
            BLEBeacon oBeacon = BLEBeacon();
            oBeacon.setData(strManufacturerData);
            Serial.printf("iBeacon Frame\n");
            Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n",oBeacon.getManufacturerId(),ENDIAN_CHANGE_U16(oBeacon.getMajor()),ENDIAN_CHANGE_U16(oBeacon.getMinor()),oBeacon.getProximityUUID().toString().c_str(),oBeacon.getSignalPower());
          }
         }

    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
}
void loop()
{
  BLEScanResults foundDevices = pBLEScan->start(scanTime);

}
