#define __AVR_ATmega328P__

#include <avr/io.h>
#ifndef _AVR_IOM328P_H_
    #include <avr/iom328p.h>
#endif

#include "transmiter.h"
#include "display.h"
#include "core.h"

using namespace cardian::remote;
using namespace cardian::core;

transmiter gTransmiter {
    /// 256bit session token
    {135,169,17,52,49,244,80,140,147,207,81,255,246,91,16,94,49,51,123,96,230,161,129,59,22,27,122,38,28,92,106,18}
};

display gDisplay;
stateType state;

bool idle = false;
uint64_t wait = 0;

void setup() {
    utils::config();
    wait = idle = millis();

    gTransmiter.begin();

    gDisplay.init();
    gDisplay.splash();
}

void loop() {
    if(abs(millis() - wait - 4000) < 100) { gDisplay.update(); }
    if(gTransmiter.read(state)) {
        gDisplay.setState(state);
    }

    if(idle) {
        idle = false;
        utils::setTim1(false);
        // gDisplay.sleep();
    }

    if((~PIND) & 0b01111000) {
        int index = __builtin_ffs((~PIND) & 0b01111000) - 1;
        if((~PIND) & 0b01110000) gDisplay.wake();
        utils::setTim1(true); /// Start sleep timer.

        switch(index) {
            case PIN2: // Mercury
            case PIN3: // Button 1
                gDisplay.toggle();
                break;

            case PIN4: // Button 2
                gDisplay.print(!state.doors ? "lock" : "unlock");
                gTransmiter.sendCommand(fmt(R"({"doors":{0}})").arg(state.doors ? 0 : 15));
                break;

            case PIN5: // Button 3
                gDisplay.print(!state.alarm ? "alarm" : "no alarm");
                gTransmiter.sendCommand(fmt(R"({"alarm":{0}})").arg(!state.alarm));
                break;

            case PIN6: // Button 4
                gDisplay.print("no-op");
                // gTransmiter.sendCommand(fmt(R"({"alarm":{0}})").arg(!state.alarm));
                break;
        }

        while((~PIND) & 0b01111100);
    }
}

// Interrupt Service Routine for Port D
// ISR(PCINT2_vect) { _PIND = PIND; }

// Interrupt Service Routine for Timer1
ISR(TIMER1_COMPA_vect) { idle = true; }
