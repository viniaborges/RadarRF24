#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define R_trigger         A1
#define R_echo            A0
#define Led                4
#define sensor             3
#define touch             A2
#define touchSamples     500
#define touchRange      0.01
#define IR_send            2
#define IR_recv           A3
#define IR_Target        100
#define RF_CE              9
#define RF_CS             10
const uint64_t RF_pipe=0xE8E8F0F0E1LL;
const byte stepperPin[2]={7,8};

RF24 RF_link(RF_CE,RF_CS);

long R_measure;
byte stepperPos=1;
int stepPosition=0;
boolean stepDirection=true;

void setup(){
  //for debugging
  Serial.begin(115200);
  //setup radar
  pinMode(R_trigger,OUTPUT);
  pinMode(R_echo,INPUT);
  digitalWrite(R_trigger,LOW);
  //setup stepper
  pinMode(stepperPin[0],OUTPUT);
  pinMode(stepperPin[1],OUTPUT);
  digitalWrite(stepperPin[0],LOW);
  digitalWrite(stepperPin[1],LOW);
  //setup IR
  pinMode(IR_send,OUTPUT);
  pinMode(IR_recv,INPUT_PULLUP);
  digitalWrite(IR_send,LOW);
  //setup RF
  RF_link.begin();
  RF_link.openWritingPipe(RF_pipe);
  //setup leds
  pinMode(Led,OUTPUT);
  //waiting for start
  waitStart(5);
}

void loop(){
  radarGet();
  RF_Transmit();
  stepperTurn(stepDirection,5);
  stepPosition++;
  boolean zeroPos=IR_test();
  if(zeroPos){
    stepPosition=0;
//    stepDirection=!stepDirection;
//    if(!stepDirection) stepPosition=0;
  }
  delay(50);
}

void radarGet(){
  digitalWrite(R_trigger,HIGH);
  delayMicroseconds(2);
  digitalWrite(R_trigger,LOW);
  R_measure=pulseIn(R_echo,HIGH,100000)/70;
}

void stepperTurn(boolean _way,int _steps){
  for(int i=0;i<_steps;i++){
    if(stepperPos%2==0) digitalWrite(stepperPin[_way],!digitalRead(stepperPin[_way]));
    else digitalWrite(stepperPin[!_way],!digitalRead(stepperPin[!_way]));
    stepperPos+=_way*2-1;
    if(stepperPos==5) stepperPos=1;
    if(stepperPos==0) stepperPos=5;
    delay(30);
  }
}

boolean IR_test(){
  digitalWrite(IR_send,HIGH);
  delay(1);
  int _IRval=analogRead(IR_recv);
  digitalWrite(IR_send,LOW);
  if(_IRval<IR_Target) return true;
  else return false;
}

void RF_Transmit(){
  int _toSend[3];
  _toSend[0]=stepPosition;
  _toSend[1]=R_measure/255;
  _toSend[2]=R_measure%255;
  RF_link.write(_toSend,sizeof(_toSend));
}

void touchStart(){
  boolean _wait=true;
  byte _calibrationTest=0;
  long _defaultValue;
  while(_wait){
    long _value=0;
    for(int _touchCounter=0;_touchCounter<touchSamples;_touchCounter++){
      pinMode(touch,INPUT_PULLUP);
      pinMode(touch,INPUT);
      _value+=analogRead(touch);
    }
    if(_calibrationTest<3){
      _defaultValue=_value;
      _calibrationTest++;
    }
    if(abs(_value-_defaultValue)>touchRange*_defaultValue) _wait=false;
    digitalWrite(Led,!digitalRead(Led));
    Serial.println(_value); //for debugging
    delay(200);
  }
  digitalWrite(Led,LOW);
}

void waitStart(int _times){
  for(int i=0;i<_times;i++){
    digitalWrite(Led,!digitalRead(Led));
    delay(200);
  }
  digitalWrite(Led,LOW);
}

void RFtest(){
  long variable=random(5000);
  int _toSend[3];
  _toSend[0]=stepPosition;
  _toSend[1]=variable/255;
  _toSend[2]=variable%255;
  RF_link.write(_toSend,sizeof(_toSend));
}
