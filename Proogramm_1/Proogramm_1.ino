#include <Arduino.h>
#include <Servo.h>
Servo zip;

#include "defs.h"

// Подключаем mp3 плеер
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;


// Подключение моторов
#include "MMotor.h"
MMotor baraban(BARABAN);
MMotor boxMotor(CASSETTE);
MMotor water(WATER);

// Подключаем кнопки
#include "GyverButton.h"
GButton mainBtn(42);     // Основная кнопка
GButton ROTATE_BTN(12);  // Кнопка поворота

// Подключение подсветки
#include "LED.h"
LED MainLED;  // Светодиоды главной кнопки

// Подключение датчика отпечатка пальца
#include "Finger.h"
Finger finger(&MainLED);

int currentID;

int value = 0, mode = 0, currentBox = 1, targetBox = 1, blankBox;
bool defaultColor = true, Ispour = false, clickFlag = false;
unsigned long defaultColorTimer, timer, waitTime, hueta, happyTime;

bool isWater = false, swWater = false, flagOK = false, giveWater = false;
bool isOther = false, pourAddedPills = false, withoutWater = false;
bool happyEnd = false, needZIP = false;

void GivePills(int index, bool secondCall = false, bool pourWater = true) {
  if (targetBox != index) {
    targetBox = index;
    baraban.rotate(index);
  } else secondCall = true;

  if (secondCall) {
    Ispour = false;

    if (pourWater) isWater = true;
    else isWater = false;
    Serial.println("Я подъехал к " + String(index) + " боксу");
    boxMotor.supply();
    hueta = millis();
    Serial.println(String(isWater) + " флаг на воду");
    boxMotor.setState_int(0);
  }
}
void getPills(bool flag = true) {
  if (!isPillGlass()) {
    myDFPlayer.playMp3Folder(NotGlassP);
    while (true) {
      digitalWrite(10, 1);
      if (isPillGlass()) break;
    }
    digitalWrite(10, 0);
  }
  needZIP = false;
  if (flag) {
    switch (mode) {
      case 1: GivePills(1); break;
      case 2: GivePills(2); break;
      case 3: GivePills(3); break;
    }
  } else {
    needZIP = true;
    GivePills(1, false, false);
  }
}

void endPills(bool IS_zip) {
  if (IS_zip) {
    zip.attach(ZIP_PIN);
    for (int i = ANGLE_UP; i > ANGLE_DOWN; --i){
      zip.write(i);
      delay(10);
    }
    for (int i = ANGLE_DOWN; i < ANGLE_UP; ++i){
      zip.write(i);
      delay(10);
    }
    zip.detach();
  }
  MainLED.blink(GREEN, true);
  // Возьмите стакан с зоны выдачи таблеток
  myDFPlayer.playMp3Folder(TakeGlassP);
  delay(4000);
  MainLED.off(true);
}


void setup() {
    Serial.begin(115200);

    MainLED.begin();
    pinMode(10, OUTPUT);
    finger.init();
    player_setup();

    water.begin();
    baraban.begin();
    boxMotor.begin();
  //  boxMotor.home();
    baraban.home();
}

void yield() {
  MainLED.tick();
}

void loop() {
  MainLED.tick();
  finger.scan();

  mainBtn.tick();
  ROTATE_BTN.tick();

  baraban.tick();
  boxMotor.tick();
  water.tick();

  if (defaultColor) MainLED.setColor(CYAN);

  currentID = finger.getID();

  if (currentID == 255) {
    myDFPlayer.playMp3Folder(UserWrong);
    finger.clearID();
  } else if (currentID == mode && clickFlag) {
    clickFlag = false;
    Serial.println(String(currentID) + "\t\t" + String(mode));
    getPills();
    finger.clearID();
  }
  if (pourAddedPills) {
    pourAddedPills = false;
    Ispour = true;
    getPills(false);
  }
  if (happyEnd) {
    happyEnd = false;
    endPills(needZIP);
  }

  if (baraban.getState() && Ispour) {
    if (!withoutWater) GivePills(targetBox, true);
    else GivePills(targetBox, true, false);
  }
  if (boxMotor.getState_int() == 1 && isWater) {
    isWater = false;
    Serial.println("test " + String(millis() - hueta));
    giveWater = true;
    boxMotor.setState_int(0);
  }

  if (boxMotor.getState_int() == 10 && giveWater) {
    giveWater = false;
    getWater();
    boxMotor.setState_int(0);
  }

  if (isOther) addedPills();

  if (Serial.available()) {
      String b = (String)Serial.readString();
      String command = b.substring(2, b.length()-1);

      if (b[0] == 'R') {
          Serial.print("rotate ");
          Serial.println(command);
          baraban.rotate(command.toInt());
      }
      if (b[0] == 'B') {
          Serial.print("get blank ");
          Serial.println(command);
          baraban.getBox(command.toInt());
      }
      if (b[0] == 'P') {
          Serial.print("take pills ");
          Serial.println(command);
          boxMotor.supply(command.toInt());
      }
      if (b[0] == 'L') {
          Serial.print("led ");
          if (command == "1") MainLED.blink(RED);
          if (command == "2") MainLED.setColor(BLUE);
          if (command == "3") MainLED.setColor(PURPLE);
          if (command == "4") MainLED.setColor(CYAN);
          if (command == "5") MainLED.setColor(WHITE);
          if (command == "6") MainLED.blink(GREEN);
          if (command == "0") MainLED.blink(None);
          Serial.println(command);
      }
      if (b[0] == 'F') {
          Serial.print("Finger " + String(command));
          if (command == "1") finger.setEvent(READ);
          if (command == "2") finger.setEvent(WRITE);
          if (command == "3") finger.setEvent(CLEAR);
      }
      if (b[0] == 'G') {
        Serial.println("Give pill of box " + String(command));
        GivePills(command.toInt());
        Ispour = true;
      }
  }
  
  if (mainBtn.isSingle()) {
    MainLED.setColor(PURPLE);
    delay(200);
    MainLED.setColor(CYAN);
    if (++mode > 3) mode = 1;
    Ispour = true;
    clickFlag = true;
    switch (mode) {
      case 1: myDFPlayer.playMp3Folder(Users1); break;
      case 2: myDFPlayer.playMp3Folder(Users2); break;
      case 3: myDFPlayer.playMp3Folder(Users3); break;
    }
    defaultColor = false;
    finger.setEvent(READ, mode);
  }
  if (mainBtn.isTriple()) {  // Запись отпечатка
    MainLED.blink(BLUE);
    defaultColor = false;
    finger.setEvent(WRITE);
  }
  if (mainBtn.isStep()) {    // Очистка базы данных
    value++;
    Serial.println(value);
    if (value == 10) {
      value = 0;
      finger.setEvent(CLEAR);
      defaultColor = false;
    }
  }


  if (ROTATE_BTN.isSingle()) baraban.switchBox(true);
  if (ROTATE_BTN.isDouble()) baraban.switchBox(false);
  if (ROTATE_BTN.isStep()) {
    value++;
    Serial.println(value);
    if (value == 5) {
      value = 0;
      boxMotor.fixStop();
    }
  }


}

void getWater() {
  bool SWW = false;
  // Если вам нужна вода, то нажмите на кнопку пока она мигает
  myDFPlayer.playMp3Folder(NeedWater);
  delay(4000);
  timer = millis();
  waitTime = millis();
  while (true) {
    if (millis() - waitTime > 400) {
      SWW = !SWW;
      waitTime = millis();
    }
    MainLED.setColor(SWW ? BLUE : None);
    if (millis() - timer >= delayNeedWater) break;
    if (!digitalRead(42)) {
      flagOK = true;
      delay(100);
      MainLED.off(true);
      break;
    }
  }
  if (flagOK) {
    if (!isWaterGlass()) {
      myDFPlayer.playMp3Folder(NotGlassW);
      while (true) {
        MainLED.setColor(PURPLE);
        if (isWaterGlass()) {
          delay(200);
          break;
        }
      }
    }
    MainLED.off();
    water.getWater();
    MainLED.blink(GREEN, true);
    // Возьмите стакан с зоны выдачи воды
    myDFPlayer.playMp3Folder(TakeGlassW);
    delay(4000);
    MainLED.off(true);
    isOther = true;
    flagOK = false;
  } else {
    isOther = true;
  }
  withoutWater = true;
  
}

void addedPills() {
  bool SWW = false;
  isOther = false;
  myDFPlayer.playMp3Folder(NeedPills1);
  delay(5000);

  timer = millis();
  waitTime = millis();
  while (true) {
    if (millis() - waitTime > 400) {
      SWW = !SWW;
      waitTime = millis();
    }
    MainLED.setColor(SWW ? BLUE : None);
    if (millis() - timer >= delayNeedPills) break;
    if (!digitalRead(42)) {
      flagOK = true;
      delay(150);
      MainLED.off(true);
      break;
    }
  }
  delay(100);
  if (flagOK) {
    pourAddedPills = true;
    flagOK = false;
  }
}

bool isPillGlass() {
  return !digitalRead(PILL_GLASS);
}

bool isWaterGlass() {
  return !digitalRead(WATER_GLASS);
}

void player_setup() {
  Serial2.begin(9600);  //Player_Serial

  if (!myDFPlayer.begin(Serial2)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(VOLUME);  //Set volume value. From 0 to 30
}