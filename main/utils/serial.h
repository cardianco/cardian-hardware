#pragma once

#include <string>

#include "attachable.h"
#include "../mcu/usart.h"

namespace cardian::core {

template<uint8_t usart_idx = 0>
class serial: public attachable {
public:
    serial() {
        usartHelper::usartInit<usart_idx>();
    }

    uint32_t write(const std::string &str) {
        return usartHelper::write<usart_idx>(str);
    }

    template <typename T>
    uint32_t print(T data) {
        if constexpr (std::is_same_v<T, bool>) {
            return usartHelper::write<usart_idx>(data ? "true" : "false");
        } else if constexpr (std::is_same_v<T, char>) {
            return usartHelper::write<usart_idx>(data);
        } else if constexpr (std::is_arithmetic_v<T>) {
            return usartHelper::write<usart_idx>(std::to_string(data));
        } else {
            return usartHelper::write<usart_idx>(data);
        }
    }

    template <typename T>
    uint32_t println(const T &data) {
        uint32_t size = print(data);
        usartHelper::write<usart_idx>('\n');
        return size + 1;
    }

    std::string read() {
        return usartHelper::readAll<usart_idx, true>();
    }

    std::string read(char delim) {
        return usartHelper::readUntil<usart_idx, true>(delim);
    }

    std::string readLine() {
        return usartHelper::readUntil<usart_idx, true>('\n');
    }

    std::string readChar() {
        return usartHelper::readAll<usart_idx, true>();
    }

    void onReadyRead(const _callback &cb) { on("readyRead", cb, event::Once); }

    bool available() {
        return usartHelper::avaliable<usart_idx>() || usartHelper::buf().length();
    }

    void eventLoop() {
        if(available()) emit("readyRead");
    }
};

struct serial1: public serial<0> {};
struct serial2: public serial<1> {};
struct serial3: public serial<2> {};
}
