/* MFWH Robot

  Uses FuzzBot + mount with XBee and LED's, received by the Neo-
  Pixel controller board.
  
  Connect:
    RFID Shield (hacked) by SFE -> Arduino
                        RFID RX -> D11
                        RFID TX -> D4
    NeoPixel Stick (LED x8)  IN -> D5
    XBee Explorer            RX -> D1
                             TX -> D0
    
  
  created 6.16.14
  made by Quin Etnyre
  
  kudos to Atmel, SparkFun, Digi (Rob Faludi)

*/

#include <ZumoMotors.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

SoftwareSerial rfid(11, 4);

ZumoMotors motors;

#define PIN 5 // NeoPixel Pin #
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, PIN); //x16

uint32_t blue = 0x0000FF;  // all colors used with NeoPixels
uint32_t green = 0x00FF00;
uint32_t white = 0xFFFFFF;
uint32_t purple = 0x800080;
uint32_t red = 0xFF0000;

//functions for RFID
void check_for_notag(void);
void halt(void);
void parse(void);
void print_serial(void);
void read_serial(void);
void seek(void);
void set_flag(void);

//Global var
int flag = 0;
int Str1[11];

int first;    // variables for mifare data
int second;
int third;
int fourth;

int last1;    // variables for old mifare data
int last2;
int last3;
int last4;

int last5;  // variable for old serial incoming data
int last6;
int last7;
int last8;
int last9;
int last10;

int potVal;  // serial incoming data variables
int turnVal;

int time = 10;  // delay time inbetween when motors are turned
           // on, vs. when we start the loop again

int speedVal; // new speed value to multiply to the root #

int robotColor = 2; // 1 is blue bot, 2 is green bot

int cycle = 1;

int runningScore = -1;  // total number of points by player

int executeMotors = 1;

int lastTurnVal;  //last turn values, make sure it doesn't have
int thirdVal;     //too hard so the motors don't burn out
int fourthVal;
int i=0;
boolean firstOK=false;
boolean backOK=false;
int markFront;
int markBack;
char raw[10];
String in;

void setup() {
    
  Serial.begin(9600);
  
  pixels.begin();
  pixels.setBrightness(90); // max brightness 180, don't go farther!
  
  if(robotColor == 1) {
    pixels.setPixelColor(0, blue);
    pixels.setPixelColor(1, blue);
    pixels.setPixelColor(2, blue);
    pixels.setPixelColor(3, blue);
    pixels.setPixelColor(4, blue);
    pixels.setPixelColor(5, blue);
    pixels.setPixelColor(6, blue);
    pixels.setPixelColor(7, blue);
  }
  
  if(robotColor == 2) {
    pixels.setPixelColor(0, green);
    pixels.setPixelColor(1, green);
    pixels.setPixelColor(2, green);
    pixels.setPixelColor(3, green);
    pixels.setPixelColor(4, green);
    pixels.setPixelColor(5, green);
    pixels.setPixelColor(6, green);
    pixels.setPixelColor(7, green);
  }
  
  pixels.show();
  
  rfid.begin(19200);
  delay(100);
  halt();
}

void loop() {
  
  while(Serial.available() > 0) {
   /*
   int k=0; 
  // while(Serial.available()>0){   
    int c=Serial.read();
     
     if(c=='C' && backOK){     
       in+=c;
       markBack=c;
       if(markFront-markBack==2){
         potVal=in.substring(markFront+1,markFront+2).toInt();
         turnVal=in.substring(markFront+2,markFront+3).toInt();
         executeMotors=true;
         Serial.print(in);
         Serial.print(potVal);
         Serial.println(turnVal);
       }else{
         executeMotors=false;
       }
       in="";
     }  
   
     if(c=='A' && !backOK){
      
       backOK=true;
       in+=c;
       markFront=c;
     } 
     
   }
   */

   
   if(Serial.read() == '~') {
     
     last7 = last9;
     last8 = last10;
     
     last5 = last7;
     last6 = last8;
     
     potVal = last5;
     turnVal = last6;
     
     potVal = Serial.read() - '0';
     turnVal = Serial.read() - '0';
     
   if(potVal == last5) {
     executeMotors == 0;
   }
   
   if(turnVal == last6) {
     executeMotors == 0;
   }
   
   if(last5 == last7) {
     executeMotors == 0;
   }
   
   if(last6 == last8) {
     executeMotors == 0;
   }
   
   if(last7 == last9) {
     executeMotors == 0;
   }
   
   if(last8 == last10) {
     executeMotors == 0;
   }
   
   if(turnVal >> 9 || turnVal << 0) {
     executeMotors == 0;
   }
   
   if(turnVal == '78' || potVal == '78') {
     executeMotors == 0;
   }
   
   if(potVal >> 9 || potVal << 0) {
     executeMotors == 0;
   }
  
  if(executeMotors == 1) {
    
  switch (potVal) {
    case 9:
      speedVal = -5;
      break;
    case 8:
      speedVal = -4;
      break;
    case 7:
      speedVal = -3;
      break;
    case 6:
      speedVal = -2;
      break;
    case 5:
      speedVal = -1;
      break;
    case 4:
      speedVal = 1;
      break;
    case 3:
      speedVal = 2;
      break;
    case 2:
      speedVal = 3;
      break;
    case 1:
      speedVal = 4;
      break;
    case 0:
      speedVal = 5;
      break;
  }
  
  Serial.println(speedVal);
  //Serial.print(turnVal);
  //Serial.println(potVal);

  switch (turnVal) {
    case 0:

      motors.setLeftSpeed(16*speedVal);
      motors.setRightSpeed(40*speedVal);
      delay(time);
      break;
    case 1:

      motors.setLeftSpeed(22*speedVal);
      motors.setRightSpeed(40*speedVal);
      delay(time);
      break;
    case 2:

      motors.setLeftSpeed(28*speedVal);
      motors.setRightSpeed(40*speedVal);
      delay(time);
      break;
    case 3:

      motors.setLeftSpeed(34*speedVal);
      motors.setRightSpeed(40*speedVal);
      delay(time);
      break;
    case 4:

      motors.setLeftSpeed(40*speedVal);
      motors.setRightSpeed(40*speedVal);
      delay(time);
      break;
    case 5:

      motors.setLeftSpeed(40*speedVal);
      motors.setRightSpeed(34*speedVal);
      delay(time);
      break;
    case 6:

      motors.setLeftSpeed(40*speedVal);
      motors.setRightSpeed(28*speedVal);
      delay(time);
      break;
    case 7:

      motors.setLeftSpeed(40*speedVal);
      motors.setRightSpeed(22*speedVal);
      delay(time);
      break;
    case 8:

      motors.setLeftSpeed(40*speedVal);
      motors.setRightSpeed(16*speedVal);
      delay(time);
      break;
    
    case 9:
      
      motors.setLeftSpeed(40*speedVal);
      motors.setRightSpeed(10*speedVal);
      delay(time);
      break;
      
        } // end of switch(case)
      } // end of executeMotors
    }  // end of if Serial(read);
    read_serial();
  }  // end of Serial(available)
}  // end of loop()
 

//start of RFID functions

void check_for_notag()
{
  seek();
  delay(10);
  parse();
  set_flag();

  if(flag = 1){
    seek();
    delay(10);
    parse();
  }
}

void halt()
{
 //Halt tag
  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)147);
  rfid.write((uint8_t)148);
}

void parse()
{
  while(rfid.available()){
    if(rfid.read() == 255){
      for(int i=1;i<11;i++){
        Str1[i]= rfid.read();
      }
    }
  }
}

void print_serial()
{
  if(flag == 1){
    
    last1 = first;
    last2 = second;
    last3 = third;
    last4 = fourth;
    
    first = Str1[8];
    second = Str1[7];
    third = Str1[6];
    fourth = Str1[5];
    
    if(first == last1 && second == last2 && third == last3 && fourth == last4) {
      runningScore = runningScore; // no points awarded, if it's the same tag
    } else {  // if it's a different tag, add one to their point total
      runningScore = runningScore + 1;
    }
    
    if(runningScore <= 7) {
      pixels.setPixelColor(runningScore, white);
    } else if (runningScore >= 8 && runningScore <= 15) {
      pixels.setPixelColor(runningScore-8, purple);
    } else if (runningScore >= 16 && runningScore <= 23) {
      pixels.setPixelColor(runningScore-16, red);
    }
    
    pixels.show();
  }
}

void read_serial()
{
  seek();
  delay(1);
  parse();
  set_flag();
  print_serial();
}

void seek()
{
  //search for RFID tag
  rfid.write((uint8_t)255);
  rfid.write((uint8_t)0);
  rfid.write((uint8_t)1);
  rfid.write((uint8_t)130);
  rfid.write((uint8_t)131);
}

void set_flag()
{
  if(Str1[2] == 6){
    flag++;
  }
  if(Str1[2] == 2){
    flag = 0;
  }
}
