#include "img_handler.hpp"

ImgHandler::ImgHandler()
{
}

ImgHandler::~ImgHandler()
{
}

void ImgHandler::showImg16(Pixel *img)
{
    for(uint16_t i = 0; i < 256; i++) {
        strip.SetPixelColor(i, RgbColor(img[i].R, img[i].G, img[i].B));
    }

    strip.Show();
}

Pixel *ImgHandler::create16fromBytes(u_int8_t *buff)
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
