#pragma once
#pragma pack(push, 1)

#include <ArduinoJson.h>
#include <AES.h>

namespace cardian::core {
struct key256 { uint8_t data[32]; };

struct fmt {
    fmt(const String &data): _data(data) {}

    operator String() const { return _data; }

    template <typename T> fmt &arg(T a) { return arg(String(a)); }
    fmt &arg(bool a) { return arg(a ? "true" : "false"); }
    fmt &arg(const String &a) {
        _data.replace("{0}", a);

        for(int i = 2; i < _data.length(); ++i) {
            if(_data[i - 2] == '{' && _data[i] == '}') {
                _data[i - 1]--;
            }
        }

        return *this;
    }

private:
    String _data;
};

struct stateType {
    uint8_t doors: 4, battery: 4, sim: 4;
    bool engine: 1, alarm : 1, gps: 1;
};

struct utils {
    /**
     * @brief
     *
     * @param data
     * @param timeout
     * @param utc
     * @return String, json string as following format.
     * {
     *      "utc": <utc>,
     *      "to": <time-out>,
     *      "d": <data-json>,
     * }
     */
    static String pack(const String &json, uint32_t timeout, uint32_t utc) {
        return fmt(R"({"utc":{0},"to":{1},"d":{2}})").arg(utc).arg(timeout).arg(json);
    }

    /**
     * @brief
     *
     * @param text
     * @param key
     * @return String
     */
    static String encryptBlock(const uint8_t *data16, const key256 &key, size_t size) {
        /// uint8_t data[32] => uint8_t schedule[32]
        uint8_t result[16];
        uint8_t data[16];
        memcpy(data, data16, size);
        static AESTiny256 aes256 = *reinterpret_cast<const AESTiny256 *>(&key);

        aes256.encryptBlock(result, data16);

        return String(reinterpret_cast<const char *>(result));
    }

    /**
     * @brief
     *
     * @param text
     * @param key
     * @return String
     */
    static String decryptBlock(const String &text, const key256 &key) {
        return text;
    }

    static uint64_t utc() {
        /// TODO: Use a RTC module to get UTC time.
    }

    /**
     * @brief 32 byte array to hex string.
     * @param key
     * @return String
     */
    static String hex(key256 key) {
        String hexString = "";

        for(uint8_t byte : key.data) {
            uint8_t left = (byte >> 4) & 0xF, right = byte & 0xF;
            hexString += String(left, 16) + String(right, 16);
        }

        return hexString;
    }

    /**
     * @brief Hex string to 32 byte array.
     * @param str
     * @return key256
     */
    static key256 unhex(const String &str) {
        key256 key{{}};
        for(int i = 0; i < 32 && (i * 2 + 1 < str.length()); i++) {
            uint8_t left = _h2v(str[2 * i]), right = _h2v(str[2 * i + 1]);
            key.data[i] = (left << 4) | right;
        }
        return key;
    }

    /**
     * @brief Hex character to value.
     * @param h
     * @return uint8_t
     */
    static inline uint8_t _h2v(char h) {
        h = tolower(h);
        return h < 97 ? h - 48 : h - 87;
    }

    static void setupTim1() {
        /// Configure idle intrrupt for every 10 s
        cli();
        TCCR1A = TCCR1B = 0;
        TCCR1B |= B00000101; // Prescalar 1024, Every 0.000128 s
        TIMSK1 |= B00000010; // Enable compare match A
        OCR1A = 78125; // Interrupt after 10 s
        sei();
    }

    static void setTim1(bool set = true) {
        TCCR1B = (set << CS10) | (set << CS12);
        TCNT1 = 0;
    }

    /**
     * @brief
     */
    static void config() {
        PCICR |= 1 << PCIE2; // PD port
        // Trigger chnage interrupts on pins D3,4,5,6
        PCMSK2 |= (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22);

        setupTim1();

        /// RF module pins
        pinMode(12, OUTPUT);
        pinMode(13, OUTPUT);
        digitalWrite(12, LOW);
        // digitalWrite(13, HIGH);

        pinMode(2, INPUT_PULLUP);
        pinMode(3, INPUT_PULLUP);
        pinMode(4, INPUT_PULLUP);
        pinMode(5, INPUT_PULLUP);
        pinMode(6, INPUT_PULLUP);
        pinMode(7, INPUT_PULLUP);
    }
};
}