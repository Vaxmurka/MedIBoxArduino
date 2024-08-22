//
// Created by Nikit on 22.08.2024.
//

#ifndef MEDIBOX_FINGER_H
#define MEDIBOX_FINGER_H

#include "LED.h"


class Finger {
public:
    Finger(LED* _led);
    void init();
private:
    LED* led;
};


#endif //MEDIBOX_FINGER_H
