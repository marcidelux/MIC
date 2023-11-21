#include "msgeq7_handler.hpp"

Msgeq7::Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t dataPin): _msgeq7(resetPin, strobePin, dataPin)
{
    _msgeq7.begin();
}

Msgeq7::~Msgeq7()
{
}

void Msgeq7::test()
{
    _msgeq7.read();
    for (uint8_t i=0; i<MAX_BAND; i++)
    {
      Serial.print(_msgeq7.get(i));
      Serial.print('\t');
    }
    Serial.println();
}
