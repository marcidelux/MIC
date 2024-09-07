#include "panel_handler.hpp"
#include "stdlib.h"

PanelHandler::PanelHandler(uint8_t pin, Orientation orientation) : _strip(PANEL_PIXEL_COUNT, pin)
{
    _strip.Begin();
    _strip.Show();
    _pixels = (Color*)_strip.Pixels();
    connect_image_to_buffer(orientation);
}

PanelHandler::~PanelHandler()
{
}

void PanelHandler::Clear()
{
    _strip.ClearTo(RgbColor{0,0,0});
}

void PanelHandler::SetPixel(Pixel &pixel)
{
    if (pixel.cord.x >= PANEL_ROWS  || pixel.cord.y >= PANEL_COLS) {
        return;
    }

    _image[pixel.cord.x][pixel.cord.y]->R = pixel.color.G;
    _image[pixel.cord.x][pixel.cord.y]->G = pixel.color.R;
    _image[pixel.cord.x][pixel.cord.y]->B = pixel.color.B;
}

void PanelHandler::SetPixelColor(Cord cord, Color pixel)
{
    if (cord.x >= PANEL_ROWS || cord.y >= PANEL_COLS) {
        return;
    }
    _image[cord.x][cord.y]->R = pixel.G;
    _image[cord.x][cord.y]->G = pixel.R;
    _image[cord.x][cord.y]->B = pixel.B;
}

void PanelHandler::SetPixelColor(uint8_t x, u_int8_t y, Color pixel)
{
    if (x >= PANEL_ROWS || y >= PANEL_COLS) {
        return;
    }
    _image[x][y]->R = pixel.G;
    _image[x][y]->G = pixel.R;
    _image[x][y]->B = pixel.B;
}

void PanelHandler::DrawLine(Cord cord1, Cord cord2, Color color) {
    int dx = abs(cord2.x - cord1.x), sx = cord1.x < cord2.x ? 1 : -1;
    int dy = -abs(cord2.y - cord1.y), sy = cord1.y < cord2.y ? 1 : -1; 
    int err = dx + dy, e2; 

    while (true) {
        if (cord1.x < PANEL_COLS && cord1.y < PANEL_ROWS) {
            SetPixelColor(cord1, color);
        }
        if (cord1.x == cord2.x && cord1.y == cord2.y) break;
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            cord1.x += sx;
        }
        if (e2 <= dx) {
            err += dx;
            cord1.y += sy;
        }
    }
}

void PanelHandler::DrawCircle(Cord cord, uint8_t radius, Color color)
{
    DrawCircle(cord.x, cord.y, radius, color);
}

void PanelHandler::DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, Color color)
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

void PanelHandler::DrawCircle(uint8_t centerX, uint8_t centerY, uint8_t radius, Color outlineColor, Color fillColor) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        for (uint8_t i = centerX - x; i <= centerX + x; i++) {
            SetPixelColor(i, centerY - y, fillColor);
            SetPixelColor(i, centerY + y, fillColor);
        }
        for (uint8_t i = centerX - y; i <= centerX + y; i++) {
            SetPixelColor(i, centerY + x, fillColor);
            SetPixelColor(i, centerY - x, fillColor);
        }

        // Draw each octant
        SetPixelColor(centerX + x, centerY + y, outlineColor);
        SetPixelColor(centerX + y, centerY + x, outlineColor);
        SetPixelColor(centerX - y, centerY + x, outlineColor);
        SetPixelColor(centerX - x, centerY + y, outlineColor);
        SetPixelColor(centerX - x, centerY - y, outlineColor);
        SetPixelColor(centerX - y, centerY - x, outlineColor);
        SetPixelColor(centerX + y, centerY - x, outlineColor);
        SetPixelColor(centerX + x, centerY - y, outlineColor);

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

void PanelHandler::DrawCircle(Cord cord, uint8_t radius, Color outlineColor, Color fillColor) {
    DrawCircle(cord.x, cord.y, radius, outlineColor, fillColor);
}

void PanelHandler::DrawRectangle(Rectangle &rect)
{
    Cord topRight{rect.topLeft.x + rect.width, rect.topLeft.y};
    Cord bottomRight{rect.topLeft.x + rect.width, rect.topLeft.y + rect.height};
    Cord bottomLeft{rect.topLeft.x, rect.topLeft.y + rect.height};

    DrawLine(rect.topLeft, topRight, rect.outLineColor);
    DrawLine(topRight, bottomRight, rect.outLineColor);
    DrawLine(bottomRight, bottomLeft, rect.outLineColor);
    DrawLine(bottomLeft, rect.topLeft, rect.outLineColor);
}

void PanelHandler::DrawSquare(Cord center, uint8_t size, Color color)
{
    // Calculate top-left corner based on the center and size
    Cord topLeft{
        static_cast<uint8_t>(center.x - size / 2),
        static_cast<uint8_t>(center.y - size / 2)
    };

    // Create a rectangle with equal width and height
    Rectangle square = {topLeft, size, size, color};

    // Draw the square using the DrawRectangle function
    DrawRectangle(square);
}

void PanelHandler::DrawFilledSquare(Cord center, uint8_t size, Color color)
{
    // Calculate top-left corner based on the center and size
    int halfSize = size / 2;
    int startX = static_cast<int>(center.x) - halfSize;
    int startY = static_cast<int>(center.y) - halfSize;
    int endX = static_cast<int>(center.x) + halfSize;
    int endY = static_cast<int>(center.y) + halfSize;

    // Ensure boundaries are respected (if needed)
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX >= PANEL_ROWS) endX = PANEL_ROWS - 1;
    if (endY >= PANEL_COLS) endY = PANEL_COLS - 1;

    // Loop through each pixel in the square and set its color
    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            Cord pixelCord = {static_cast<uint8_t>(x), static_cast<uint8_t>(y)};
            SetPixelColor(pixelCord, color);
        }
    }
}

Color *PanelHandler::GetPixel(uint8_t row, u_int8_t col)
{
    return _image[row][col];
}

void PanelHandler::SetStripBuffer(uint8_t *buff)
{
    memcpy(_pixels, buff, PANEL_PIXEL_COUNT * 3);
}

void PanelHandler::Show()
{
    for(uint16_t i = 0; i < PANEL_PIXEL_COUNT; i++)
    {
        _pixels[i] *= brightness_;
    }

    _strip.Show();
}

void PanelHandler::SetBrightness(float brightness)
{
    if (brightness <= 0 || brightness >= 1.0) {
        return;
    }
    brightness_ = brightness;
}

void PanelHandler::connect_image_to_buffer(Orientation orientation) {
    uint16_t current_pixel = 0;

      switch (orientation) {
        case STARTING_POINT_TOP_LEFT_X_GOES_RIGHT_Y_GOES_DOWN:
            for(int row = 0; row < PANEL_ROWS; row++) {
                if (row % 2 == 0) {
                    for (int col = PANEL_COLS - 1; col >= 0; col--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int col = 0; col < PANEL_COLS; col++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_TOP_LEFT_Y_GOES_RIGHT_X_GOES_DOWN:
            for(int col = 0; col < PANEL_COLS; col++) {
                if (col % 2 == 0) {
                    for (int row = 0; row < PANEL_ROWS; row++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int row = PANEL_ROWS - 1; row >= 0; row--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_TOP_RIGHT_X_GOES_LEFT_Y_GOES_DOWN:
            for(int row = 0; row < PANEL_ROWS; row++) {
                if (row % 2 == 0) {
                    for (int col = 0; col < PANEL_COLS; col++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int col = PANEL_COLS - 1; col >= 0; col--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_TOP_RIGHT_Y_GOES_LEFT_X_GOES_DOWN:
            for(int col = PANEL_COLS - 1; col >= 0; col--) {
                if (col % 2 == 0) {
                    for (int row = 0; row < PANEL_ROWS; row++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int row = PANEL_ROWS - 1; row >= 0; row--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_BOTTOM_LEFT_X_GOES_RIGHT_Y_GOES_UP:
            for(int row = PANEL_ROWS - 1; row >= 0; row--) {
                if (row % 2 == 0) {
                    for (int col = PANEL_COLS - 1; col >= 0; col--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int col = 0; col < PANEL_COLS; col++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_BOTTOM_LEFT_Y_GOES_RIGHT_X_GOES_UP:
            for(int col = 0; col < PANEL_COLS; col++) {
                if (col % 2 == 0) {
                    for (int row = PANEL_ROWS - 1; row >= 0; row--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int row = 0; row < PANEL_ROWS; row++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_BOTTOM_RIGHT_X_GOES_LEFT_Y_GOES_UP:
            for(int row = PANEL_ROWS - 1; row >= 0; row--) {
                if (row % 2 == 0) {
                    for (int col = 0; col < PANEL_COLS; col++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int col = PANEL_COLS - 1; col >= 0; col--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        case STARTING_POINT_BOTTOM_RIGHT_Y_GOES_LEFT_X_GOES_UP:
            for(int col = PANEL_COLS - 1; col >= 0; col--) {
                if (col % 2 == 0) {
                    for (int row = PANEL_ROWS - 1; row >= 0; row--) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                } else {
                    for (int row = 0; row < PANEL_ROWS; row++) {
                        _image[col][row] = &_pixels[current_pixel++];
                    }
                }
            }
            break;

        default:
            // Handle invalid orientation case
            break;
    }
}