#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"
//#include "esp_sleep.h"


/******************** BLE ************************************/
BLEAdvertising *pAdvertising;
String product_url = "bit.ly/2RTeYtv";

#define DEBUG
/*****************************************************************************
Initialize Bluetooth Low energy for Eddystone
*****************************************************************************/
void setBeacon() {
  char beacon_data[36];
  uint16_t beaconUUID = 0xFEAA;   // UUID for Eddystone Service
  int url_length;
  int count;

  
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x06);    // GENERAL_DISK_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  oAdvertisementData.setCompleteServices(BLEUUID(beaconUUID));

  url_length = product_url.length();

  beacon_data[0] = 0x10;      // Frame Type - URL
  beacon_data[1] = 0x00;      // Tx power 4dBm?
  beacon_data[2] = 0x03;      // URL Scheme Prefix - https://
  for(count=0; count<url_length; count++) {
    beacon_data[3+count] = product_url.charAt(count);
  }
  //beacon_data[14+count] = 0xFF;

#ifdef DEBUG
  Serial.print("Beacon Data: ");
  for(count=0; count<url_length+3; count++) {
    if(beacon_data[count] < 16){
      Serial.print('0');
    }
    Serial.print(beacon_data[count], HEX);
    Serial.print(' ');
  }
  Serial.println();
#endif

  oAdvertisementData.setServiceData(BLEUUID(beaconUUID), std::string(beacon_data, url_length+3));
#ifdef DEBUG
  Serial.println("Service Data set!");
#endif

  pAdvertising->setScanResponseData(oAdvertisementData);
#ifdef DEBUG
  Serial.println("Scan response set!");
#endif  
}

/*****************************************************************************
Initialize Bluetooth Low energy for Eddystone
*****************************************************************************/

void setup() {
  
  // Create BLE device
  BLEDevice::init("UniShelf");

  // Create BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  
  pAdvertising = pServer->getAdvertising();

  setBeacon();

  // Start advertising
  pAdvertising->start();
#ifdef DEBUG
  Serial.println("Advertising started...");
#endif

  delay(5000);
}


/*****************************************************************************
the loop function runs over and over again forever
*****************************************************************************/
void loop() {

}
