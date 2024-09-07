#include "buttons_handler.hpp"

ButtonsHandler::ButtonsHandler()
{
    pinMode(BUTTON_RED_PIN, INPUT);
    pinMode(BUTTON_YELLOW_PIN, INPUT);
    pinMode(BUTTON_BLUE_PIN, INPUT);
    pinMode(BUTTON_GREEN_PIN, INPUT);

    for (size_t i = 0; i < 4; i++)
    {
        buttons[i] = BT_STATE_STEADY;
    }

    potmeterChange[POTMETER_RED] = false;
    potmeterChange[POTMETER_BLUE] = false;
}

ButtonsHandler::~ButtonsHandler()
{
}

bool ButtonsHandler::Measure()
{
    bool eventFlag = false;
    const uint8_t TOLERANCE = 10;  // Define a tolerance of +/-5

    // Read and map potentiometer values (map 0-4095 to 0-255)
    uint8_t currentPotmeterRed = map(analogRead(POTMETER_RED_PIN), 0, 4095, 0, 255);
    uint8_t currentPotmeterBlue = map(analogRead(POTMETER_BLUE_PIN), 0, 4095, 0, 255);

    // Check for potentiometer changes with a tolerance and compare to the public potmeters[]
    if (abs(currentPotmeterRed - potmeters[POTMETER_RED]) > TOLERANCE) {
        potmeterChange[POTMETER_RED] = true;
        eventFlag = true;
        potmeters[POTMETER_RED] = currentPotmeterRed;
    } else {
        potmeterChange[POTMETER_RED] = false; 
    }

    if (abs(currentPotmeterBlue - potmeters[POTMETER_BLUE]) > TOLERANCE) {
        potmeterChange[POTMETER_BLUE] = true;
        potmeters[POTMETER_BLUE] = currentPotmeterBlue;
        eventFlag = true;
    } else {
        potmeterChange[POTMETER_BLUE] = false; 
    }

    // Read current button states
    currentStates_[BUTTON_RED] = digitalRead(BUTTON_RED_PIN) ? BT_STATE_PRESSED : BT_STATE_STEADY;
    currentStates_[BUTTON_YELLOW] = digitalRead(BUTTON_YELLOW_PIN) ? BT_STATE_PRESSED : BT_STATE_STEADY;
    currentStates_[BUTTON_BLUE] = digitalRead(BUTTON_BLUE_PIN) ? BT_STATE_PRESSED : BT_STATE_STEADY;
    currentStates_[BUTTON_GREEN] = digitalRead(BUTTON_GREEN_PIN) ? BT_STATE_PRESSED : BT_STATE_STEADY;

    // Compare current states with previous states and update buttons[] array
    for (int i = 0; i < 4; i++) {
        if (previousStates_[i] == BT_STATE_PRESSED && currentStates_[i] == BT_STATE_STEADY) {
            // Button was pressed and now released
            buttons[i] = BT_STATE_RELEASED;
            eventFlag = true;
        } else if (previousStates_[i] == BT_STATE_RELEASED) {
            buttons[i] = BT_STATE_STEADY;
        } else {
            buttons[i] = currentStates_[i];
        }
        previousStates_[i] = currentStates_[i];
    }

    return eventFlag;
}
