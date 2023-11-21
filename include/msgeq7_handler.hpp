#ifndef MSGEQ7_HANDLER
#define MSGEQ7_HANDLER

#include <MD_MSGEQ7.h>

class Msgeq7
{
    public:
    Msgeq7(uint8_t resetPin, uint8_t strobePin, uint8_t dataPin);
    ~Msgeq7();
    void test();

    private:
    MD_MSGEQ7 _msgeq7;
};

#endif //MSGEQ7_HANDLER