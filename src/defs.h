//
// Created by Nikit on 22.08.2024.
//

#ifndef MEDIBOX_DEFS_H
#define MEDIBOX_DEFS_H

// Константы скорости
#define NORMAL_SPEED 255
#define SLOW_SPEED 150

// Константы времени
#define delayWater 3000        // Время на нали воды в стака
#define delayCheckPills 10000  // Время на проверку наличия таблеток в боксе
#define delayNeedWater 10000   // Время на кнопку при выдаче воды
#define delayNeedPills 20000   // Время на кнопку при выдаче таблеток

// Войсы [ РУССКИЕ ]
#define NotGlassP 0    // Стакан таблеток не на месте, поставьте его пожалуйста)
#define NotGlassW 1    // Стакан воды не на месте, поставьте его пожалуйста)
#define TakeGlassP 2   // Возьмите пожалуйста стакан таблеток
#define TakeGlassW 3   // Возьмите пожалуйста стакан воды
#define NotPills 4     // В боксе нет таблеток
#define NeedWater 5    // Если вам нужна вода, нажмите на кнопку
#define NeedPills1 6   // Если вы не сможете принять таблетки, возьмите их се` йчас
#define Users1 7       // Первый пользователь, подойдите к таблетнице
#define Users2 8       // Второй пользователь, подойдите к таблетнице
#define Users3 9       // Третий пользователь, подойдите к таблетнице
#define UserWrong 10   // Этого пользователя нет в базе
#define NeedPills2 11  // Закиньте таблеточки в пакетик и вперёд отсюда

#define VOLUME 20

#define MAX_BOX 8      // Максимальное количество контейнеров

// Углы для сервомотора
#define ANGLE_UP 160   // Верхний
#define ANGLE_DOWN 85  // Нижний


// Коневик на контейнер
#define endStop 11

// Подключение помпы
#define WATER_PIN 23

// Подключение датчиков линии
#define WATER_GLASS 37  // Стакан воды
#define PILL_GLASS 38   // Стакан таблеток
#define HOME_POINT 36   // Домашняя точка
#define CHECK_PILL 35   // Датчик таблетки

#endif //MEDIBOX_DEFS_H
