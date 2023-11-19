#include "panel_handler.hpp"

PanelHandler::PanelHandler(uint8_t pin) : _strip(PANEL_PIXEL_COUNT, pin)
{
    _strip.Begin();
    _strip.Show();
    _pixels = (RgbPixel*)_strip.Pixels();
    connect_image_to_buffer();
}

PanelHandler::~PanelHandler()
{
}

void PanelHandler::SetPixelColor(uint8_t row, uint8_t col, RgbPixel pixel)
{
    if (row >= PANEL_ROWS || col >= PANEL_COLS) {
        return;
    }
    _image[row][col]->R = pixel.R;
    _image[row][col]->G = pixel.G;
    _image[row][col]->B = pixel.B;
}

void PanelHandler::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, RgbPixel color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
    int err = dx + dy, e2; 

    while (true) {
        if (x1 < PANEL_COLS && y1 < PANEL_ROWS) {
            SetPixelColor(y1, x1, color);
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void PanelHandler::DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, RgbPixel color)
{
        int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        // Draw each octant
        SetPixelColor(centerX + x, centerY + y, color);
        SetPixelColor(centerX + y, centerY + x, color);
        SetPixelColor(centerX - y, centerY + x, color);
        SetPixelColor(centerX - x, centerY + y, color);
        SetPixelColor(centerX - x, centerY - y, color);
        SetPixelColor(centerX - y, centerY - x, color);
        SetPixelColor(centerX + y, centerY - x, color);
        SetPixelColor(centerX + x, centerY - y, color);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

RgbPixel *PanelHandler::GetPixel(uint8_t row, u_int8_t col)
{
    return _image[row][col];
}

void PanelHandler::Show()
{
    _strip.Show();
}

void PanelHandler::connect_image_to_buffer() {
    uint16_t current_pixel = 0;
    for(int row = PANEL_ROWS - 1; row >= 0; row--) {
        if ((row+1) % 2 == 0) {
            for (int col = 0; col < PANEL_COLS; col++) {
                _image[row][col] = &_pixels[current_pixel];
                current_pixel++;
            }
        } else {
            for (int col = PANEL_COLS - 1; col >= 0; col--) {
                _image[row][col] = &_pixels[current_pixel];
                current_pixel++;
            }
        }
    }
}


/*
void PanelHandler::showImg16(Pixel *img)
{
    for(uint16_t i = 0; i < 256; i++) {
        strip.SetPixelColor(i, RgbColor(img[i].R, img[i].G, img[i].B));
    }

    strip.Show();
}

Pixel *PanelHandler::create16fromBytes(u_int8_t *buff)
{
    if (buff == nullptr) {
        return nullptr;
    }

    Pixel *img = new Pixel[256];
    for (uint16_t i = 0; i < 256; i++) {
        img[i].R = buff[i*3];
        img[i].G = buff[i*3] + 1;
        img[i].B = buff[i*3] + 2;
    }

    return img;
}
*/
