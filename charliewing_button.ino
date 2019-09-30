#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_IS31FL3731.h>

// If you're using the full breakout...
// Adafruit_IS31FL3731 ledmatrix = Adafruit_IS31FL3731();
// If you're using the FeatherWing version
Adafruit_IS31FL3731_Wing ledmatrix = Adafruit_IS31FL3731_Wing();

#define BUTTON1 10  //Input Button

uint8_t        brightness = 128; //Brightness can be from 0 to 255

//Defined below are the patterns to display on the matrix
//They are either an arrow pointing up, down, left or right
static const uint8_t PROGMEM
  left_bmp[] =
  { B00011000,
    B00111100,
    B01111110,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000 },
  right_bmp[] =
  { B00011000,
    B00011000,
    B00011000,
    B00011000,
    B01111110,
    B00111100,
    B00011000,
    B00011000 },
  down_bmp[] =
  { B00000000,
    B00100000,
    B01100000,
    B11111111,
    B11111111,
    B01100000,
    B00100000,
    B00000000 },
  up_bmp[] =
  { B00000000,
    B00000100,
    B00000110,
    B11111111,
    B11111111,
    B00000110,
    B00000100,
    B00000000 };

int lastButtonState1 = LOW;
int buttonState1;
int pattern = -1;
bool patternChange=false;

//For debouncing buttons
unsigned long lastDebounceTime1=0;
unsigned long debounceDelay = 200;

const uint8_t numPatterns = 5;

void setup() {
  Serial.begin(9600);
  Serial.println("Button-controlled charliewing");

  if (! ledmatrix.begin()) {
    Serial.println("LED matrix not found");
    while (1);
  }
  Serial.println("LED matrix found!");
  pinMode(BUTTON1, INPUT); //the variable BUTTON1 is set above. this makes that input on the Feather board an input
}

void loop() {

  //read state of the button pin
  int reading1 = digitalRead(BUTTON1);
  //Serial.println(reading1); //this can be used for debugging if something isn't right
  //check to see if change in button is real (or noise)
  if (reading1 != lastButtonState1){
    lastDebounceTime1 = millis();
  }
  if ((millis()-lastDebounceTime1)>debounceDelay){
    if (reading1!=buttonState1){
      buttonState1=reading1;
      if (buttonState1==HIGH) {
        
        pattern = pattern+1;
        patternChange=true;
        Serial.println(pattern);
        if (pattern>numPatterns){
          pattern=0;
                
        }
      }
    }
  }
  lastButtonState1=reading1;
  
  if (patternChange) {
    if (pattern==0) {
      //Serial.println("Pattern1");
      ledmatrix.clear(); //clears the screen
      ledmatrix.drawPixel(7,2,brightness); //sets an individual pixel to the set brightness
      ledmatrix.drawPixel(8,2,brightness);
      ledmatrix.drawPixel(7,3,brightness);
      ledmatrix.drawPixel(8,3,brightness);
      ledmatrix.drawPixel(7,4,brightness);
      ledmatrix.drawPixel(8,4,brightness);
    }
    if (pattern==1) {
      ledmatrix.clear();
      ledmatrix.drawBitmap(2,0,up_bmp,8,8,brightness); //draws a pattern defined in 'up_bmp' starting at (2,0) and having size (8,8)
      //delay(1500); 
      }
    if (pattern==2) {
      ledmatrix.clear();
      ledmatrix.drawBitmap(2,0,down_bmp,8,8,brightness);
      //delay(1500); 
      }
    if (pattern==3) {
      ledmatrix.clear();
      ledmatrix.drawBitmap(2,0,right_bmp,8,8,brightness);
      //delay(1500); 
      }
    if (pattern==4) {
      ledmatrix.clear();
      ledmatrix.drawBitmap(2,0,left_bmp,8,8,brightness);
      //delay(1500); 
      }
    if (pattern==numPatterns) {
      //clears the matrix 
      ledmatrix.clear();
      //delay(1500); 
      }
  }
  
}
