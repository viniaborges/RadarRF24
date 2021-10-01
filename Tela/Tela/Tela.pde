import processing.serial.*;

Serial radarSerial;
int screenSize=650;
int range=500;
int steps=85;
PFont screenFont;
int[] position=new int[steps];
int positionNow;

void setup(){
  size(screenSize,screenSize);
  background(0);
  screenFont=createFont("verdana",12);
  textFont(screenFont);
  radarSerial=new Serial(this,Serial.list()[0],115200);
  radarSerial.clear();
}

void draw(){
  if(radarSerial.available()>0){
    char[] _serialBuffer=new char[3];
    _serialBuffer[0]=radarSerial.readChar();
    _serialBuffer[1]=radarSerial.readChar();
    _serialBuffer[2]=radarSerial.readChar();
    radarSerial.clear();
    positionNow=_serialBuffer[0];
    position[positionNow]=_serialBuffer[1]*255+_serialBuffer[2];
    screenBG();
    screenWipe();
    screenDots();
  }
}

void screenBG(){
  fill(0);
  rect(0,0,screenSize,screenSize);
  noFill();
  stroke(0,255,0);
  strokeWeight(1);
  ellipse(screenSize/2,screenSize/2,screenSize*0.8,screenSize*0.8);
  ellipse(screenSize/2,screenSize/2,screenSize*0.4,screenSize*0.4);
  stroke(0,80,0);
  ellipse(screenSize/2,screenSize/2,screenSize*0.6,screenSize*0.6);
  ellipse(screenSize/2,screenSize/2,screenSize*0.2,screenSize*0.2);
  stroke(0,35,0);
  strokeWeight(1);
  ellipse(screenSize/2,screenSize/2,screenSize*0.72,screenSize*0.72);
  ellipse(screenSize/2,screenSize/2,screenSize*0.64,screenSize*0.64);
  ellipse(screenSize/2,screenSize/2,screenSize*0.56,screenSize*0.56);
  ellipse(screenSize/2,screenSize/2,screenSize*0.48,screenSize*0.48);
  ellipse(screenSize/2,screenSize/2,screenSize*0.32,screenSize*0.32);
  ellipse(screenSize/2,screenSize/2,screenSize*0.24,screenSize*0.24);
  ellipse(screenSize/2,screenSize/2,screenSize*0.16,screenSize*0.16);
  ellipse(screenSize/2,screenSize/2,screenSize*0.08,screenSize*0.08);
  stroke(0,255,0);
  strokeWeight(2);
  line(0,screenSize/2,screenSize,screenSize/2);
  line(screenSize/2,0,screenSize/2,screenSize);
  fill(0,255,0);
  text(int(range*0.005),screenSize*0.5-10,screenSize*0.3-5);
  text(int(range*0.01),screenSize*0.5-18,screenSize*0.1-5);
  text(int(range*0.005),screenSize*0.3-10,screenSize*0.5-5);
  text(int(range*0.01),screenSize*0.1-18,screenSize*0.5-5);
  text(int(range*0.005),screenSize*0.5-10,screenSize*0.7-5);
  text(int(range*0.01),screenSize*0.5-18,screenSize*0.9-5);
  text(int(range*0.005),screenSize*0.7-10,screenSize*0.5-5);
  text(int(range*0.01),screenSize*0.9-18,screenSize*0.5-5);
  text("Alcance: "+range+"cm",screenSize-120,20);
  text(int(range*0.01)+"m",screenSize-64,35);
  text("Alunos:",10,screenSize-60);
  text("Fábio Shimomoto (351296)",10,screenSize-48);
  text("Patrick Oliveira (414590)",10,screenSize-36);
  text("Vinicius Borges (351415)",10,screenSize-24);
  text("Referência: "+positionNow,screenSize-100,screenSize-10);
}
 
void screenWipe(){
  strokeWeight(2);
  stroke(255,100,0);
  line(screenSize*0.5,screenSize*0.5,screenSize*0.5+screenSize*0.4*cos(2*(positionNow+1)*PI/steps),screenSize*0.5+screenSize*0.4*sin(2*(positionNow+1)*PI/steps));
}

void screenDots(){
  noStroke();
  for(int i=0;i<steps;i++){
    if(position[i]<range){
      float posX=(position[i]*cos(2*(i)*PI/steps))*(screenSize*0.4/range)+screenSize/2;
      float posY=(position[i]*sin(2*(i)*PI/steps))*(screenSize*0.4/range)+screenSize/2;
      fill(255,int((255/(range*1.0))*position[i]),0);
      ellipse(posX,posY,5,5);
    }
  }
}
