#ifndef PANEL_HANDLER
#define PANEL_HANDLER

#include <Arduino.h>
#include <NeoPixelBus.h>

#define PANEL_ROWS 16
#define PANEL_COLS 16
#define PANEL_PIXEL_COUNT (PANEL_ROWS * PANEL_COLS)

struct RgbPixel
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

class PanelHandler
{
    public:
        PanelHandler(uint8_t pin);
        ~PanelHandler();
        void SetPixelColor(uint8_t row, uint8_t col, RgbPixel pixel);
        void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, RgbPixel color);
        RgbPixel* GetPixel(uint8_t row, u_int8_t col);
        void Show();
        NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1X8Ws2812xMethod> _strip;
    private:
        RgbPixel *_image[PANEL_ROWS][PANEL_COLS];
        RgbPixel *_pixels;
        void connect_image_to_buffer();
};

#endif