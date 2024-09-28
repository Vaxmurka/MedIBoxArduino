#include <Arduino.h>

#include "defs.h"


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

int value = 0, mode = 0;
bool defaultColor = true;
unsigned long defaultColorTimer;

// bool isTake = false, isWater = false;


// void getPills(int box, int count) {
//     baraban.rotate(box);
//     if (baraban.getState() == 1) {
//         boxMotor.supply(count);
//         if (boxMotor.getState() == 1) {
//             Serial.println("OK");
//             isTake = false;

//             isWater = true;
//         }
//         else if (boxMotor.getState() == -1) {
//             Serial.println("Stop -------");
//             Serial.println("ERROR");
//             isTake = false;
//         }
//     }
// }

// void TakingPills() {
//     switch (mode) {
//         case 1:
//             getPills(2, 1);
//             break;
//         case 2:
//             getPills(1, 1);
//             break;
//         case 3:
//             getPills(3, 1);
//             break;
//     }
// }


void setup() {
    Serial.begin(115200);

    MainLED.begin();
    finger.init();

    water.begin();
    baraban.begin();
    boxMotor.begin();
//    boxMotor.home();
    baraban.home();
}

void loop() {
  MainLED.tick();
  finger.scan();

  mainBtn.tick();
  ROTATE_BTN.tick();

  baraban.tick();
  boxMotor.tick();
  water.tick();

  currentID = finger.getID();
  // Serial.println("CurrentID: " + String(currentID));

  if (defaultColor) MainLED.setColor(CYAN);
  if (!defaultColor && millis() - defaultColorTimer > 1500) {
      defaultColor = true;
  }
  

    // if (isTake) TakingPills();
    // if (isWater) {
    //     water.getWater();
    //     isWater = false;
    // }

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
   }
  
  if (mainBtn.isSingle()) {  // Чтение отпечатков с определением личности
    MainLED.click();
    defaultColor = false;
    defaultColorTimer = millis();
    finger.setEvent(READ);
  }
  if (mainBtn.isTriple()) {  // Запись отпечатка
    MainLED.blink(BLUE);
    defaultColor = false;
    defaultColorTimer = millis();
    finger.setEvent(WRITE);
  }
  if (mainBtn.isStep()) {    // Очистка базы данных
    value++;
    Serial.println(value);
    if (value == 10) {
      finger.setEvent(CLEAR);
      defaultColor = false;
      defaultColorTimer = millis();
    }
  }



//     if (mainBtn.isSingle()) {  // Чтение отпечатков с определением личности
//         MainLED.click();
//         defaultColor = false;
//         defaultColorTimer = millis();

//         isTake = true;
//         if (++mode > 3) mode = 1;
// //        fingerMode = 0;
//     }

//     if (mainBtn.isTriple()) {  // Запись отпечатка
//         MainLED.blink(BLUE);
//         defaultColor = false;
//         defaultColorTimer = millis();
// //        fingerMode = 5;
//     }

//     if (mainBtn.isStep()) {  // Очистка базы данных
//         value++;
//         Serial.println(value);
//         defaultColor = false;
//         defaultColorTimer = millis();
// //        if (value == 10) fingerMode = 10;
//     }

}