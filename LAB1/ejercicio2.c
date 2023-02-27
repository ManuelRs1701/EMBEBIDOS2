#include "lab1.h"
#define xPortPendSVHandler PendSV_Handler 
#define vPortSVCHandler SVC_Handler 
#define xPortSysTickHandler SysTick_Handler

uint32_t my_Ticks = 0;

void vTaskFunction_1(void *pvParameters);
void vTaskFunction_2(void *pvParameters);


int ejercicio_2(){


	TaskHandle_t vTaskAHandle_1 = NULL;
	TaskHandle_t vTaskAHandle_2 = NULL;
	
	BaseType_t result_1 = pdPASS;
	BaseType_t result_2 = pdPASS;
	
	
	result_1 = xTaskCreate(vTaskFunction_1, "Tarea 1", 1000, NULL, 0, &vTaskAHandle_1);
	result_2 = xTaskCreate(vTaskFunction_2, "Tarea 2", 1000, NULL, 1, &vTaskAHandle_2);
	
	result_1 = pdPASS;
	result_2 = pdPASS;

	//Condiguracion del timer
	setup_delay();
	
	
	
	//Configuracion de los leds
	RCC->AHBENR |= (1U<<17);					// Habilitamos el acceso al AHB
	
	GPIOA->MODER |=  (1U<<0);				// Configura el PA0 como salida
	GPIOA->MODER &= ~(1U<<1); 				
	
	GPIOA->MODER |=  (1U<<2);				// Configura el PA1 como salida
	GPIOA->MODER &= ~(1U<<3); 				
	
	GPIOA->MODER |=  (1U<<8);				// Configura el PA4 como salida
	GPIOA->MODER &= ~(1U<<9);
	
	GPIOA->ODR |= (1U<<0);					// Salida PA0
	delay_us(200000);
	GPIOA->ODR &= ~(1U<<0);

	vTaskStartScheduler(); 				// Para llamar al thread mediante el cronograma 
	
	return 0;
}

void vTaskFunction_1(void *pvParameters){

	//Init can be here
	while(1){
	
		//Task functionality 
		GPIOA->ODR ^= (1U<<1);					// Salida PA1
		delay_us(200000);
	}

}

void vTaskFunction_2(void *pvParameters){

	//Init can be here
	while(1){
	
		//Task functionality 
		GPIOA->ODR ^= (1U<<4);					//salida PA4
		delay_us(200000);
		
	}

}

//Configuracion del delay con TIM3
void setup_delay(void){
	
	RCC->APB1ENR |= (1U << 1);		//Habilitamos el clock para TIM3

	TIM3->PSC = 0;								
	TIM3->ARR = 72;								//frecuencia del timer 1Mhz
	TIM3->CR1 |= (1U << 2);	
	TIM3->DIER |= (1U << 0);
	TIM3->EGR |= (1U << 0);
	
	NVIC_EnableIRQ(TIM3_IRQn);

}

void TIM3_IRQHandler(void){

	my_Ticks++;
	TIM3->SR &= ~(1U<<0);

}

void delay_us(uint32_t us){

	TIM3->CR1 |= (1U << 0);
	my_Ticks = 0;
	while(my_Ticks < us);
	
	TIM3->CR1 &= ~(1U << 0);
}
