#pragma once
#include <U8g2lib.h>
#include "icons.h"

namespace cardian::remote {
struct rect { uint8_t x, y, width, height; };

struct helper {
    static String loadFSTR(const uint8_t *rstr) {
        return reinterpret_cast<const __FlashStringHelper *>(&rstr[0]);
    }

    static void print(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C &led, uint8_t x, uint8_t y, const char *text) {
        // Select a font https://github.com/olikraus/u8g2/wiki/fntlistall
        led.setFont(u8g2_font_9x18_tr);
        led.drawStr(x, y + 12, text); // Writes `text` at [x, y] coordinates
    }

    static void drawIcon(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C &led, uint8_t x, uint8_t y, const uint8_t *icon) {
        String buf = loadFSTR(icon);
        const uint8_t *_buf = reinterpret_cast<const uint8_t*>(buf.c_str());

        led.drawBitmap(x, y, _buf[0]/8, _buf[1], _buf + 2);
    }

    static void drawBattery(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C &led, uint8_t x, uint8_t y, uint8_t battery) {
        led.drawFrame(x, y, 17, 6);
        led.drawBox(x + 17, y + 2, 1, 2);
        led.drawBox(x + 1, y + 1, battery, 4);
    }

    static void drawStatuses(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C &led, uint8_t x, uint8_t y,
                            bool gps, bool alarm, bool engine, uint8_t sim) {
        drawIcon(led, x,      y, gps    ? icons::gps    : icons::gps_off);
        drawIcon(led, x + 12, y, alarm  ? icons::alarm  : icons::alarm_off);
        drawIcon(led, x + 24, y, engine ? icons::engine : icons::engine_off);
        drawIcon(led, x + 36, y, sim == 0 ? icons::sim_0 : sim == 1 ? icons::sim_1 :
                                                           sim == 2 ? icons::sim_2 : icons::sim_3);
    }

    static void drawCar(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C &led, uint8_t x, uint8_t y, uint8_t doors) {
        const uint8_t *lock = icons::lock, *unlock = icons::unlock;

        /// Doors: [0:tl, 1:tr, 2:br, 3:bl]
        drawIcon(led, x,      y + 10, doors & 0b0001 ? lock : unlock);
        drawIcon(led, x + 24, y + 10, doors & 0b0010 ? lock : unlock);
        drawIcon(led, x + 24, y + 20, doors & 0b0100 ? lock : unlock);
        drawIcon(led, x,      y + 20, doors & 0b1000 ? lock : unlock);

        drawIcon(led, x + 8,  y, icons::car);
    }
};
}