#ifndef PANEL_HANDLER
#define PANEL_HANDLER

#include <Arduino.h>
#include <NeoPixelBus.h>
#include "colors.hpp"

#define PANEL_ROWS 16
#define PANEL_COLS 16
#define PANEL_PIXEL_COUNT (PANEL_ROWS * PANEL_COLS)
#define BRIGHTNESS_MIN 0.1
#define BRIGHTNESS_MAX 1.0
#define BIRGHTNESS_DIFF (BRIGHTNESS_MAX - BRIGHTNESS_MIN)

enum Orientation {
    STARTING_POINT_TOP_LEFT_X_GOES_RIGHT_Y_GOES_DOWN,
    STARTING_POINT_TOP_LEFT_Y_GOES_RIGHT_X_GOES_DOWN,
    STARTING_POINT_TOP_RIGHT_X_GOES_LEFT_Y_GOES_DOWN,
    STARTING_POINT_TOP_RIGHT_Y_GOES_LEFT_X_GOES_DOWN,
    STARTING_POINT_BOTTOM_LEFT_X_GOES_RIGHT_Y_GOES_UP,
    STARTING_POINT_BOTTOM_LEFT_Y_GOES_RIGHT_X_GOES_UP,
    STARTING_POINT_BOTTOM_RIGHT_X_GOES_LEFT_Y_GOES_UP,
    STARTING_POINT_BOTTOM_RIGHT_Y_GOES_LEFT_X_GOES_UP
};

struct Cord {
    uint8_t x;
    uint8_t y;

    // Overload the + operator
    Cord operator+(const Cord& other) const {
        return {static_cast<uint8_t>(std::min(255, x + other.x)), 
                static_cast<uint8_t>(std::min(255, y + other.y))};
    }

    // Overload the - operator
    Cord operator-(const Cord& other) const {
        return {static_cast<uint8_t>(std::max(0, x - other.x)), 
                static_cast<uint8_t>(std::max(0, y - other.y))};
    }
    
    // Pre-increment overload
    Cord& operator++() {
        if (x < 255) x++;
        if (y < 255) y++;
        return *this;
    }

    // Post-increment overload
    Cord operator++(int) {
        Cord temp = *this;
        ++(*this); // Use the pre-increment overload
        return temp;
    }
};

typedef struct {
    Cord  cord;
    Color color;
}Pixel;

typedef struct {
    Cord  begin;
    Cord  end;
    Color color;
}Line;

typedef struct {
    Cord center;
    Color outLineColor;
    Color fillColor;
}Circle;

typedef struct {
    Cord topLeft;
    uint8_t width;
    uint8_t height;
    Color outLineColor;
}Rectangle;

struct ParticleRect
{
    uint8_t life = 0;
    Rectangle rect;
};

class PanelHandler
{
    public:
        PanelHandler(uint8_t pin, Orientation orientation);
        ~PanelHandler();
        void Clear();
        void SetPixel(Pixel &pixel);
        void SetPixelColor(Cord cord, Color pixel);
        void SetPixelColor(uint8_t x, u_int8_t y, Color pixel);
        void DrawLine(Cord cord1, Cord cord2, Color color);
        void DrawCircle(Cord cord, uint8_t radius, Color color);
        void DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, Color color);
        void DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, Color outlineColor, Color fillColor);
        void DrawCircle(Cord cord, uint8_t radius, Color outlineColor, Color fillColor);
        void DrawRectangle(Rectangle &rect);
        void DrawSquare(Cord center, uint8_t size, Color color);
        void DrawFilledSquare(Cord center, uint8_t size, Color color);
        Color* GetPixel(uint8_t row, u_int8_t col);
        void SetStripBuffer(uint8_t *buff); // replace values of buff to _pixels.
        void Show();
        void SetBrightness(float brightness);
        NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1X8Ws2812xMethod> _strip;
    private:
        Color *_image[PANEL_ROWS][PANEL_COLS];
        Color *_pixels;
        void connect_image_to_buffer(Orientation orientation);
        Rectangle cornerRect{{0, 0}, 15, 15, {0,0,0}};
        uint8_t prevScaledIntensity = 0;
        float brightness_ = 1.0;
};

#endif