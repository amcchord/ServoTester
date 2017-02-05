/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <FlashStorage.h>



#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define KEY_REPEAT 250 //MS before key repeats;

#define BTN_A 9 //Up
#define BTN_B 6 //Menu
#define BTN_C 5 //Down

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
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

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Servo myservo;
int pos = 90;
int mode = 0;
int menuSelect = 0;
int lastButton = 0;

unsigned long lastMillis = 0;
unsigned long firstMillis = 0;

int speed = 10;
int direction = 1;
int tick = 1;

FlashStorage(longTermPOS, int);


void setup()   {

  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  Wire.setClock(1600000);

  display.clearDisplay();
  display.display();

  pos = longTermPOS.read();
  if (pos < 1 || pos > 180){
    pos = 90;
  }
  myservo.attach(10);
  myservo.write(pos);



  // display.drawPixel(10, 10, WHITE);
  // display.display();
  // delay(2000);
  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // display.setCursor(0,0);
  // display.println("Hello, world!");
  // display.setTextColor(BLACK, WHITE); // 'inverted' text
  // display.println(3.141592);
  // display.setTextSize(2);
  // display.setTextColor(WHITE);
  // display.print("0x"); display.println(0xDEADBEEF, HEX);
  // display.display();


  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("Servo Tstr");
  display.display();
  delay(500);
  display.clearDisplay();

}


void loop() {

  if (mode == 0){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    if (menuSelect == 1){
      display.setTextColor(BLACK, WHITE);
    } else {
      display.setTextColor(WHITE, BLACK);
    }
    display.println("- Manual Mode");
    if (menuSelect == 2){
      display.setTextColor(BLACK, WHITE);
    } else {
      display.setTextColor(WHITE, BLACK);
    }
    display.println("- Sweep Mode");
    if (menuSelect == 3){
      display.setTextColor(BLACK, WHITE);
    } else {
      display.setTextColor(WHITE, BLACK);
    }
    display.println("- Ramp Mode");
    if (menuSelect == 4){
      display.setTextColor(BLACK, WHITE);
    } else {
      display.setTextColor(WHITE, BLACK);
    }
    display.println("- Random Mode");
    display.setCursor(108,0);
    display.setTextColor(WHITE, BLACK);
    updateFlash(pos);
    display.println(pos);
    display.display();
    if(handleMenu(menuSelect, 0, 4) == 2){
      mode = menuSelect;
    }

    delay(100);
  }
  else if (mode == 1){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Manual");
    display.println("");
    printServo();
    display.display();
    myservo.write(pos);
    if(handleNumber(pos, 0, 180) == 2){
      mode = 0;
    }
    delay(10);

  }
  else if (mode == 2){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Sweep");
    display.println("");
    display.setTextSize(2);
    display.print("D:");
    if (speed < 10){
      display.print(" ");
    }
    if (speed < 100){
      display.print(" ");
    }
    display.print(speed);
    printServo();
    display.display();
    if (handleNumber(speed, 1, 100) == 2){
      mode = 0;
    }
    myservo.write(pos);

    if (tick % speed == 0){
      if (direction == 0){
        pos++;
        if (pos == 180){
          direction = 1;
        }
      }
      if (direction == 1){
        pos--;
        if (pos == 0){
          direction = 0;
        }
      }
      tick = 1;
    }
    tick++;
    delay(1);

  }
  else if (mode == 3){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Ramp");
    display.println("");
    display.setTextSize(2);
    display.print("D:");
    if (speed < 10){
      display.print(" ");
    }
    if (speed < 100){
      display.print(" ");
    }
    display.print(speed);
    printServo();
    display.display();
    if (handleNumber(speed, 1, 100) == 2){
      mode = 0;
    }
    myservo.write(pos);

    if (tick % speed == 0){
      if (direction == 0){
        pos++;
        if (pos == 180){
          direction = 1;
        }
      }
      if (direction == 1){
        pos = 0;
        direction = 0;
      }
      tick = 1;
    }
    tick++;
    delay(1);
  }
  else if (mode == 4){
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Random");
    display.println("");
    display.setTextSize(2);
    display.print("D:");
    if (speed < 10){
      display.print(" ");
    }
    if (speed < 100){
      display.print(" ");
    }
    display.print(speed);
    printServo();
    display.display();
    if (handleNumber(speed, 1, 100) == 2){
      mode = 0;
    }
    myservo.write(pos);

    if (tick % speed == 0){
      pos = random(0,180);
    }
    tick++;
    delay(1);

  }

 //  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
 //   // in steps of 1 degree
 //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
 //   delay(15);                       // waits 15ms for the servo to reach the position
 // }
 // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
 //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
 //   delay(15);                       // waits 15ms for the servo to reach the position
 // }

}

void printServo(){
  int servo = myservo.read();
  int servoMS = myservo.readMicroseconds();


  display.setCursor(70,0);
  display.setTextSize(3);
  if (servo < 10){
    display.print(" ");
  }
  if (servo < 100){
    display.print(" ");
  }
  display.println(servo);
  display.setTextSize(1);
  display.setCursor(70,24);
  display.print("mS: ");
  display.println(servoMS);

}


int handleNumber(int &myNumber, int min, int max){
  int btn = checkButtons();
  int fastMode = 0;
  if (btn != lastButton || firstMillis == 0){
    firstMillis = millis();
  }
  if (btn == lastButton && lastButton != 0 && millis() > firstMillis + KEY_REPEAT * 5){
      fastMode = 1;
  } else {
      fastMode = 0;
  }

  if (btn == lastButton && lastButton != 0 && millis() < lastMillis + KEY_REPEAT){
      return 0;
  }
  else {
    lastButton = btn;
    lastMillis = millis();
    if (btn == 1){
      if (myNumber < max){
        if (fastMode){
          myNumber = myNumber + 10;
        } else {
          myNumber++;
        }
      }
    }
    if (btn == 3){
      if(myNumber > min){
        if (fastMode){
          myNumber = myNumber - 10;
        } else {
          myNumber--;
        }
      }
    }
    if (myNumber < min){
      myNumber = min;
    }
    else if (myNumber > max){
      myNumber = max;
    }
    return btn;
  }
}

void updateFlash(int currentPos){
  if(longTermPOS.read() != currentPos){
    longTermPOS.write(currentPos);
    delay(250);
  }
}

int handleMenu(int &menuVal, int min, int max){
  int btn = checkButtons();
  if (btn == lastButton && lastButton != 0 && millis() < lastMillis + KEY_REPEAT){
      //Do Nothing
      return 0;
  }
  else {
    lastButton = btn;
    lastMillis = millis();
    if (btn == 1){
      if (menuVal > min){
        menuVal--;
      }
    }
    if (btn == 3){
      if(menuVal < max){
        menuVal++;
      }
    }
    return btn;
  }
}


int checkButtons(){
  int buttonValue = 0;
  int a = 0;
  int b = 0;
  int c = 0;

  a = digitalRead(BTN_A);
  b = digitalRead(BTN_B);
  c = digitalRead(BTN_C);

  if (a == 0){
    delay(1);
    if (!digitalRead(BTN_A)){
      buttonValue += 1;
    }
  }
  else if (b == 0){
    delay(1);
    if (!digitalRead(BTN_B)){
      buttonValue += 2;
    }
  }
  else if (c == 0){
    delay(1);
    if (!digitalRead(BTN_C)){
      buttonValue += 3;
    }
  }

  return buttonValue;

}
