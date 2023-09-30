#include <stm32f1xx_ll_system.h>
#include <system_stm32f1xx.h>

#include "mcu/timer.h"
#include "mcu/mcu.h"

using cardian::mcu::utils;
using cardian::mcu::timer;

extern "C" {
void TIM4_IRQHandler(void) {
    if(TIM4->SR & TIM4->DIER & 1) { TIM4->SR = ~TIM_DIER_UIE; }
}

void TIM3_IRQHandler(void) {
    if(TIM3->SR & TIM3->DIER & 1) { TIM3->SR = ~TIM_DIER_UIE; }
}
}

void setupITM() {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Enable ITM stimulus ports
    ITM->LAR = 0xC5ACCE55;
    ITM->TCR = 0x0001000D;
    ITM->TPR = 0x00000001;
    ITM->TER = 0x00000001;

    // Set SWO baud rate (optional)
    TPI->SPPR = 0x00000002;
    TPI->ACPR = 0x00000007;
}
//ITM_SendChar('V');


int main() {
    utils::setupHSI();
    utils::pllHsiTo32mhz();

    setupITM();

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH = 0x1111;

    static bool led1 = false;
    timer::repeat(500, []{
        led1 = !led1;
        GPIOA->BSRR |= led1 ? 0x200 : 0x2000000;
    });

    timer::repeat(1000, []{ GPIOA->BSRR |= 0x100; });
    timer::once(500, []{
        timer::repeat(1000, []{ GPIOA->BSRR |= 0x1000000; });
    });

    while(true) {
        /// Timer loop
        timer::instance().timerloop();
    }

    return 0;
}
