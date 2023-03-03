#include <SPI.h>
#include <LoRa.h>

#define ss 21
#define rst 17
#define dio0 16

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);
  
  while (!LoRa.begin(433E6)) 
  {
    Serial.println(".");
    delay(500);
  }
  
  LoRa.setSyncWord(0x34);
  Serial.println("LoRa Initializing OK!");
}

void loop() 
{

  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {

    Serial.print("Received packet '");

    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  } 
}