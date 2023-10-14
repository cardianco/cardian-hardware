#pragma once

#include <stm32f1xx_ll_cortex.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_rcc.h>
#include <string>

namespace cardian::core {
struct usartHelper final {
    enum UsartNum { Usart1 = 1, Usart2 = 2, Usart3 = 3 };
    static inline std::string buffer[3];
    static inline USART_TypeDef * usartx[] = {USART1, USART2, USART3};

    template <uint8_t idx = 0>
    inline __attribute__((always_inline)) static std::string &buf() {
        static_assert(idx <= 2, "Usart index should be between 0 and 2");
        return buffer[idx];
    }

    template <uint8_t idx = 0>
    static std::string readChar() {
        static_assert(idx <= 2, "Usart index should be between 0 and 2");
        std::string temp = buffer[idx].erase(0, 1);

        return temp;
    }

    template <uint8_t idx = 0, bool autoclear = false>
    static std::string readAll() {
        static_assert(idx <= 2, "Usart index should be between 0 and 2");
        std::string temp = buffer[idx];

        if(autoclear) buffer[idx].clear();

        return temp;
    }

    template <uint8_t idx = 0, bool autoclear = false>
    static std::string readUntil(char delim) {
        static_assert(idx <= 2, "Usart index should be between 0 and 2");
        std::string & _buf = buffer[idx];
        std::string temp = _buf.substr(0, _buf.find(delim));

        if(autoclear) {
            if(temp.length() ==_buf.length()) _buf.clear();
            else _buf = temp.erase(0, temp.length());
        }

        return temp;
    }

    template <uint8_t idx>
    inline static bool avaliable() { return usartx[idx]->SR & USART_SR_RXNE; }

    template <uint8_t idx>
    static uint32_t write(char ch) {
        usartx[idx]->DR = ch;
        while (!(usartx[idx]->SR & USART_SR_TXE));
        return 1;
    }

    template <uint8_t idx>
    static uint32_t write(const std::string &str) {
        uint32_t count = 0;

        for(char ch : str) {
            usartx[idx]->DR = ch; // Set data register
            while (!(usartx[idx]->SR & USART_SR_TXE)); // Wait for data written
            count++;
        }

        return count;
    }

    /**
     * @brief usartInit
     * @param buadrate
     */
    template <uint8_t idx = 0, bool interrupt = false>
    static void usartInit(uint32_t buadrate = 9600) {
        static_assert(idx <= 2, "Usart index should be between 0 and 2");
        usartInit(idx, buadrate, interrupt);
    }

    /**
     * @brief usartInit
     * @param index
     *  USART1<0>: RX(30) TX(31)
     *  USART2<1>: RX(13) TX(12)
     *  USART3<2>: RX(22) TX(21)
     * @param buadrate
     * @param interrupt
     */
    static void usartInit(uint8_t index, uint32_t buadrate = 9600, bool interrupt = false) {
        USART_TypeDef * usartx[] = {USART1, USART2, USART3};
        IRQn_Type usart_irqn[] = {USART1_IRQn, USART2_IRQn, USART3_IRQn};
        uint32_t usartx_io[] = {RCC_APB2ENR_IOPAEN, RCC_APB2ENR_IOPAEN, RCC_APB2ENR_IOPBEN};
        uint32_t usartx_en[] = {RCC_APB2ENR_USART1EN, RCC_APB1ENR_USART2EN, RCC_APB1ENR_USART3EN};

        uint32_t usartx_crh_mask =
            index == 0 ? GPIO_CRH_MODE9 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10 :
            index == 1 ? GPIO_CRH_MODE10 | GPIO_CRH_CNF10 :
                         GPIO_CRH_MODE10 | GPIO_CRH_CNF10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF11;

        uint32_t usartx_crh =
            index == 0 ? GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_0 :
            index == 1 ? GPIO_CRH_CNF10_0 : GPIO_CRH_MODE10_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_0;

        GPIO_TypeDef * GPIO = index == 2 ? GPIOB : GPIOA;

        // Enable GPIO
        RCC->APB2ENR |= usartx_io[index];
        if(index == 0) RCC->APB2ENR |= usartx_en[index];
        else RCC->APB1ENR |= usartx_en[index];

        // Reset CRH
        GPIO->CRH &= ~usartx_crh_mask;
        if(index == 1) GPIO->CRL &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
        // Configure `TX` as alternate function push-pull and RX as floating input `RX`
        GPIO->CRH |= usartx_crh;
        if(index == 1) GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_0;;

        // Configure buadrate
        usartx[index]->BRR = SystemCoreClock / buadrate;
        usartx[index]->CR2 &= ~USART_CR2_STOP;
        usartx[index]->CR3 &= ~(USART_CR3_CTSE | USART_CR3_RTSE);

        // USART baud rate = 9600, 8 data bits, 1 stop bit, no parity
        usartx[index]->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

        if(interrupt) {
            NVIC_EnableIRQ(usart_irqn[index]);
            NVIC_SetPriority(usart_irqn[index], index);
        }
    }
};

} // namespace cardian::core
