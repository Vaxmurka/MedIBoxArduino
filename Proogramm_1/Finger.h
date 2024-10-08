#include <stdint.h>
//
// Created by Nikit on 22.08.2024.
//

#ifndef MEDIBOX_FINGER_H
#define MEDIBOX_FINGER_H

#include "LED.h"

enum State {
  READ, 
  WRITE,
  CLEAR
};


class Finger {
public:
    Finger(LED* _led);
    void init();
    void scan();

    void setEvent(State _state, int mode = 0);
    int getID();
    void clearID();
private:
    LED* led;
    State state;

    int targetID;

    bool scanLoop = false, readFlag = false, writeFlag = false;
    bool scanComplite = false;
    unsigned long tmr, loopTime;
    uint8_t id = 1; // Используемый ID (текущий)
    uint8_t VAR_this_ID = 0;  // номер ID, шаблон которого требуется сохранить/удалить

    // void writeFinger();
    void clearData();
    void readFinger();
    uint8_t getFingerprintID();
    uint8_t getFingerprintEnroll();
};


#endif //MEDIBOX_FINGER_H