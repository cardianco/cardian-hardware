#pragma once

#include <string>

#include "attachable.h"

namespace cardian::core {
class serial: public attachable {
public:
    void writeln(const std::string &data) { write(data + "\n"); }
    void write(const std::string &data) {
        _UNUSED(data)
    }

    char getchar() { return '\0'; }
    std::string getline(char delimeter = '\n') {
        _UNUSED(delimeter)
        return "";
    }

    void onReadyRead(const _callback &cb) { on("readyRead", cb, event::Once); }

    bool avalible() { return false; }
};
}
