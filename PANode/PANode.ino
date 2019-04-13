// PANode
// Wireless audio transmitter and reciever program


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=394,277
AudioAnalyzeFFT1024      fft1024_1;      //xy=405,624
AudioAnalyzeFFT256       fft256_1;       //xy=631,294
AudioConnection          patchCord1(adc1, fft256_1);
// GUItool: end automatically generated code


#define NODE_TX 1
#define NODE_RX !NODE_TX



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
