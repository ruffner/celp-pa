// PANode
// Wireless audio transmitter and reciever program


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <RH_RF95.h>

#include "AudioInputSpeex.h"


// GUItool: begin automatically generated code
AudioInputSpeex          speex1;
AudioInputAnalog         adc1;           //xy=394,277
AudioAnalyzeFFT256       fft256_1;       //xy=631,294
AudioConnection          patchCord1(speex1, fft256_1);
// GUItool: end automatically generated code

RH_RF95 rf95(10,15);
#define RF95_RST 15
#define RF95_FREQ 915.0


#define LED 13

#define NODE_TX 1
#define NODE_RX !NODE_TX



void setup() {
  // put your setup code here, to run once:
  AudioMemory(10);
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(RF95_RST, OUTPUT);


  // reset radio
  
  // manual reset
  digitalWrite(RF95_RST, LOW);
  delay(10);
  digitalWrite(RF95_RST, HIGH);
  delay(10);

   while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
}

void loop() {
  // put your main code here, to run repeatedly:
//  for( int i=0; i<127; i++ ){
//    Serial.print(fft256_1.read(i));
//    if( i==126 ) Serial.println();
//    else Serial.print(", ");
//  }


   if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      Serial.println((char*)buf);
       Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
