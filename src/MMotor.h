//
// Created by Nikit on 22.08.2024.
//

#ifndef MEDIBOX_MMOTOR_H
#define MEDIBOX_MMOTOR_H

#include "defs.h"

enum Type {
    BARABAN,
    CASSETTE,
    WATER
};

class MMotor {
public:
    MMotor(Type _type);
    void begin();
    void tick();
    int getState();
    void get() {
        Serial.println(type);
    }

    void rotate(int box);   // Поворот барабана контейнер -> подача
    void home();            // Возвращение в нулевую точку
    void getBox(int box);   // Поворот барабана контейнер -> дверь

    void parcking();        // Парковка
    void supply(int count); // Подача таблеток

    void getWater();
private:
    Type type;
    uint8_t pin1, pin2, end, homeP, checkPill;

    int currentBox = 1, blankBox = 6;
    int boxCounter, counter, pillCounter;
    bool dir, flagParcking = false, stopSupply = false;
    int state = 0;

    int powerB = 250;
    int powerC = 60;

    unsigned long _tmr, pillTimer, stopTimer, parckingTimer, waterTimer;
    bool _flag, flagPill, flagStop = false;

    void run(bool _dir);
    void stop();
    bool isHome() {
        return digitalRead(HOME_POINT);
    }
    bool click() {
        bool btnState = digitalRead(11);
        if (!btnState && !_flag && millis() - _tmr >= 200) {
            _flag = true;
            _tmr = millis();
            return true;
        }
        if (!btnState && _flag && millis() - _tmr >= 500) {
            _tmr = millis();
            return true;
        }
        if (btnState && _flag) {
            _flag = false;
            _tmr = millis();
        }
        return false;
    }
    bool isPill() {
        bool checkState = digitalRead(CHECK_PILL);
        return !checkState;
    }

    bool rotateLoopRunning = false;

    int arrayR[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int arrayB[8] = {6, 7, 8, 1, 2, 3, 4, 5};
    int Array(int array[], int val);
};


#endif //MEDIBOX_MMOTOR_H
