#ifndef FS_HANDLER
#define FS_HANDLER

#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED false
#define BUFF_SIZE_16X16 768

class FsHandler
{
    public:
        FsHandler();
        ~FsHandler();
        uint8_t init();
        uint8_t WriteData(const char* path, uint8_t *buffer);
        uint8_t ReadData(const char* path);
        String ReadDirectory(const char* dir);
        uint8_t RemoveImage(const char* path);
        uint8_t *GetBuffer16x16();
    private:
        uint8_t _buffer_16x16[BUFF_SIZE_16X16];
};

#endif  // FS_HANDLER