#pragma once
#pragma pack(push, 1)

#include <Arduino.h>
#include <U8g2lib.h>

#include "core.h"
#include "icons.h"
#include "helper.h"

namespace cardian::remote {
class display {
public:
    display(): mActive(true), mOled{U8G2_R0} {};

    /**
     * @brief Initialize the OLED display.
     * @note This function should be called during setup.
     */
    void init() {
        mOled.begin();
    }

    /// @brief Display the splash screen.
    void splash() {
        mOled.clearBuffer(); // Clear the internal memory

        helper::print(mOled, 27,  3,  "Cardian");
        helper::print(mOled, 30, 15, "remote v0.1");

        helper:: drawIcon(mOled, 0, 3, icons::logo);

        mOled.sendBuffer();
    }

    /// @brief Update the display.
    void update(const char *text = nullptr) {
        mOled.clearBuffer(); // Clear the internal memory

        helper::drawBattery(mOled, 3, 4, mBattery);
        helper::drawStatuses(mOled, 27, 3, mGps, mAlarm, mEngine, mSim);
        helper::drawCar(mOled, 97, 0, mDoors);
        if(text) helper::print(mOled, 3, 15, text);

        mOled.sendBuffer();
    }

    /**
     * @brief Print input text
     * @param text
     */
    void print(const char *text) {
        if(text && *text == '\0') return;
        update(text);
    }

    void setState(const core::stateType &state) {
        mGps = state.gps;
        mAlarm = state.alarm;
        mEngine = state.engine;
        mDoors = state.doors;
        mSim = state.sim;
        mBattery = state.battery;
        update();
    }

    /**
     * @brief Set the Doors object
     *
     * @param tl top left
     * @param tr top right
     * @param br bottom left
     * @param bl bottom right
     */
    void setDoors(bool tl, bool tr, bool br, bool bl) {
        mDoors = (tl & 0x1) | (tr & 0x2) | (br & 0x4) | (bl & 0x8);
        update();
    }

    /**
     * @brief Set the Battery object
     * @param battery 0 - 16
     */
    void setBattery(uint8_t battery) {
        mBattery = battery;
        update();
    }

    /**
     * @brief Set the Statuses object.
     *
     * @param gps
     * @param alarm
     * @param engine
     * @param sim
     */
    void setStatuses(bool gps, bool alarm, bool engine, uint8_t sim) {
        mGps = gps;
        mAlarm = alarm;
        mEngine = engine;
        mSim = sim;
        update();
    }

    /// @brief Toggles display
    void toggle() {
        mActive = !mActive;
        if(mActive) mOled.sleepOff();
        else mOled.sleepOn();
    }

    /// @brief Turns OLED display off
    void sleep() {
        mActive = false;
        mOled.sleepOn();
    }

    /// @brief Turns OLED display on
    void wake() {
        mActive = true;
        mOled.sleepOff();
    }

private:
    bool mActive: 1, mGps: 1, mAlarm: 1, mEngine: 1;
    uint8_t mDoors: 4, mSim: 4, mBattery: 4;

    U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C mOled;
};
}