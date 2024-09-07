#include "msgeq7_handler.hpp"

Msgeq7::Msgeq7()
{
    msgeq7_.begin();
    beat = false;
}

Msgeq7::~Msgeq7()
{

}

void Msgeq7::Measure()
{
    msgeq7_.read();

    for (uint8_t i = 0; i < 7; i++)
    {
        uint16_t value = static_cast<uint16_t>(msgeq7_.get(i) * sensitivity_);
        if (value > 255)
        {
            value = 255;
        }
        values[i] = static_cast<uint8_t>(value);
    }

    // Get overall volume
    uint8_t rawVolume = msgeq7_.getVolume();

    // Adjust volume based on sensitivity
    uint16_t adjustedVolume = static_cast<uint16_t>(rawVolume * sensitivity_);
    if (adjustedVolume > 255) {
        adjustedVolume = 255;
    }
    volume = static_cast<uint8_t>(adjustedVolume);
/*
    // Calculate the average of the last NUM_SAMPLES samples
    uint32_t sum = 0;
    for (uint8_t j = 0; j < NUM_SAMPLES; j++) {
        sum += history_[j];
    }
    float averageVolume = sum / static_cast<float>(NUM_SAMPLES);
    float peakValueF = averageVolume + (averageVolume * BEAT_THRESHOLD);
    uint8_t peakValue = static_cast<uint8_t>(peakValueF);
    if (peakValue >= 254) {
        peakValue = 255;
    }

    // Cooldown logic with counter
    static uint8_t cooldownCounter = 0;  // Cooldown counter in terms of function calls
    const uint8_t cooldownLimit = 20;    // 200ms cooldown (100 calls per second)

    // Detect beat with cooldown period and history exclusion for peaks
    if (cooldownCounter == 0 && rawVolume > peakValue) {
        beat = true;  // A beat is detected
        cooldownCounter = cooldownLimit;  // Start cooldown

        // Skip adding the peak to history, continue to the next cycle
    } else {
        // If no beat detected, add current volume to history
        history_[historyIndex_] = rawVolume;
    }

    // Decrease the cooldown counter until it reaches 0
    if (cooldownCounter > 0) {
        cooldownCounter--;
    }

    // Update the history index for the next cycle
    historyIndex_ = (historyIndex_ + 1) % NUM_SAMPLES;
*/
}



void Msgeq7::SetSensitivity(float sensitivity)
{
  if(sensitivity <= 0.1 || sensitivity >= 2.0) {
    return;
  }
  
  sensitivity_ = sensitivity;
}
