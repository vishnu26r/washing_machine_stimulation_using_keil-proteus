#include<stm32f4xx.h>
void delay(unsigned int);
void UART_init(void);
void UART_sendchar(unsigned char ch);
unsigned int UART_Receivechar(void);
unsigned int data;
int main(){
	SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOAEN);//Enable the clock for GPIOA because it is used for uart serial communication(UART2)
	SET_BIT(RCC->AHB1ENR,RCC_AHB1ENR_GPIOCEN);//Enable the clock for GPIOC because it is used to turn on led for specific character
	SET_BIT(RCC->APB1ENR,RCC_APB1ENR_USART2EN);//Enable the Uart2 for serial communication
  UART_init();//UART initializaton
	GPIOC->MODER |=((1<<(0*2))|(1<<(2*2))|(1<<(4*2)));
	//GPIOA->PUPDR |=((1<<(0*2))|(2<<(2*2)));//PIN 0->PULL UP PIN 2->PULL DOWN 
	while(1){//infinite loop
	//UART_sendchar('A');//send the character
	//delay(1000);//1 second delay
	//GPIOC->ODR |=(0x00);
	 data=UART_Receivechar();
	 //dat=data;
	 if(data=='W'){
		GPIOC->ODR &=0x00;
		GPIOC->ODR |=(1<<0);//send the character
		GPIOC->ODR |=(0<<2);
		UART_sendchar('M');
		//delay(5000);
		/*GPIOC->ODR &=0x00;
		GPIOC->ODR |=(1<<0);//send the character
		GPIOC->ODR |=(0<<2);
    delay(5000);*/	 
  }

	if(data=='S'){
		GPIOC->ODR &=0x00;
		//GPIOC->ODR &=~((1<<0)|(0<<2));
		GPIOC->ODR |=(0<<0);
		GPIOC->ODR |=(1<<2);
		delay(100);
		UART_sendchar('S');
	}
	if(data=='D'){
		GPIOC->ODR &=(0x00);
		GPIOC->ODR |=(1<<4);
		delay(100);
		UART_sendchar('D');
	}
}
	}

void UART_init(void){
	/*1. Enable the USART by writing the UE bit in USART_CR1 register to 1
	  2.Program the M bit in USART_CR1 to define the word length.
	  3. Program the number of stop bits in USART_CR2.
	  4.Select the desired baud rate using the USART_BRR register.
	  5. Set the TE bit in USART_CR1 to send an idle frame as first transmission
	  6. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this 
       for each data to be transmitted in case of single buffer.
    7. After writing the last data into the USART_DR register, wait until TC=1. This indicates 
       that the transmission of the last frame is complete. This is required for instance when 
       the USART is disabled or enters the Halt mode to avoid corrupting the last 
       transmission.*/
	GPIOA->MODER |=(2<<(2*2))| (2<<(3*2));//Enable the PA2->TX and PA3->RX for serial communication 
	GPIOA->AFR[0] |=(7<<(2*4))|(7<<(3*4));// Set the hexadecimal(7)<<4 Alternate function low for PA2 and PA3
	GPIOA->OSPEEDR |= (3<<(2*2))|(3<<(3*2));//Set the very high speed for both PA2 and PA3 pins in UART
	//enable CR1 IN USART
	USART2->CR1 |=(1<<13);//set (UE)USART enable bit
	//define the word length(M==0)
	USART2->CR1 |=(0<<12);//1 start bit,8 Data bits,n stop bit
	//Select the Baud rate for uart
	USART2->BRR |=0x0683;//baud rate 9600
	//Enable the TE(Transmitter enable) bit in USART_CR1(TE=1)
	USART2->CR1 |=(1<<3);
}
void UART_sendchar(unsigned char ch)//Transmitter function for sending character
{
	USART2->DR=ch;//UART data register for load value into data register
	while((USART2->SR &(1<<6))==0);// it will wait for (TC==1)means Transmission Completed
	
}
void delay(unsigned int ms){
	unsigned int i,j;
	for(i=0;i<ms;i++)//delay loop
	for(j=0;j<1275;j++);
}
unsigned int UART_Receivechar(){
	/*1. Program the number of stop bits in USART_CR2.
	  2. Set the RE bit USART_CR1. This enables the receiver which begins searching for a 
     start bit
	  3. The RXNE bit is set. It indicates that the content of the shift register is transferred to the 
       RDR. In other words, data has been received and can be read (as well as its 
       associated error flags).
	  4.The error flags can be set if a frame error, noise or an overrun error has been detected 
      during reception.
	  4. In single buffer mode, clearing the RXNE bit is performed by a software read to the 
       USART_DR register. The RXNE flag can also be cleared by writing a zero to it. The 
       RXNE bit must be cleared before the end of the reception of the next character to avoid 
       an overrun error
	*/
	//enable number of stop bits in USART_CR2
	USART2->CR2 |=(0<<12) | (0<<13);
	//enable RE==1 (Receiver Enable) bit
	USART2->CR1 |=(1<<2);
	while((USART2->SR & (1<<5))==0);
	USART2->SR &=~(1<<2);
	return USART2->DR;
}