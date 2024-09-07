#include <stdint.h>

struct Color
{
    uint8_t R;
    uint8_t G;
    uint8_t B;

    Color& operator*=(float scale) {
        R = static_cast<uint8_t>(R * scale);
        G = static_cast<uint8_t>(G * scale);
        B = static_cast<uint8_t>(B * scale);
        return *this;
    }
};

inline Color HSVtoRGB(float h, float s, float v) {
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    float r, g, b;
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    Color color;
    color.R = static_cast<uint8_t>(r * 255);
    color.G = static_cast<uint8_t>(g * 255);
    color.B = static_cast<uint8_t>(b * 255);
    
    return color;
}

const Color BLACK{0,0,0};
const Color RED{255,0,0};
const Color YELLOW{255,255,0};
const Color GREEN{0,255,0};
const Color CYAN{0,255,255};
const Color BLUE{0,0,255};
const Color MAGENTA{255,0,255};
const Color WHITE{255,0,255};

const Color  BASE_COLORS[7] = {RED,YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE};