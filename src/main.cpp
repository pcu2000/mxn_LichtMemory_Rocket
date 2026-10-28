#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PCA9685.h"
//#include "display.h"
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

#define IN_BUTTON D0
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
void ledWrongPair(int Ball1, int Ball2);
void writeState();
void drawActivePlayer();

void randomNumber();

void checkRocketCount();


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

#define PCB 0
#define LED 1
#define COLOR 2
#define SENSaCT 3
#define SENSoLD 4
#define PLAYER 5

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

int DataColorOld[8][3] = {
        {4000, 4000,    0},    // Gelb
        {4000,    0, 4000},    // Pink / Magenta
        {   0, 4000, 4000},    // Cyan
        {4000, 2000,    0},    // Orange
        {2000,    0, 4000},    // Violett
        {2000, 4000, 1000},    // Lime
        {4000, 4000, 4000},    // Weiß
        {1000, 3000, 4000}     // Türkis / Himmelblau
};

int DataColor[8][3] = {
        {   0, 4000,    0},    // Gelb
        {   0,    0, 4000},    // Pink / Magenta
        {   0, 4000, 4000},    // Cyan
        {4000, 4000,    0},    // Orange
        {4000,    0, 4000},    // Violett
        {4000, 4000, 4000},    // Lime
        {4000, 1000, 4000},    // Weiß
        {1000, 4000, 1000}     // Türkis / Himmelblau
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

#define STATEINIT        0
#define STATEROCKET1     10
#define STATEROCKET2     11
#define STATEROCKETFAIL  12
#define STATEPAIROK      13
#define STATEPAIRNOK     14

int gameState = STATEINIT;

uint8_t rocketCount = 0;
bool activePlayer = 0;
uint8_t player1Count = 0;
uint8_t player2Count = 0;

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
  //display_init();
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
  //display.drawPixel(14, 10, WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(15,4);             // Start at top-left corner
  display.println(F("Initialisierung"));

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

  display.setCursor(15,20);             // Start at top-left corner
  display.println(F("LED Test"));
  display.display();

  for(uint8_t place = 0; place <16; place++){
      PCA[Data[place][PCB]].setPWM(Data[place][LED], 0, 1000);
      PCA[Data[place][PCB]].setPWM(Data[place][LED]+1, 0, 1000);
      PCA[Data[place][PCB]].setPWM(Data[place][LED]+2, 0, 1000);
      delay(100);
  }

  for(uint8_t place = 0; place <16; place++){
      PCA[Data[place][PCB]].setPWM(Data[place][LED], 0, 0);
      PCA[Data[place][PCB]].setPWM(Data[place][LED]+1, 0, 0);
      PCA[Data[place][PCB]].setPWM(Data[place][LED]+2, 0, 0);
      PCA[Data[place][PCB]].setPWM(Data[place][LED]+3, 0, 0);
      delay(100);
  }

  display.clearDisplay();
  display.dim(false);
  display.drawRect(13, 0, 100, 62, WHITE);
  display.drawRect(14, 1, 57, 57, WHITE);
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
  display.display();

  drawActivePlayer();

  //delay(5000);
  //Serial.begin(115200);

}

void loop() {

  delay(500);
  switch (gameState)
    {
    case STATEINIT:
        checkSensor();
        checkRocketCount();
      break;
    
      case STATEROCKET1:
      /* code */
      break;
    
      case STATEROCKET2:
      /* code */
      break;
    
      case STATEPAIROK:
      
        drawActivePlayer();
        gameState = STATEINIT;
      break;
    
      case STATEPAIRNOK:
      
        checkSensor();
        if (Data[Ball1][SENSaCT] == 1 || Data[Ball2][SENSaCT] == 1){
          ledWrongPair(Ball1, Ball2);
        }else{
          Data[Ball1][SENSoLD] = 0;
          Data[Ball2][SENSoLD] = 0;
          setLedsOff(Ball1);
          setLedsOff(Ball2);
          rocketCount = 0;
          activePlayer = !activePlayer;
          drawActivePlayer();
          gameState = STATEINIT;
        }
      break;
    
    default:
      break;
    }
  //checkSensor();
  //checkColorMatch();
  writeState();
  drawSensorData();
  delay(200);

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
  const int16_t startPosX = 22;
  const int16_t startPosY = 8;
  const int16_t spaceCircle = 14;
  const uint16_t radCircle = 5;
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++) {
      display.drawCircle(startPosX + j*spaceCircle, startPosY + i*spaceCircle, radCircle, 1);
    }
  }
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++) {
      display.drawCircle(startPosX + j*spaceCircle, startPosY + i*spaceCircle, 1, Data[i*4+j][3]);
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

void drawActivePlayer(){
  if(!activePlayer){
    display.drawRect(73, 18, 20, 11, 1);
    display.drawRect(73, 38, 20, 11, 0);
  }else{
    display.drawRect(73, 18, 20, 11, 0);
    display.drawRect(73, 38, 20, 11, 1);
  }
  display.fillRect(95, 20, 16, 8, 0);
  display.fillRect(95, 40, 16, 8, 0);
  display.setCursor(95,20);             // Start at top-left corner
  display.println(player1Count);
  display.setCursor(95,40);             // Start at top-left corner
  display.println(player2Count);
  display.display();
}

void writeState(){
  //display.drawRect(20, 50, 80, 10, 1);
  
  display.drawRect(73, 51, 39, 11, 1);
  display.fillRect(74, 53, 30, 8, 0);
  display.setCursor(75,53);             // Start at top-left corner
  //display.write(gameState);        // Draw white text
  //display.println(F("maxon"));
  display.println(gameState);
  display.display();

}
  
  


void setLedPWMOn(int module, int led, int ledBrightness){
  PCA[module].setPWM(led, 0, ledBrightness);
}

void setSensorPWMOn(int module, int led){
  PCA[module].setPWM(led, 0, 4095);
}


void setPWMOff(int module, int led){
  PCA[module].setPWM(led, 0, 0);
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
void checkRocketCount(){
  for (uint8_t zeile = 0; zeile < 16; zeile++){
    if (Data[zeile][SENSaCT] == 1 && Data[zeile][SENSoLD] == 0 && Data[zeile][PLAYER] == 0)
      {
        rocketCount ++;


      switch (rocketCount)
        {
        case 1:
        //Data[zeile][5] = 1;
        Data[zeile][SENSoLD] = 1;
        Ball1 = zeile;
        setLedsOn(Ball1);
          break;

        case 2:
        //Data[zeile][5] = 1;
        Data[zeile][SENSoLD] = 1;
        Ball2 = zeile;
        setLedsOn(Ball2);
        break;

        default:
        Ball3 = zeile;
        ledAnzahlError(Ball1,Ball2,Ball3);
        gameState = STATEROCKETFAIL;
        return;
          break;
        }
      /*}else if(Data[zeile][3] == 0 && Data[zeile][4] == 1){
        rocketCount --;
        //Data[zeile][5] = 0;
        Data[zeile][4] = 0;
        setLedsOff(zeile);
      }*/
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
    if(rocketCount == 2){
      if(Data[Ball1][COLOR] == Data[Ball2][COLOR] ){
        if(!activePlayer){
          Data[Ball1][PLAYER] = 1;
          Data[Ball2][PLAYER] = 1;
          player1Count++;
        }else{
          Data[Ball1][PLAYER] = 2;
          Data[Ball2][PLAYER] = 2;
          player2Count++;
        }
        rocketCount = 0;
        gameState = STATEPAIROK;
      }else{
        ledWrongPair(Ball1, Ball2);
        gameState = STATEPAIRNOK;
      }
    }
}

void ledWrongPair(int Ball1, int Ball2){
      setLedPWMOn(Data[Ball1][PCB], Data[Ball1][LED], 4095);
      setLedPWMOn(Data[Ball2][PCB], Data[Ball2][LED], 4095);
      setLedPWMOn(Data[Ball1][PCB], Data[Ball1][LED]+1, 0);
      setLedPWMOn(Data[Ball2][PCB], Data[Ball2][LED]+1, 0);
      setLedPWMOn(Data[Ball1][PCB], Data[Ball1][LED]+2, 0);
      setLedPWMOn(Data[Ball2][PCB], Data[Ball2][LED]+2, 0);
      delay(200);
      setLedsOn(Ball1);
      setLedsOn(Ball2);
      delay(800);
}

void ledAnzahlError(int Ball1, int Ball2, int Ball3){
      setLedPWMOn(Data[Ball1][PCB], Data[Ball1][LED], 4095);
      setLedPWMOn(Data[Ball2][PCB], Data[Ball2][LED], 4095);
      setLedPWMOn(Data[Ball3][PCB], Data[Ball3][LED], 4095);
      setLedPWMOn(Data[Ball1][PCB], Data[Ball1][LED]+1, 0);
      setLedPWMOn(Data[Ball2][PCB], Data[Ball2][LED]+1, 0);
      setLedPWMOn(Data[Ball3][PCB], Data[Ball3][LED]+1, 0);
      setLedPWMOn(Data[Ball1][PCB], Data[Ball1][LED]+2, 0);
      setLedPWMOn(Data[Ball2][PCB], Data[Ball2][LED]+2, 0);
      setLedPWMOn(Data[Ball3][PCB], Data[Ball3][LED]+2, 0);
}


void setLedsOn(int zeile){
        setLedPWMOn(Data[zeile][PCB], Data[zeile][LED], DataColor[Data[zeile][COLOR]][0]);
        setLedPWMOn(Data[zeile][PCB], Data[zeile][LED] +1, DataColor[Data[zeile][COLOR]][1]);
        setLedPWMOn(Data[zeile][PCB], Data[zeile][LED] +2, DataColor[Data[zeile][COLOR]][2]);
}

void setLedsOff(int zeile){
        setPWMOff(Data[zeile][PCB], Data[zeile][LED]);
        setPWMOff(Data[zeile][PCB], Data[zeile][LED] +1);
        setPWMOff(Data[zeile][PCB], Data[zeile][LED] +2);
}


void checkSensor(){
  //setLedPWMOn(0, 0, 1000);
  display.fillCircle(105, 57, 2, 1);
  display.display();
  for (uint8_t zeile = 0; zeile < 16; zeile++) {
    setSensorPWMOn(Data[zeile][PCB], Data[zeile][LED] +3);   //turns pin fully on
    delay(2);
    if(!digitalRead(IN_SENSOR)){       //IN_SENSOR = 3
        Data[zeile][SENSaCT] = 1;
    }else{
        Data[zeile][SENSaCT] = 0;
    }
    setPWMOff(Data[zeile][PCB], Data[zeile][LED] +3);    //turns pin fully off
    delay(2);
  }
  //setLedPWMOn(0, 0, 0);
  display.fillCircle(105, 57, 2, 0);
  display.display();
}

void randomNumber(){
  int randNumber = random(10);
  for (uint8_t zeile = 0; zeile < 16; zeile++)
  {
    Data[zeile][COLOR] = DataColorState[randNumber][zeile];
  }
}