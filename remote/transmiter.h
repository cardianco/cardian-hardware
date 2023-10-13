#pragma once
#pragma pack(push, 1)
#define __AVR_ATmega328P__
#include <Arduino.h>

#include <SoftwareSerial.h>
#include <jsonlib.h>

#include "core.h"

namespace cardian::remote {
class transmiter {
public:
    transmiter(core::key256 key, uint8_t rx = 11, uint8_t tx = 10): mRf(rx, tx), mTimeout(3000), mKey(key) {}
    transmiter(uint8_t rx = 11, uint8_t tx = 10): transmiter({}, rx, tx) {}

    void begin(long speed = 9600) {
        mRf.begin(speed);
    }

    /// @brief Read data from reciver module
    bool read(core::stateType & state) {
        if(mRf.available()) {
            String data = mRf.readStringUntil('\n');

            if(data == R"({"d":"ok"})"){
                return false;
            } else if(data.length()) {
                parseData(data, state);
                return true;
            }
        }

        return false;
    }

    void parseData(const String &data, core::stateType &state) {
        if(data.indexOf("doors") != -1) {
            state.doors = jsonExtract(data, "doors").toInt();
        } if(data.indexOf("sim") != -1) {
            state.sim = jsonExtract(data, "sim").toInt();
        } if(data.indexOf("battery") != -1) {
            state.battery = jsonExtract(data, "battery").toInt();
        } if(data.indexOf("gps") != -1) {
            state.gps   = jsonExtract(data, "gps").toInt();
        } if(data.indexOf("alarm") != -1) {
            state.alarm = jsonExtract(data, "alarm").toInt();
        } if(data.indexOf("engine") != -1) {
            state.engine = jsonExtract(data, "engine").toInt();
        }
    }

    /**
     * @brief
     * @param json
     */
    void sendCommand(const String &json) {
        uint64_t utc = core::utils::utc();
        String data = core::utils::pack(json, mTimeout, utc);
        mRf.println(data);
        // streamEncrypte(data);
    }

    void streamEncrypte(const String &text) {
        for(int i = 0; i < text.length(); i += 16) {
            String block = core::utils::encryptBlock(reinterpret_cast<const uint8_t *>(text.begin() + i), mKey, min(text.length() - i, 16));
            mRf.print(block);
        }
        mRf.println();
    }

    /// @brief
    void ping() {
        String msg = String(R"({"d":"ping"})");
        mRf.println(msg);
        // streamEncrypte(msg);
    }

// private:
    SoftwareSerial mRf;
    uint32_t mTimeout;
    core::key256 mKey;
    bool mOk: 1;
};
}