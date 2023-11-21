#if !defined(CMD_HANDLER)
#define CMD_HANDLER

#include <Arduino.h>

enum {
    CMD_WRIE_IMAGE = 0x01,
    CMD_SHOW_IMAGE = 0x02,
    CMD_DELETE_IMAGE = 0x03,
    CMD_READ_DIR = 0x04,
};

struct Command {
    uint8_t commandType;
    uint8_t parameterLength;
    uint16_t dataLength;
    uint8_t* parameter;
    uint8_t* data;
};

std::tuple<Command, uint8_t> readCommand();
void clearCmd(Command *cmd);

#endif // CMD_HANDLER