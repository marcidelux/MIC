#ifndef MSGEQ7_HANDLER
#define MSGEQ7_HANDLER

#include <MSGEQ7.h>

#define MSGEQ7_ANALOG_PIN 15
#define MSGEQ7_STROBE_PIN 16
#define MSGEQ7_RESET_PIN  17
#define MSGEQ7_SMOOTH     100

#define SENSITIVITY_MIN   0.1
#define SENSITIVITY_MAX   1.5
#define SENSITIVITY_DIFF  (SENSITIVITY_MAX - SENSITIVITY_MIN)
#define BEAT_THRESHOLD    1.2   // Threshold to determine a beat (1.4 times the average)
#define NUM_SAMPLES       20    // Store the last 20 samples per frequency

class Msgeq7
{
    public:
        Msgeq7();
        ~Msgeq7();
        void Measure();
        void SetSensitivity(float sensitivity);
        uint8_t values[7];
        uint8_t volume;
        bool beat;
        
    private:
        CMSGEQ7<MSGEQ7_SMOOTH, MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_ANALOG_PIN> msgeq7_;
        float sensitivity_ = 1.0;
        uint8_t history_[NUM_SAMPLES]; 
        uint8_t historyIndex_ = 0;
};

#endif //MSGEQ7_HANDLER