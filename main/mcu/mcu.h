#pragma once

#include <stm32f1xx_ll_cortex.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_rcc.h>

namespace cardian::mcu {
struct utils final {
    static void init() {
        /// bits[7:5] used for Group Priority
        /// bits[4:3] used for Sub-Group Priority
        NVIC_SetPriorityGrouping(4);
    }
    /**
     * @brief setupHSI
     * @abstract setting up High Speed Internal clock
     */
    static inline void setupHSI() {
        /// Enable HSI (== 0b1)
        RCC->CR |= RCC_CR_HSION;
        /// Wait until HSI gets ready, CR(Clock Control Register)
        while(!(RCC->CR & RCC_CR_HSIRDY));

        /// Select HSI as main system clock (== 0b00), CFGR(Clock Configuration Register)
        RCC->CFGR &= ~RCC_CFGR_SW;
        /// Wait until main system clock set to HSI
        while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
    }

    /**
     * @abstract For a higher frequency, we need to use the PLL.
     * To enable the PLL, follow these steps:
     * 1. Disable PLL.
     * 2. Wait for the PLL to stop (`PLLRDY` == 0).
     * 3. <Set PLL parameters>.
     * 4. Set PLL on.
     * 5. Wait for the PLL to be enabled (`PLLRDY` == 1).
     *
     * HSI(8MHz) -(/2)-> PLL Src(4MHz) -(*8)-> SysClk (32Mhz)
     */
    static inline void pllHsiTo32mhz() {
        RCC->CR &= ~RCC_CR_PLLON;
        while(RCC->CR & RCC_CR_PLLRDY);

        /// Set PLL multiplier to 8 (8Mhz / 2 * 8 == 32Mhz)
        RCC->CFGR &= ~RCC_CFGR_PLLMULL;
        RCC->CFGR |= RCC_CFGR_PLLMULL8;

        RCC->CR |= RCC_CR_PLLON;
        while(!(RCC->CR & RCC_CR_PLLRDY));

        /// Select PLL as main system clock (== 0b10)
        RCC->CFGR &= ~RCC_CFGR_SW;
        RCC->CFGR |= RCC_CFGR_SW_PLL;
        /// Wait until main system clock set to PLL
        while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

        SystemCoreClockUpdate();
    }

    /**
     * @brief delay
     * @param ms delay time in milisecond
     * @note This function uses TIM4 as a timer for both short and long delays.
     *  Long delays occur when the wait time is greater than 5 seconds, and the MCU enters sleep mode using the WFI (Wait For Interrupt) instruction.
     *  It is worth to mention that every interrupt will wake the MCU up from a long delay.
     *  Short delays only utilize the timer's counter and count until the wait time is over.
     *  According to the MCU manual, the long delay consumes less power.
     */
    static void delay(uint32_t ms = UINT32_MAX) {
        if(ms < 5'000) { /// Sleeps less than 5 seconds
            __disable_irq();

            RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; /// Enable TIM4 clock
            TIM4->PSC = SystemCoreClock/1000;
            TIM4->EGR |= TIM_EGR_UG; /// Reset TIM4
            TIM4->CNT = 0;
            TIM4->ARR = UINT32_MAX; /// Trim at <arr>
            TIM4->CR1 |= TIM_CR1_CEN; /// Enable TIM4

            while(TIM4->CNT < ms) __asm("nop");

            __enable_irq();
        } else { /// MCU enters long sleep.
            TIM4->CR1 &= ~(TIM_CR1_CEN); /// Disable TIM4
            RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; /// Enable TIM4 clock

            NVIC_SetPriority(TIM4_IRQn, 0);
            NVIC_EnableIRQ(TIM4_IRQn);

            TIM4->PSC = SystemCoreClock/1000;
            TIM4->EGR |= TIM_EGR_UG; /// Reset TIM4
            TIM4->ARR = ms; /// Trim at <arr>

            TIM4->DIER |= TIM_DIER_UIE; /// Enable interupt
            TIM4->CR1 |= TIM_CR1_CEN; /// Enable TIM4

            __WFI(); /// Wait For Interrupt

            NVIC_DisableIRQ(TIM4_IRQn); /// Disable TIM4 interrupt
            RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN; /// Disable TIM4 clock
        }
    }

    static uint32_t timxNum(TIM_TypeDef *timx) {
        return (timx == TIM1) * 0x1 | (timx == TIM2) * 0x2 |
               (timx == TIM3) * 0x3 | (timx == TIM4) * 0x4;
    }

    template <uint32_t num = 1, bool interrupt = false>
    static void timxInit() {
        static_assert( 1 <= num && num <= 4, "Timer number should be between 1 and 4");
        timxInit(num - 1, interrupt);
    }

    /**
     * @brief initTIMx
     * @param idx
     * @param interrupt
     * @note The idea of giving index is to prevent resolving the input timer index.
     */
    static void timxInit(uint32_t idx, bool interrupt = false) {
        IRQn_Type timx_irqn[4] = {TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn};
        TIM_TypeDef* timx[4] = {TIM1, TIM2, TIM3, TIM4};
        uint32_t timx_en[4] = {RCC_APB2ENR_TIM1EN, RCC_APB1ENR_TIM2EN, RCC_APB1ENR_TIM3EN, RCC_APB1ENR_TIM4EN};

        timx[idx]->CR1 &= ~(TIM_CR1_CEN); /// Disable TIMx

        /// Enable TIMx clock
        if(idx) RCC->APB1ENR |= timx_en[idx];
        else RCC->APB2ENR |= timx_en[idx];

        NVIC_SetPriority(timx_irqn[idx], idx);
        NVIC_EnableIRQ(timx_irqn[idx]);

        timx[idx]->PSC = SystemCoreClock/1000;
        timx[idx]->EGR |= TIM_EGR_UG; /// Reset TIMx
        timx[idx]->ARR = UINT32_MAX;

        timx[idx]->DIER |= interrupt ? TIM_DIER_UIE : 0x0UL; /// Enable interupt
        // timx[index]->CR1 |= TIM_CR1_CEN;
    }
};
}
