//#include <Arduino.h>
//
//#include "defs.h"
//
//// Подключаем mp3 плеер
//#include "DFRobotDFPlayerMini.h"
//DFRobotDFPlayerMini myDFPlayer;
//
//// Подключение моторов
//#include "MMotor.h"
//MMotor baraban(BARABAN);
//MMotor boxMotor(CASSETTE);
//MMotor water(WATER);
//
//// Подключаем кнопки
//#include "GyverButton.h"
//GButton mainBtn(42);     // Основная кнопка
//GButton ROTATE_BTN(12);  // Кнопка поворота
//
//// Подключение подсветки
//#include "LED.h"
//LED MainLED;  // Светодиоды главной кнопки
//
//// Подключение датчика отпечатка пальца
//#include "Finger.h"
//Finger finger(&MainLED);
//
//
//int currentID;
//
//int value = 0, mode = 0, currentBox = 1, targetBox = 1, blankBox;
//bool defaultColor = true, Ispour = false;
//unsigned long defaultColorTimer;
//
////int value = 0, mode = 0;
////bool defaultColor = true;
////unsigned long defaultColorTimer;
////
////bool isTake = false, isWater = false;
////
////
////void getPills(int box, int count) {
////    baraban.rotate(box);
////    if (baraban.getState() == 1) {
////        boxMotor.supply(count);
////        if (boxMotor.getState() == 1) {
////            Serial.println("OK");
////            isTake = false;
////
////            isWater = true;
////        }
////        else if (boxMotor.getState() == -1) {
////            Serial.println("Stop -------");
////            Serial.println("ERROR");
////            isTake = false;
////        }
////    }
////}
////
////void TakingPills() {
////    switch (mode) {
////        case 1:
////            getPills(2, 1);
////            break;
////        case 2:
////            getPills(1, 1);
////            break;
////        case 3:
////            getPills(3, 1);
////            break;
////    }
////}
//
//void GivePills(int index, bool secondCall = false) {
//    if (targetBox != index) {
//        targetBox = index;
//        baraban.rotate(index);
//    } else secondCall = true;
//
//    if (secondCall) {
//        Ispour = false;
//        Serial.println("Я подъехал к " + String(index) + " боксу");
//        boxMotor.supply();
//    }
//}
//
//void player_setup() {
//    Serial2.begin(9600);  //Player_Serial
//
//    if (!myDFPlayer.begin(Serial2)) {  //Use softwareSerial to communicate with mp3.
//        Serial.println(F("Unable to begin:"));
//        Serial.println(F("1.Please recheck the connection!"));
//        Serial.println(F("2.Please insert the SD card!"));
//    }
//    Serial.println(F("DFPlayer Mini online."));
//
//    myDFPlayer.volume(VOLUME);  //Set volume value. From 0 to 30
//}
//
//
//void setup() {
//    Serial.begin(115200);
//
//    MainLED.begin();
//    finger.init();
//    player_setup();
//
//    water.begin();
//    baraban.begin();
//    boxMotor.begin();
////    boxMotor.home();
//    baraban.home();
//}
//
//void loop() {
//    if (defaultColor) MainLED.setColor(CYAN);
//    if (!defaultColor && millis() - defaultColorTimer > 1500) {
//        defaultColor = true;
//    }
//    finger.scan();
//
//    baraban.tick();
//    boxMotor.tick();
//    water.tick();
//
//    MainLED.tick();
//    mainBtn.tick();
//    ROTATE_BTN.tick();
//
//    currentID = finger.getID();
////    if (isTake) TakingPills();
////    if (isWater) {
////        water.getWater();
////        isWater = false;
////    }
//
////    if (Serial.available()) {
////        String b = (String)Serial.readString();
////        String command = b.substring(2, b.length()-1);
////
////        if (b[0] == 'R') {
////            Serial.print("rotate ");
////            Serial.println(command);
////            baraban.rotate(command.toInt());
////        }
////        if (b[0] == 'B') {
////            Serial.print("get blank ");
////            Serial.println(command);
////            baraban.getBox(command.toInt());
////        }
////        if (b[0] == 'P') {
////            Serial.print("take pills ");
////            Serial.println(command);
////            boxMotor.supply(command.toInt());
////        }
////        if (b[0] == 'L') {
////            Serial.print("led ");
////            if (command == "1") MainLED.blink(RED);
////            if (command == "2") MainLED.blink(BLUE);
////            if (command == "3") MainLED.blink(PURPLE);
////            if (command == "4") MainLED.blink(CYAN);
////            if (command == "5") MainLED.blink(WHITE);
////            if (command == "6") MainLED.blink(GREEN);
////            if (command == "0") MainLED.blink(None);
////            Serial.println(command);
////        }
////    }
//
//
//    if (mainBtn.isSingle()) {  // Чтение отпечатков с определением личности
//        MainLED.click();
//        if (++mode > 4) mode = 1;
//        Ispour = true;
//        switch (mode) {
//            case 1: myDFPlayer.playMp3Folder(Users1); break;
//            case 2: myDFPlayer.playMp3Folder(Users2); break;
//            case 3: myDFPlayer.playMp3Folder(Users1); break;
//            case 4: myDFPlayer.playMp3Folder(Users2); break;
//        }
//        delay(6000);
//        defaultColor = false;
//        defaultColorTimer = millis();
//        finger.setEvent(READ);
//    }
//
//    if (mainBtn.isTriple()) {  // Запись отпечатка
//        MainLED.blink(BLUE);
//        defaultColor = false;
//        defaultColorTimer = millis();
//        finger.setEvent(WRITE);
//    }
//
//    if (mainBtn.isStep()) {    // Очистка базы данных
//        value++;
//        Serial.println(value);
//        if (value == 10) {
//            finger.setEvent(CLEAR);
//            defaultColor = false;
//            defaultColorTimer = millis();
//        }
//    }
//}

#include <Arduino.h>

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
bool defaultColor = true, Ispour = false;
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

void GivePills(int index, bool secondCall = false) {
    if (targetBox != index) {
        targetBox = index;
        baraban.rotate(index);
    } else secondCall = true;

    if (secondCall) {
        Ispour = false;
        Serial.println("Я подъехал к " + String(index) + " боксу");
        boxMotor.supply();
    }
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


void setup() {
    Serial.begin(115200);

    MainLED.begin();
    finger.init();
    player_setup();

    water.begin();
    baraban.begin();
    boxMotor.begin();
    //  boxMotor.home();
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
    if (currentID == mode) {
        // Serial.println("Найдено совпадение  с базой");
        switch (mode) {
            case 1: GivePills(1); break;
            case 2: GivePills(2); break;
            case 3: GivePills(1); break;
            case 4: GivePills(2); break;
        }
        finger.clearID();
    } else {
        // Serial.println("Не найдено совпадение  с базой");
        myDFPlayer.playMp3Folder(UserWrong);
    }
    currentBox, blankBox = baraban.getCurrentBlankBox();
    if (baraban.getState() && Ispour) GivePills(targetBox, true);
    // if (!baraban.getState()) Serial.println(baraban.getState());
    // else Serial.println(" ------- STOP ----------");
    // Serial.println("CurrentID: " + String(currentID));

    if (defaultColor) MainLED.setColor(CYAN);
    if (!defaultColor && millis() - defaultColorTimer > 1500) {
        defaultColor = true;
    }

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

    if (mainBtn.isSingle()) {  // Чтение отпечатков с определением личности
        MainLED.click();
        if (++mode > 4) mode = 1;
        Ispour = true;
        switch (mode) {
            case 1: myDFPlayer.playMp3Folder(Users1); break;
            case 2: myDFPlayer.playMp3Folder(Users2); break;
            case 3: myDFPlayer.playMp3Folder(Users1); break;
            case 4: myDFPlayer.playMp3Folder(Users2); break;
        }
        delay(6000);
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

}