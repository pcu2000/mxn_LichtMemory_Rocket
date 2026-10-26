#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PCA9685.h"
//#include "Adafruit_PWMServoDriver.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define MEMORYMODULES 4

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     2 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#define IN_BUTTON 1
#define IN_SWITCH_UP 9
#define IN_SWITCH_LEFT 8
#define IN_SWITCH_DOWN 7
#define IN_SWITCH_RIGHT 44
#define IN_SWITCH_PUSH 43
#define IN_SENSOR D2
#define OUT_OE 4
//#define 

  uint16_t calcLedValue(int ledNumber, int rgb, int sensorValue);
  //uint8_t checkSensor();
  void drawSensorData();
  bool sensorArray[4]{0, 0, 0, 0};
  bool ledArray[4][3] {{1,0,0},{0,1,0},{0,0,1},{1,1,0}};

//Adafruit_PWMServoDriver pwmM1 = Adafruit_PWMServoDriver(0x40);
//Adafruit_PWMServoDriver pwmM2 = Adafruit_PWMServoDriver(0x41);

//Adafruit_PWMServoDriver pwmM3 = Adafruit_PWMServoDriver(0x42);
//Adafruit_PWMServoDriver pwmM4 = Adafruit_PWMServoDriver(0x43);
//Adafruit_PWMServoDriver pwmAll = Adafruit_PWMServoDriver(0xE0);

PCA9685 PCA[] = {
  PCA9685(0x42), //Pannel 1
  PCA9685(0x43),
  PCA9685(0x40),
  PCA9685(0x41)
};

void setLedPWMOn(int module, int led, int ledBrightness);
void setSensorPWMOn(int module, int led);
void setPWMOff(int module, int led);
void setAllRgbLedOff();

void setLedsOn(int zeile);
void setLedsOff(int zeile);
void checkSensor();

void checkColorMatch();
void ledAnzahlError(int Ball1, int Ball2, int Ball3);

void randomNumber();


/*int Data[16][6]= {  //PCB, LED, RandomNumber, Sensorvalue
        {0,  0, 0, 0, 0, 0},
        {0,  4, 0, 0, 0, 0},
        {0,  8, 0, 0, 0, 0},
        {0, 12, 0, 0, 0, 0},
        {1,  0, 0, 0, 0, 0},
        {1,  4, 0, 0, 0, 0},
        {1,  8, 0, 0, 0, 0},
        {1, 12, 0, 0, 0, 0},
        {2,  0, 0, 0, 0, 0},
        {2,  4, 0, 0, 0, 0},
        {2,  8, 0, 0, 0, 0},
        {2, 12, 0, 0, 0, 0},
        {3,  0, 0, 0, 0, 0},
        {3,  4, 0, 0, 0, 0},
        {3,  8, 0, 0, 0, 0},
        {3, 12, 0, 0, 0, 0}
};*/

int Data[16][6]= {  //PCB, LED, RandomNumber, Sensorvalue, Player
        {0,  0, 0, 0, 0, 0},
        {0,  4, 0, 0, 0, 0},
        {1,  0, 0, 0, 0, 0},
        {1,  4, 0, 0, 0, 0},
        {0,  8, 0, 0, 0, 0},
        {0, 12, 0, 0, 0, 0},
        {1,  8, 0, 0, 0, 0},
        {1, 12, 0, 0, 0, 0},
        {2,  0, 0, 0, 0, 0},
        {2,  4, 0, 0, 0, 0},
        {3,  0, 0, 0, 0, 0},
        {3,  4, 0, 0, 0, 0},
        {2,  8, 0, 0, 0, 0},
        {2, 12, 0, 0, 0, 0},
        {3,  8, 0, 0, 0, 0},
        {3, 12, 0, 0, 0, 0}
};
//int rand(100);

int DataColor[8][3] = {
        {4000, 4000,    0},    // Gelb
        {4000,    0, 4000},    // Pink / Magenta
        {   0, 4000, 4000},    // Cyan
        {4000, 2000,    0},    // Orange
        {2000,    0, 4000},    // Violett
        {2000, 4000, 1000},    // Lime
        {4000, 4000, 4000},    // Weiß
        {1000, 3000, 4000}     // Türkis / Himmelblau
};

int DataColorState[10][16] = {
        {6, 1, 4, 7, 2, 0, 5, 3, 1, 6, 4, 2, 7, 0, 5, 3},
        {0, 5, 2, 6, 1, 7, 4, 3, 5, 2, 0, 6, 3, 1, 7, 4},
        {7, 3, 1, 5, 6, 2, 0, 4, 2, 5, 7, 1, 0, 6, 3, 4},
        {4, 0, 6, 2, 3, 5, 1, 7, 6, 4, 2, 0, 5, 7, 1, 3},
        {1, 7, 5, 3, 0, 4, 6, 2, 7, 1, 3, 5, 2, 6, 4, 0},
        {2, 4, 0, 1, 7, 3, 5, 6, 0, 2, 4, 7, 1, 5, 3, 6},
        {5, 6, 3, 4, 2, 1, 7, 0, 3, 5, 6, 4, 0, 2, 1, 7},
        {3, 2, 7, 0, 4, 6, 1, 5, 4, 0, 2, 3, 7, 1, 6, 5},
        {0, 1, 6, 7, 5, 2, 3, 4, 1, 7, 5, 0, 6, 3, 2, 4},
        {7, 5, 4, 2, 1, 0, 6, 3, 5, 7, 1, 4, 2, 6, 0, 3}
};
      int ballAnzahl = 0;
      int Ball1 = -1;
      int Ball2 = -1;
      int Ball3 = -1;

void setup() {
  pinMode(IN_BUTTON, INPUT);
  pinMode(IN_SWITCH_UP, INPUT);
  pinMode(IN_SWITCH_LEFT, INPUT);
  pinMode(IN_SWITCH_DOWN, INPUT);
  pinMode(IN_SWITCH_RIGHT, INPUT);
  pinMode(IN_SWITCH_PUSH, INPUT);
  pinMode(IN_SENSOR, INPUT);
  pinMode(OUT_OE, OUTPUT);

  digitalWrite(OUT_OE, LOW);

   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.setRotation(2);
  display.display();
  delay(2000); // Pause for 2 seconds


  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(14, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  Wire.begin(D4, D5, 100000);
  for (int i = 0; i < MEMORYMODULES; i++)
      {
      PCA[i].begin();
      //PCA[i].setMode2(0b00011011); //Konfiguration für Gridfinity (LED mit Vorwiderstand auf 5V) (Datasheet NXP S. 16 MODE2)
      //PCA[i].setFrequency(200); //default 200Hz (Datasheet NXP S. 25)
      PCA[i].setFrequency(1000); //default 200Hz (Datasheet NXP S. 25)
      }
  /*pwmM1.begin();
  pwmM2.begin();
  pwmM3.begin();
  pwmM4.begin();
                         
  pwmM1.reset();
  pwmM2.reset();
  pwmM3.reset();
  pwmM4.reset();

  pwmM1.setPWMFreq(1000);
  pwmM2.setPWMFreq(1000);
  pwmM3.setPWMFreq(1000);
  pwmM4.setPWMFreq(1000);*/

  //setAllRgbLedOff();

  randomSeed(analogRead(13));
  randomNumber();

/*
  for (uint8_t pcb = 0; pcb < 4; pcb++){
    for (uint8_t led = 0; led < 16; led++){
        setLedPWMOn(pcb, led, 1000);
        delay(100);
    }
  }
  delay(500);
  for (uint8_t pcb = 0; pcb < 4; pcb++){
    for (uint8_t led = 0; led < 16; led++){
        setLedPWMOn(pcb, led, 0);
        delay(100);
    }
  }*/

  for(uint8_t place = 0; place <16; place++){
      PCA[Data[place][0]].setPWM(Data[place][1], 0, 1000);
      PCA[Data[place][0]].setPWM(Data[place][1]+1, 0, 1000);
      PCA[Data[place][0]].setPWM(Data[place][1]+2, 0, 1000);
      delay(100);
  }

  for(uint8_t place = 0; place <16; place++){
      PCA[Data[place][0]].setPWM(Data[place][1], 0, 0);
      PCA[Data[place][0]].setPWM(Data[place][1]+1, 0, 0);
      PCA[Data[place][0]].setPWM(Data[place][1]+2, 0, 0);
      PCA[Data[place][0]].setPWM(Data[place][1]+3, 0, 0);
      delay(100);
  }

  display.clearDisplay();
  display.dim(false);
  display.drawRect(14, 1, 100, 62, WHITE);
  display.drawRect(16, 3, 57, 57, WHITE);
  display.display();
  delay(200);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(75,4);             // Start at top-left corner
  display.println(F("maxon"));
  display.setCursor(75,20);             // Start at top-left corner
  display.println(F("P1:"));
  display.setCursor(75,40);             // Start at top-left corner
  display.println(F("P2:"));

  delay(5000);
  //Serial.begin(115200);

}

void loop() {

  delay(500);
  checkSensor();
  checkColorMatch();
  drawSensorData();
  delay(500);

}

uint16_t calcLedValue(int ledNumber, int rgb, int sensorValue){
  if(ledArray[ledNumber][rgb] == 1){
    if(sensorValue == 1){
      return 4095;
    }else{
      return 100;
    }
  }
  return 0;
}


void drawSensorData(){
  const int16_t startPosX = 23;
  const int16_t startPosY = 10;
  const int16_t spaceCircle = 14;
  const uint16_t radCircle = 5;
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++) {
      display.drawCircle(startPosX + j*spaceCircle, startPosY + i*spaceCircle, radCircle, 1);
    }
  }
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++) {
      display.drawCircle(startPosX + j*spaceCircle, startPosY + i*spaceCircle, 2, Data[i*4+j][3]);
    }
  }
  /*display.drawCircle(startPosX, startPosY, radCircle, Data[zeile][3]);
  display.drawCircle(startPosX, startPosY + 1*spaceCircle, radCircle, sensorArray[0]);
  display.drawCircle(startPosX, startPosY + 2*spaceCircle, radCircle, sensorArray[0]);
  display.drawCircle(startPosX, startPosY + 3*spaceCircle, radCircle, sensorArray[0]);
  display.drawCircle(startPosX + 1*spaceCircle, startPosY, radCircle, sensorArray[0]);
  display.drawCircle(startPosX + 2*spaceCircle, startPosY, radCircle, sensorArray[0]);
  display.drawCircle(startPosX + 3*spaceCircle, startPosY, radCircle, sensorArray[0]);*/
  display.display();

}
  
  


void setLedPWMOn(int module, int led, int ledBrightness){
  PCA[module].setPWM(led, 0, ledBrightness);
  /*switch (module)
  {
  case 0:
    pwmM1.setPWM(led, 0, ledBrightness);
    break;
  case 1:
    pwmM2.setPWM(led, 0, ledBrightness);
    break;
  case 2:
    pwmM3.setPWM(led, 0, ledBrightness);
    break;
  case 3:
    pwmM4.setPWM(led, 0, ledBrightness);
    break;
  
  default:
    break;
  }*/
}

void setSensorPWMOn(int module, int led){
  
  PCA[module].setPWM(led, 0, 4095);
  /*switch (module)
  {
  case 0:
    pwmM1.setPWM(led, 0, 4095);
    break;
  case 1:
    pwmM2.setPWM(led, 0, 4095);
    break;
  case 2:
    pwmM3.setPWM(led, 0, 4095);
    break;
  case 3:
    pwmM4.setPWM(led, 0, 4095);
    break;
  
  default:
    break;
  }*/
}


void setPWMOff(int module, int led){
  PCA[module].setPWM(led, 0, 0);
  /*switch (module)
  {
  case 0:
    pwmM1.setPWM(led, 0, 0);
    break;
  case 1:
    pwmM2.setPWM(led, 0, 0);
    break;
  case 2:
    pwmM3.setPWM(led, 0, 0);
    break;
  case 3:
    pwmM4.setPWM(led, 0, 0);
    break;

  default:
    break;
  }*/
}

void setAllRgbLedOff(){
  for (uint8_t ebene = 0; ebene < 4; ebene++){
    for (uint8_t zeile = 0; zeile < 4; zeile++){
        setLedPWMOn(ebene, (4*zeile), 0);
        setLedPWMOn(ebene, (4*zeile)+1, 0);
        setLedPWMOn(ebene, (4*zeile)+2, 0);
    }
  }
}

/*void setLedsOn(){
      for (uint8_t ebene = 0; ebene < 4; ebene++){
    for (uint8_t zeile = 0; zeile < 4; zeile++){
      if (ledData[ebene][zeile][3] == 1)
      {
        setLedPwmOn(ebene + 1, (4*zeile), ledData[ebene][zeile][0]);
        setLedPwmOn(ebene + 1, (4*zeile)+1, ledData[ebene][zeile][1]);
        setLedPwmOn(ebene + 1, (4*zeile)+2, ledData[ebene][zeile][2]);
      }else{
        setLedPwmOff(ebene + 1, (4*zeile), 0);
        setLedPwmOff(ebene + 1, (4*zeile)+1, 0);
        setLedPwmOff(ebene + 1, (4*zeile)+2, 0);
      }
      
    }
    }
}*/
void checkColorMatch(){
      for (uint8_t zeile = 0; zeile < 16; zeile++){
      if (Data[zeile][3] == 1 && Data[zeile][4] == 0)
      {
        ballAnzahl ++;


      switch (ballAnzahl)
        {
        case 1:
        Data[zeile][5] = 1;
        Data[zeile][4] = 1;
        Ball1 = zeile;
        setLedsOn(Ball1);
          break;

        case 2:
        Data[zeile][5] = 1;
        Data[zeile][4] = 1;
        Ball2 = zeile;
        setLedsOn(Ball2);
        break;

        default:
        Ball3 = zeile;
        ledAnzahlError(Ball1,Ball2,Ball3);
        return;
          break;
        }
      }else if(Data[zeile][3] == 0 && Data[zeile][4] == 1){
        ballAnzahl --;
        Data[zeile][5] = 0;
        Data[zeile][4] = 0;
        setLedsOff(zeile);
      }
  }
   /*if(Data[Ball2][4] == 1){
        setLedsOn(Ball2);
      if(Data[Ball1][2] != Data[Ball2][2]){
        Data[Ball1][4] = 0;
        Data[Ball2][4] = 0;
        setLedsOff(Ball1);
        setLedsOff(Ball2);
        ballAnzahl = 0;
        Ball1 = 0;
        Ball2 = 0;
        Ball3 = 0;
      }else{
        ballAnzahl = 0;
      }
    }*/
}

void ledAnzahlError(int Ball1, int Ball2, int Ball3){
      setLedPWMOn(Data[Ball1][0], Data[Ball1][1], 4095);
      setLedPWMOn(Data[Ball2][0], Data[Ball2][1], 4095);
      setLedPWMOn(Data[Ball3][0], Data[Ball3][1], 4095);
}


void setLedsOn(int zeile){
        setLedPWMOn(Data[zeile][0], Data[zeile][1], DataColor[Data[zeile][2]][0]);
        setLedPWMOn(Data[zeile][0], Data[zeile][1] +1, DataColor[Data[zeile][2]][1]);
        setLedPWMOn(Data[zeile][0], Data[zeile][1] +2, DataColor[Data[zeile][2]][2]);
}

void setLedsOff(int zeile){
        setPWMOff(Data[zeile][0], Data[zeile][1]);
        setPWMOff(Data[zeile][0], Data[zeile][1] +1);
        setPWMOff(Data[zeile][0], Data[zeile][1] +2);
}


void checkSensor(){
  setLedPWMOn(0, 0, 1000);
  for (uint8_t zeile = 0; zeile < 16; zeile++) {
    //setSensorPWMOn(Data[zeile][0], Data[zeile][1] +3);   //turns pin fully on
    setSensorPWMOn(Data[zeile][0], Data[zeile][1] +3);   //turns pin fully on
    //delayMicroseconds(600);
    delay(2);
    if(!digitalRead(IN_SENSOR)){       //IN_SENSOR = 3
        Data[zeile][3] = 1;
    }else{
        Data[zeile][3] = 0;
    }
    //setPWMOff(Data[zeile][0], Data[zeile][1] +3);    //turns pin fully off
    setPWMOff(Data[zeile][0], Data[zeile][1] +3);    //turns pin fully off
    //delayMicroseconds(200);
    delay(2);
  }
  setLedPWMOn(0, 0, 0);
}

void randomNumber(){
  int randNumber = random(10);
  for (uint8_t zeile = 0; zeile < 16; zeile++)
  {
    Data[zeile][2] = DataColorState[randNumber][zeile];
  }
}