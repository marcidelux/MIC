#ifndef FS_HANDLER
#define FS_HANDLER

#include "img_handler.hpp"

#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED false

class FsHandler
{
    public:
        FsHandler();
        ~FsHandler();
        uint8_t init();
        //void listDir(const char * dirname, uint8_t levels);
        Pixel *readImg16x16(const char * path);
        uint8_t writeImg16X16(const char * path, const Pixel *img);
};

#endif  // FS_HANDLER