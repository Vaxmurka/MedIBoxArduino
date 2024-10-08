//
// Created by Nikit on 22.08.2024.
//
#include <Arduino.h>
#include "LED.h"

LED::LED() {
    pinRed = 41;
    pinBlue = 40;
    pinGreen = 39;
}

void LED::begin() {
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinBlue, OUTPUT);
}

void LED::tick() {
//    setColor(CYAN);
    if (!ledLoopRunning) return;

    if (clickFlag) {
        if (millis() - tmr > 500) {
            off();
            clickFlag = false;
            ledLoopRunning = false;
            return;
        }
        setColor(PURPLE);
    }

    if (errorFlag) {
        if (stateError == 1) {
            if (millis() - tmr > 2000) {
                off();
                errorFlag = false;
                ledLoopRunning = false;
                return;
            }
            setColor(RED);
        } else if (stateError == 2) {
            if (count == 4) {
                errorFlag = false;
                ledLoopRunning = false;
            }

            if (millis() - tmr > 200 && count < 4) {
                ++count;
                SW = !SW;
                tmr = millis();
            }

            setColor(SW ? RED : None);
        }
    }

    if (blinkFlag) {
        if (!blinkLoop) {
          if (count == 6) {
              blinkFlag = false;
              ledLoopRunning = false;
          }
          if (millis() - tmr > 200 && count < 6) {
              ++count;
              SW = !SW;
              tmr = millis();
          }
        } else {
          if (millis() - tmr > 400) {
            SW = !SW;
            tmr = millis();
          }
        }
        setColor(SW ? color : None);
    }
}

void LED::setColor(Color _color) {
    Color newcolor = _color;

    switch(newcolor) {
        case RED:
            digitalWrite(pinRed, 1);
            digitalWrite(pinGreen, 0);
            digitalWrite(pinBlue, 0);
            break;
        case GREEN:
            digitalWrite(pinRed, 0);
            digitalWrite(pinGreen, 1);
            digitalWrite(pinBlue, 0);
            break;
        case BLUE:
            digitalWrite(pinRed, 0);
            digitalWrite(pinGreen, 0);
            digitalWrite(pinBlue, 1);
            break;
        case PURPLE:
            digitalWrite(pinRed, 1);
            digitalWrite(pinGreen, 0);
            digitalWrite(pinBlue, 1);
            break;
        case CYAN:
            digitalWrite(pinRed, 0);
            digitalWrite(pinGreen, 1);
            digitalWrite(pinBlue, 1);
            break;
        case WHITE:
            digitalWrite(pinRed, 1);
            digitalWrite(pinGreen, 1);
            digitalWrite(pinBlue, 1);
            break;
        case None:
            off();
            break;
    }
}

void LED::off(bool stop = false) {
    if (stop) {
      blinkLoop = false;
      ledLoopRunning = false;
    }
    digitalWrite(pinRed, 0);
    digitalWrite(pinGreen, 0);
    digitalWrite(pinBlue, 0);
}

void LED::click() {
    tmr = millis();
    clickFlag = true;
    errorFlag = false;
    blinkFlag = false;
    ledLoopRunning = true;
}

void LED::error(int state) {
    tmr = millis();
    errorFlag = true;
    stateError = state;
    count = 0;
    clickFlag = false;
    blinkFlag = false;
    ledLoopRunning = true;
}

void LED::blink(Color _color, bool Loop = false) {
    color = _color;
    tmr = millis();
    blinkFlag = true;
    if (Loop) blinkLoop = true;
    else blinkLoop = false;
    count = 0;
    clickFlag = false;
    errorFlag = false;
    ledLoopRunning = true;
}