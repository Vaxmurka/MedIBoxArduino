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
}

void Finger::init() {
  Serial.println("Finger init");
  fp.begin(57600);

  if (!fp.verifyPassword()) {
      Serial.println(F("Не найден датчик отпечатков пальцев :("));
      led->error(1);
  }
  Serial.println(F("Все ок, работаем дальше!"));

  fp.getTemplateCount();
  id = fp.templateCount + 1;

  led->setColor(CYAN);
}

void Finger::setEvent(State _state, int mode = 0)  {
  state = _state;

  switch(state) {
    case READ: 
      targetID = mode;
      scanLoop = true;
      readFlag = true;
      writeFlag = false;
      break;
    case WRITE: 
      scanLoop = true;
      writeFlag = true;
      readFlag = false;
      break;
    case CLEAR: 
      clearData();
      break;
  }
}

void Finger::scan() {
  if (!scanLoop) {
    return;
  }

  if (readFlag) {
    if (millis() - loopTime >= 10) {
      loopTime = millis();
      readFinger();
      // VAR_this_ID = getFingerprintID();
    }
  }
  if (writeFlag) {
    Serial.println(F("Сканер готов зарегистрировать отпечаток пальца!"));
    while (!  getFingerprintEnroll() );
    writeFlag = false;
  }

}

int Finger::getID() {
  return VAR_this_ID;
}
void Finger::clearID() {
  VAR_this_ID = 0;
}

void Finger::readFinger() {
  Serial.println(F("Чтение отпечатка пальца"));
  if (fp.getImage() == FINGERPRINT_OK) {
    if (fp.image2Tz() == FINGERPRINT_OK) {
      if (fp.fingerFastSearch() == FINGERPRINT_OK) {    //fingerSearch
        VAR_this_ID = fp.fingerID;
        if (VAR_this_ID == targetID) {
          Serial.println(F("Найдено совпадение по отпечатку!"));
          Serial.println("ID fp: " + String(VAR_this_ID));
          led->blink(GREEN);
        } else {
          Serial.println(F("NE Найдено совпадение по отпечатку!"));
          led->error(1);
          VAR_this_ID = -1;
        }
        scanLoop = false;
        readFlag = false;
        return;
      } else {
        scanLoop = false;
        readFlag = false;
        led->error(1);
        VAR_this_ID = -1;
        return;
      }
    }
  }
}

uint8_t Finger::getFingerprintID() {
  uint8_t p = fp.getImage();
  if (p == FINGERPRINT_OK) {

    p = fp.image2Tz();
    if(p == FINGERPRINT_OK) {

      scanLoop = false;
      readFlag = false;
      p = fp.fingerSearch();
      if (p == FINGERPRINT_OK) {

        Serial.print(F("Найденный идентификатор #")); Serial.print(fp.fingerID);
        Serial.print(F(" с уверенностью в ")); Serial.println(fp.confidence);
        led->blink(GREEN);
        return fp.fingerID;
      } else {
        led->error(1);
        return -1;
      }
    }
  } else {
    scanLoop = false;
    readFlag = false;
    Serial.println(F("Неопознанная ошибка"));
    led->error(2);
    return -1;
  }
}

//***************** Запись отпечатков в сканер ***************************

uint8_t Finger::getFingerprintEnroll() {
  int p = -1;
  Serial.print(F("Ожидание действительного пальца для регистрации в качестве #")); Serial.println(id);
  led->setColor(BLUE);
  while (p != FINGERPRINT_OK) {
     p = fp.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println(F("Изображение отпечатка снято"));
        led->setColor(BLUE);
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println(F("Ошибка связи"));
        led->error(2);
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println(F("Ошибка изображения"));
        led->error(2);
        break;
      default:
        Serial.println(F("Неопознанная ошибка"));
        led->error(2);
        break;
    }
  }

  // Успешно!

  p = fp.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Изображение преобразовано"));
      led->setColor(BLUE);
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Изображение слишком нечеткое"));
      led->error(2);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Ошибка связи"));
      led->error(2);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Не удалось найти признаки отпечатка пальца"));
      led->error(2);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Не удалось найти признаки отпечатка пальца"));
      led->error(2);
      return p;
    default:
      Serial.println(F("Неопознанная ошибка"));
      led->error(2);
      return p;
  }

  Serial.println(F("Уберите палец со сканера"));
   p = 0;
  while (p != FINGERPRINT_OK) {
    p = fp.getImage();
  }
    p = -1;
    Serial.println(F("Снова приложите тот же палец"));
  while (p != FINGERPRINT_OK) {
    p = fp.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println(F("Сканированное изображение"));
        break;
      case FINGERPRINT_NOFINGER:
        led->setColor(WHITE);
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println(F("Ошибка связи"));
        led->error(2);
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println(F("Ошибка изображения"));
        led->error(2);
        break;
      default:
        Serial.println(F("Неопознанная ошибка"));
        led->error(2);
        break;
    }
  }

  // Успешно!

  p = fp.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Изображение преобразовано"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Изображение слишком нечеткое"));
      led->error(2);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Ошибка связи"));
      led->error(2);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Не удалось найти признаки отпечатка пальца"));
      led->error(2);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Не удалось найти признаки отпечатка пальца"));
      led->error(2);
      return p;
    default:
      Serial.println(F("Неопознанная ошибка"));
      led->error(2);
      return p;
  }

  // Успешно преобразовано!
  Serial.print(F("Создание модели для #"));  Serial.println(id);

  p = fp.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Отпечатки совпали!"));
    led->setColor(GREEN);
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Ошибка связи"));
    led->error(2);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println(F("Отпечатки пальцев не совпали"));
    led->setColor(RED);
    delay(2000);
    return p;
  } else {
    Serial.println(F("Неопознанная ошибка"));
    led->error(2);
    return p;
  }


  p = fp.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Сохранено!"));
    id++;  
    led->setColor(BLUE); 
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Ошибка связи"));
    led->error(2);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println(F("Не удалось сохранить в этом месте"));
    led->error(2);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println(F("Ошибка записи во flash"));
    led->error(2);
    return p;
  } else {
    Serial.println(F("Неопознанная ошибка"));
    led->error(2);
    return p;
  }
  p=-1;
  return true;
}

void Finger::clearData() {
  fp.emptyDatabase();
  led->blink(GREEN);
  id = 1;
  VAR_this_ID = 0;
}
