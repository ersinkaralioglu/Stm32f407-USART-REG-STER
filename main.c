#include "stm32f4xx.h"                  // Device header

#include "clock_config.h"
#include "delay.h"

void uart2config(void)
{
RCC->AHB1ENR |=(1<<17);	//usart2 clock eneable
	RCC->AHB1ENR |=(1<<0);//gpioa clock enable
	GPIOA->MODER |= (2<<4);  // Bits (5:4)= 1:0 --> Alternate Function for Pin PA2
	GPIOA->MODER |= (2<<6);  // Bits (7:6)= 1:0 --> Alternate Function for Pin PA3
	GPIOA->OSPEEDR |= (3<<4) | (3<<6);  // Bits (5:4)= 1:1 and Bits (7:6)= 1:1 --> High Speed for PIN PA2 and PA3
GPIOA->AFR[0] |= (7<<8);  // Bytes (11:10:9:8) = 0:1:1:1  --> AF7 Alternate function for USART2 at Pin PA2
	GPIOA->AFR[0] |= (7<<12); // Bytes (15:14:13:12) = 0:1:1:1  --> AF7 Alternate function for USART2 at Pin PA3
	USART2->CR1 = 0x00;  // clear all
	USART2->CR1 |= (1<<13);  // UE = 1... Enable USART
	USART2->CR1 &= ~(1<<12);  // M =0; 8 bit word length
	USART2->BRR = (7<<0) | (24<<4);   // Baud rate of 115200, PCLK1 at 45MHz
	USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
	USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter
}
	void UART2_SendChar (uint8_t c)
{
	
	
	USART2->DR = c; // load the data into DR register
	while (!(USART2->SR & (1<<6)));  // Wait for TC to SET.. This indicates that the data has been transmitted
}
	
void UART2_SendString (char *string)
{
	while (*string) UART2_SendChar (*string++);
}

uint8_t UART2_GetChar (void)
{
		
	uint8_t temp;
	
	while (!(USART2->SR & (1<<5)));  // wait for RXNE bit to set
	temp = USART2->DR;  // Read the data. This clears the RXNE also
	return temp;
}
	

int main(void)
{
	SysClockConfig();
	TIM6Config();
	uart2config();
while(1)
{
UART2_SendString ("hello world from controllerstech\n");
		Delay_ms (1000);
uint8_t data = UART2_GetChar ();
	UART2_SendChar (data);

}


}