#include "cmd_handler.hpp"

std::tuple<Command, uint8_t> readCommand() {
    Command cmd;

    if (Serial.available() >= 4) {  // Make sure 4 bytes are available
        cmd.commandType = Serial.read();
        Serial.print("Command Type: "); Serial.println(cmd.commandType);

        cmd.parameterLength = Serial.read();
        Serial.print("Parameter Length: "); Serial.println(cmd.parameterLength);

        cmd.dataLength = Serial.read();
        cmd.dataLength |= Serial.read() << 8;
        Serial.print("Data Length: "); Serial.println(cmd.dataLength);

        if (cmd.parameterLength > 0) {
            cmd.parameter = new uint8_t[cmd.parameterLength];
            Serial.readBytes(cmd.parameter, cmd.parameterLength);
        }

        if (cmd.dataLength > 0) {
            cmd.data = new uint8_t[cmd.dataLength];
            Serial.readBytes(cmd.data, cmd.dataLength);
        }

        Serial.flush();
        return std::make_tuple(cmd, 0);
    }

    return std::make_tuple(cmd, 1);
}

void clearCmd(Command *cmd)
{
    if(cmd->dataLength) {
        delete[] cmd->data;
    }
    if(cmd->parameterLength) {
        delete[] cmd->parameter;
    }
}
