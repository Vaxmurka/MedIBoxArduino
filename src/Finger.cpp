//
// Created by Nikit on 22.08.2024.
//
#include <Arduino.h>
#include "Finger.h"
#include "Adafruit_Fingerprint.h"

#define FP_Serial Serial1
Adafruit_Fingerprint fp = Adafruit_Fingerprint(&FP_Serial);

Finger::Finger(LED* _led) {
    led = _led;
    Serial.println("Finger create");
}

void Finger::init() {
    Serial.println("Finger init");
    fp.begin(57600);

    if (!fp.verifyPassword()) {
        Serial.println(F("Не найден датчик отпечатков пальцев :("));
        led->error(1);
    }
}
