#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <FlashStorage.h>



#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define KEY_REPEAT 250 //MS before key repeats;
#define SERVO_PIN 10 // This is the pin for the servo output
#define BTN_A 9 //Up
#define BTN_B 6 //Menu
#define BTN_C 5 //Down

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//Globals
Servo myservo;
int pos = 90;
int mode = 0;
int menuSelect = 0;
int lastButton = 0;
int min = 0;
int max = 180;
int subMenu = 0;

//Needed for Key Repeat
unsigned long lastMillis = 0;
unsigned long firstMillis = 0;

//For auto-servo stuff
int speed = 10;
int direction = 1;
int tick = 1;

FlashStorage(longTermPOS, int);
FlashStorage(minFlash, int);
FlashStorage(maxFlash, int);


void setup()   {

  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  Wire.setClock(1600000);
  display.clearDisplay();
  display.display();
  pos = longTermPOS.read();
  if (minFlash.read() > 0 && minFlash.read() < 180){
    min = minFlash.read();
  }
  if (maxFlash.read() > 0 && maxFlash.read() < 180){
    max = maxFlash.read();
  }
  if (pos < 1 || pos > 180){
    pos = 90;
  }
  myservo.attach(SERVO_PIN);
  myservo.write(pos);


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

  if (mode == 0){ //Menu
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

    if (menuSelect == 5){
      display.setTextColor(BLACK, WHITE);
    } else {
      display.setTextColor(WHITE, BLACK);
    }
    display.setCursor(107,24);
    display.println("m/M");


    display.setCursor(108,0);
    display.setTextColor(WHITE, BLACK);
    updateFlash(pos);
    display.println(pos);
    display.display();
    if(handleMenu(menuSelect, 0, 5) == 2){
      mode = menuSelect;
    }
    delay(100);
  }
  else if (mode == 1){ //Manual Mode
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Manual");
    display.println("");
    printServo();
    display.display();
    myservo.write(pos);
    if(handleNumber(pos, min, max) == 2){
      mode = 0;
    }
    delay(10);

  }
  else if (mode == 2){ //Sweep Mode
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
        if (pos == max){
          direction = 1;
        }
      }
      if (direction == 1){
        pos--;
        if (pos == min){
          direction = 0;
        }
      }
      tick = 1;
    }
    tick++;
    delay(1);

  }
  else if (mode == 3){ //Ramp Mode
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
        if (pos == max){
          direction = 1;
        }
      }
      if (direction == 1){
        pos = min;
        direction = 0;
      }
      tick = 1;
    }
    tick++;
    delay(1);
  }
  else if (mode == 4){ //Random Mode
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
      pos = random(min,max);
    }
    tick++;
    delay(1);

  }
  else if (mode == 5){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    if (subMenu == 0){
      display.setTextColor(BLACK,WHITE);
    }
    display.print("Min");
    display.setTextColor(WHITE,BLACK);
    display.setCursor(107,0);
    if (subMenu == 1){
      display.setTextColor(BLACK,WHITE);
    }
    display.print("Max");
    display.setTextColor(WHITE,BLACK);
    display.setCursor(0,8);
    display.setTextSize(3);
    display.print(min);
    display.setCursor(72,8);
    display.print(max);
    display.display();

      if (subMenu == 0){
        if (handleNumber(min, 0 , 200) == 2){
          subMenu = 1;
        }
      }
      else if (subMenu == 1){
        if (handleNumber(max, 0 , 200) == 2){
          subMenu = 0;
          maxFlash.write(max);
          minFlash.write(min);
          mode = 0;
          menuSelect = 1;
        }
      }
    }
}

void printServo(){ //Print the servo angle and Milliseconds
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


int handleNumber(int &myNumber, int min, int max){ //Update a number using the buttons
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

void updateFlash(int currentPos){ //Update a number stored in flash ram
  if(longTermPOS.read() != currentPos){
    longTermPOS.write(currentPos);
    delay(250);
  }
}

int handleMenu(int &menuVal, int min, int max){ //Update menu items
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


int checkButtons(){ //Read the button inputs;
  int buttonValue = 0;
  int a = 0;
  int b = 0;
  int c = 0;

  a = digitalRead(BTN_A);
  b = digitalRead(BTN_B);
  c = digitalRead(BTN_C);

  if (a == 0){
    delay(1); //Bounce Check
    if (!digitalRead(BTN_A)){
      buttonValue += 1;
    }
  }
  else if (b == 0){
    delay(1); //Bounce Check
    if (!digitalRead(BTN_B)){
      buttonValue += 2;
    }
  }
  else if (c == 0){
    delay(1); //Bounce Check
    if (!digitalRead(BTN_C)){
      buttonValue += 3;
    }
  }
  return buttonValue;
}
