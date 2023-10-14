#include "usart.h"

namespace cardian::core {
extern "C" {
void USART1_IRQHandler(void) {
    // USART Status register
    if(USART1->SR & USART_SR_RXNE) {
        usartHelper::buf<0>().push_back(USART1->DR);
    }
}

void USART2_IRQHandler(void) {
    if(USART2->SR & USART_SR_RXNE) {
        usartHelper::buf<1>().push_back(USART2->DR);
    }
}

void USART3_IRQHandler(void) {
    if(USART3->SR & USART_SR_RXNE) {
        usartHelper::buf<2>().push_back(USART3->DR);
    }
}
}
} // namespace cardian::core
