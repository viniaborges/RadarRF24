#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define _RF_CE    9
#define _RF_CS   10
const uint64_t _RFpipe=0xE8E8F0F0E1LL;

RF24 RFlink(_RF_CE,_RF_CS);

void setup(){
  Serial.begin(115200);
  RFlink.begin();
  RFlink.openReadingPipe(1,_RFpipe);
  RFlink.startListening();
}

void loop(){
  if(RFlink.available()){
    int _toRead[3];
    boolean _Read=false;
    while(!_Read) _Read=RFlink.read(_toRead,sizeof(_toRead));
    for(byte i=0;i<3;i++) Serial.write(_toRead[i]);
  }
}
