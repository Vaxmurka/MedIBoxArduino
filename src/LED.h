//
// Created by Nikit on 22.08.2024.
//

#ifndef MEDIBOX_LED_H
#define MEDIBOX_LED_H

enum Color {
    RED,
    GREEN,
    BLUE,
    PURPLE,
    CYAN,
    WHITE,
    None
};


class LED {
public:
    LED();
    void begin();
    void tick();

    void setColor(Color _color);
    void off();

    void click();
    void error(int state);
    void blink(Color _color);

private:
    Color color;
    int pinRed;
    int pinGreen;
    int pinBlue;

    unsigned long tmr;
    int stateError, count;

    bool ledLoopRunning = false;
    bool clickFlag = false, errorFlag = false, blinkFlag = false, SW;
};


#endif //MEDIBOX_LED_H
