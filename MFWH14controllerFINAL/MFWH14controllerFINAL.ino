/* MFWH Controller 
   
   Uses XBee Shield + SFE Joystick + NeoPixel Ring.
    
   The XBee trasnmits data from the Joystick.
   
   The NeoPixel Ring takes direct data from the
   Joystick and shows the direction the XBee will
   be transmitting.
       
   Connect:
     Joystick A1 -> A0
              A2 -> A1
     NeoPixel Ring IN -> D4
   
   created 6.16.14
   made by Quin Etnyre
   
   kudos to Atmel, SparkFun, Adafruit, Digi (Rob Faludi)
*/

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
 
#define PIN 4 // NeoPixel Pin #
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, PIN); //x16

uint32_t blue = 0x0000FF; // color for bot 1
uint32_t green = 0x00FF00; // color for bot 2

int robotColor = 2; // 1 is blue bot, 2 is green bot
int color;  // color variable for current color

int joystickA;  // variables for joystick direction and speed
int joystickB;

int joystickPin1 = A0;  // predefined pins
int joystickPin2 = A1;

void setup() {
  Serial.begin(9600);
  
  pixels.begin();
  pixels.setBrightness(180); // 1/3 brightness
}

void loop(){ 
    
  joystickA=analogRead(joystickPin1); // joystick A
  joystickB=analogRead(joystickPin2); // joystick B
  
  int printSpeed = map(joystickA, 0, 1023, 0, 15);
  int printDirection = map(joystickB, 0, 1023, 0, 15);
  
  int writeSpeed = map(joystickA, 0, 1023, 0, 9);
  int writeDirection = map(joystickB, 0, 1023, 0, 9);
  
  if(robotColor == 1) {
    color = blue;
  }
  
  if(robotColor == 2) {
    color = green;
  } 
  
  uint8_t i;
  
  i = random(32);
  pixels.setPixelColor(i, color);
  pixels.show();
  pixels.setPixelColor(i, 0);


  Serial.write(0x7E);
  Serial.print(writeSpeed);
  Serial.print(writeDirection);
/*
  Serial.print("A");
  Serial.print(writeSpeed);
  Serial.print(writeDirection);
  Serial.println("C");
*/
}
