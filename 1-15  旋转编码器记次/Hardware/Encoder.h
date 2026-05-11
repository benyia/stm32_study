#ifndef _ENCODER_H
#define _ENCODER_H

void Encoder_Init(void);
int16_t Encoder_Get(void);
void EXTI1_IRQHandler(void);
void EXTI0_IRQHandler(void);


#endif
