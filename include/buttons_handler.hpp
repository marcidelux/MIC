#include <Arduino.h>

#define BUTTON_RED_PIN      26
#define BUTTON_YELLOW_PIN   25
#define BUTTON_BLUE_PIN     13
#define BUTTON_GREEN_PIN    12 
#define POTMETER_RED_PIN    14
#define POTMETER_BLUE_PIN   27

#define BUTTON_RED          0
#define BUTTON_YELLOW       1
#define BUTTON_BLUE         2
#define BUTTON_GREEN        3
#define POTMETER_RED        0
#define POTMETER_BLUE       1

enum BUTTON_STATE {
    BT_STATE_STEADY,
    BT_STATE_PRESSED,
    BT_STATE_RELEASED,
};

class ButtonsHandler
{
public:
    ButtonsHandler();
    ~ButtonsHandler();
    bool Measure();  //return stroeu if something hapend
    bool potmeterChange[2];
    uint8_t potmeters[2];
    BUTTON_STATE buttons[4];
private:
    BUTTON_STATE currentStates_[4];
    BUTTON_STATE previousStates_[4];
    uint8_t previousPotmeters[2];
};
