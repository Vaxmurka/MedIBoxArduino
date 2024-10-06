//
// Created by Nikit on 22.08.2024.
//

#include <Arduino.h>
#include "MMotor.h"

#include "defs.h"


MMotor::MMotor(Type _type) {
    type = _type;

    if (type == BARABAN) {
        pin1 = 4;
        pin2 = 5;
        end = endStop;
        homeP = HOME_POINT;
    } else if (type == CASSETTE) {
        pin1 = 6;
        pin2 = 7;
        checkPill = CHECK_PILL;
    }
}

void MMotor::begin() {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);

    pinMode(11, INPUT_PULLUP);
    pinMode(homeP, INPUT);
    pinMode(checkPill, INPUT);

    pinMode(WATER_PIN, OUTPUT);
}

void MMotor::switchBox(bool dir) {
  int choiseBox = currentBox;
  if (dir) {
    if (++choiseBox > 8) choiseBox = 1;
  } else {
    if (--choiseBox < 1) choiseBox = 8;
  }
  rotate(choiseBox);
}

void MMotor::fixStop() {
  run(true);
  delay(1000);
  run(false);
  delay(2000);
  stop();
}

void MMotor::rotate(int box) {
    if (box == currentBox) return;
    if (box > 8) return;
    if (box < 1) return;

    boxCounter = box - currentBox;
    int min = min(abs(boxCounter), MAX_BOX - abs(boxCounter));

    if (boxCounter > 0) dir = true;
    else dir = false;

    if (abs(boxCounter) > min) {
        boxCounter = min;
        dir = !dir;
    } else {
        boxCounter = abs(boxCounter);
    }

    currentBox = box;
    int index = Array(arrayR, currentBox);
    blankBox = arrayB[index];
    Serial.println("currentBox " + String(currentBox) + " blankBox " + String(blankBox) + "  index " + String(index));

    counter = -1;
    rotateLoopRunning = true;
}

void MMotor::getBox(int box) {
    if (blankBox == box) return;
    if (blankBox < 1 || blankBox > 8) return;

    boxCounter = box - blankBox;
    int min = min(abs(boxCounter), MAX_BOX - abs(boxCounter));

    if (boxCounter > 0) dir = true;
    else dir = false;

    if (abs(boxCounter) > min) {
        boxCounter = min;
        dir = !dir;
    } else {
        boxCounter = abs(boxCounter);
    }

    blankBox = box;

    int index = Array(arrayB, blankBox);
    currentBox = arrayR[index];
    Serial.println("currentBox " + String(currentBox) + " blankBox " + String(blankBox));

    counter = -1;
    rotateLoopRunning = true;
}

void MMotor::home() {
    if (type == BARABAN) {
        if (isHome()) return;

        while (!isHome()) {
            run(false);
        }
        stop();

        currentBox = 1;
    } else if (type == CASSETTE) {
        Serial.println("home");
        parcking();
    }
}

void MMotor::run(bool _dir) {
    if (type == WATER) {
        digitalWrite(WATER_PIN, HIGH);
    } else {
        int power;
        if (type == BARABAN) power = powerB;
        else if (type == CASSETTE) power = powerC;
        analogWrite(pin1, !_dir ? LOW : power);
        analogWrite(pin2, _dir ? LOW : power);
    }
}

void MMotor::stop() {
    if (type == WATER) {
        digitalWrite(WATER_PIN, LOW);
    } else {
        analogWrite(pin1, LOW);
        analogWrite(pin2, LOW);
    }
}

void MMotor::tick() {
    if (!rotateLoopRunning) return;

    if (type == BARABAN) tickBaraban();
    else if (type == CASSETTE) tickCassette();
    else tickWater();
}

void MMotor::tickBaraban() {
  if (counter == boxCounter) {
    rotateLoopRunning = false;
    stop();
    statePos = true;
    return;
  }

  if (click()) counter++;
  

  if (counter < boxCounter) {
    run(dir);
    statePos = false;
  }
}
void MMotor::tickCassette() {
  if (!flagParcking) {
    if (stopSupply) {
      parckingTimer = millis();
      flagParcking = true;
      state = 1;
    }

    if (isPill()) {
      stopSupply = true;
      parckingTimer = millis();
      flagParcking = true;
      state = 1;
    }

    if (millis() - pillTimer > delayCheckPills && !stopSupply) {
      parckingTimer = millis();
      flagParcking = true;

      state = -1;
    }

    if (!stopSupply) {
      run(true);
      state = 0;
    }
  } else {
    if (millis() - parckingTimer > 2000) {
      stop();
      flagParcking = false;
      rotateLoopRunning = false;
      state = 10;
      return;
    }
    run(false);
  }
}
void MMotor::tickWater() {
  if (millis() - waterTimer > 2000) {
    stop();
    rotateLoopRunning = false;
    return;
  }
  run(true);
}

int MMotor::Array(int array[], int val) {
    for (int i=0; i<8; ++i) {
        if (array[i] == val) return i;
    }
}

void MMotor::supply(int count = 1) {
    counter = 0;
    pillCounter = count;
    pillTimer = millis();
    flagParcking = false;
    stopSupply = false;
    rotateLoopRunning = true;
}

void MMotor::parcking() {
    parckingTimer = millis();
    flagParcking = true;
    rotateLoopRunning = true;
}

int MMotor::getState_int() {
  // if (state == 1) {
  //   state = 0;
  //   return 1;
  // } else if (state == -1) {
  //   state = 0;
  //   return -1;
  // }
  return state;
}

bool MMotor::getState() {
  if (statePos) {
    statePos = false;
    return true;
  }
  else return false;
}

void MMotor::getWater() {
    waterTimer = millis();
    rotateLoopRunning = true;
}