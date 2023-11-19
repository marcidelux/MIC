#ifndef IMG_HANDLER
#define IMG_HANDLER

#include <Arduino.h>
#include <NeoPixelBus.h>

extern NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1X8Ws2812xMethod> strip;

struct Pixel {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

class ImgHandler
{
    public:
        ImgHandler();
        ~ImgHandler();
        void showImg16(Pixel *img);
        Pixel* create16fromBytes(u_int8_t* buff);
};

#endif