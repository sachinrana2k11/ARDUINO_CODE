#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

//Time used to wait for an interval before resending BLE infos
unsigned long timeBLE= 0;

//core on which the BLE detection task will run
static int taskCore = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
      void onResult(BLEAdvertisedDevice advertisedDevice) {
        String mac_adress = advertisedDevice.getAddress().toString().c_str();
        mac_adress.replace(":","");
        mac_adress.toUpperCase();
        //String mactopic = subjectBTtoMQTT + mac_adress;
        if (advertisedDevice.haveName()){
            trc(F("Get Name "));
            String nameBLE = advertisedDevice.getName().c_str();
            trc(nameBLE);
            //client.publish((char *)(mactopic + "/name").c_str(),(char *)nameBLE.c_str());
        }
        if (advertisedDevice.haveManufacturerData()){
            trc(F("Get ManufacturerData "));
            String ManufacturerData = advertisedDevice.getManufacturerData().c_str();
            trc(ManufacturerData);
            //client.publish((char *)(mactopic + "/ManufacturerData").c_str(),(char *)ManufacturerData.c_str());
        }
        if (advertisedDevice.haveRSSI()){
          trc(F("Get RSSI "));
          String rssi = String(advertisedDevice.getRSSI());
          String rssitopic = mactopic + subjectBTtoMQTTrssi;
          trc(rssitopic + " " + rssi);
          //client.publish((char *)rssitopic.c_str(),(char *)rssi.c_str());
        }
        if (advertisedDevice.haveTXPower()){
          trc(F("Get TXPower "));
          int8_t TXPower = advertisedDevice.getTXPower();
          trc(TXPower);
          char cTXPower[5];
          sprintf(cTXPower, "%d", TXPower);
          //client.publish((char *)(mactopic + "/tx").c_str(),cTXPower);
        }
        if (advertisedDevice.haveServiceData()){
            trc(F("Get service data "));
            std::string serviceData = advertisedDevice.getServiceData();
            int serviceDataLength = serviceData.length();
            String returnedString = "";
            for (int i=0; i<serviceDataLength; i++)
            {
              int a = serviceData[i];
              if (a < 16) {
                returnedString = returnedString + "0";
              }
              returnedString = returnedString + String(a,HEX);
            }
            trc(returnedString);

            trc(F("Get service data UUID"));
            BLEUUID serviceDataUUID = advertisedDevice.getServiceDataUUID();
            trc(serviceDataUUID.toString().c_str());

            if (strstr(serviceDataUUID.toString().c_str(),"fe95") != NULL){
              trc("Processing BLE device data");
              char service_data[returnedString.length()+1];
              returnedString.toCharArray(service_data,returnedString.length()+1);
              service_data[returnedString.length()] = '\0';
              char mac[mac_adress.length()+1];
              mac_adress.toCharArray(mac,mac_adress.length()+1);
              if (strstr(service_data,"209800") != NULL) {
                trc("mi flora data reading");
                boolean result = process_data(-22,service_data,mac);
              }
              if (strstr(service_data,"20aa01") != NULL){
                trc("mi jia data reading");
                boolean result = process_data(-24,service_data,mac);
              }
            }
        }
      }
  };

void setupBT(){
    #ifdef multiCore
    // we setup a task with priority one to avoid conflict with other gateways
    xTaskCreatePinnedToCore(
                      coreTask,   /* Function to implement the task */
                      "coreTask", /* Name of the task */
                      10000,      /* Stack size in words */
                      NULL,       /* Task input parameter */
                      1,          /* Priority of the task */
                      NULL,       /* Task handle. */
                      taskCore);  /* Core where the task should run */
      trc(F("ZgatewayBT multicore ESP32 setup done "));
    #else
      trc(F("ZgatewayBT singlecore ESP32 setup done "));
    #endif
}
void coreTask( void * pvParameters ){

    String taskMessage = "BT Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();

    while(true){
        trc(taskMessage);
        delay(TimeBtw_Read);
        BLEDevice::init("");
        BLEScan* pBLEScan = BLEDevice::getScan(); //create new scan
        MyAdvertisedDeviceCallbacks myCallbacks;
        pBLEScan->setAdvertisedDeviceCallbacks(&myCallbacks);
        pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
        BLEScanResults foundDevices = pBLEScan->start(Scan_duration);
    }
}

}
