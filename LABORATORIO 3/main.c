#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// TODO: Include Semaphores Exercise 2

#include "gpio.h"

// Simple random delay
void vSimpleDelay(void);

// TODO: Global Variables here

flag[2] = {0, 0};
int token = 0;

//estructura
typedef struct{

	QueueHandle_t xQueueSqrt;
	QueueHandle_t xQueueDecr;
	
} TaskQueues_t;

// END TODO

// Task functions
void vTask1(void* pvParameters);
void vTask2(void* pvParameters);


#define LARGE	5
#define ITEM_SIZE	sizeof(uint32_t)

SemaphoreHandle_t xSemaphore = NULL;



int main(){
	
	setup_RCC();
	setup_GPIO();
	
	TaskQueues_t taskQueues;

	QueueHandle_t xQueueSqrt = NULL;
	QueueHandle_t xQueueDecr = NULL;

	xQueueSqrt = xQueueCreate(LARGE, ITEM_SIZE);
	xQueueDecr = xQueueCreate(LARGE, ITEM_SIZE);

	taskQueues.xQueueSqrt = xQueueSqrt;
	taskQueues.xQueueDecr = xQueueDecr;
	
	void* pvPointer = (void*)&taskQueues;
	
	xSemaphore = xSemaphoreCreateMutex();

	BaseType_t result = pdPASS;
	
	result = xTaskCreate(vTask1, "SquareTask", configMINIMAL_STACK_SIZE, pvPointer, 1, NULL);
	configASSERT(result == pdPASS)
	
	result = xTaskCreate(vTask2, "DecrementTask", configMINIMAL_STACK_SIZE, pvPointer, 1, NULL);
	configASSERT(result == pdPASS)
	
	vTaskStartScheduler();
	
	
	return 0;
}



void vSimpleDelay(void) {
	uint32_t nCount = 8000000;
	for(uint32_t i=0; i < nCount; i++) {
		__NOP();
	}
}

/*
*	@Brief Embedded Systems II (ES II) Lab 2
* 
* The goal of this lab is that the students get to know the OS
* FreeRTOS and concepts related to shared resources.
* 
* This functions implements the task 1 and task 2
* where task1 could be a blue LED and task2 could be a green LED
* 
* TODO:
*     Change to your Ports and pins
*     Exercise 1: Shared resources
*     Exercise 2: Semph
*/
void vTask1(void* pvPointer){
		uint32_t square = 0;
		uint32_t decrement_recieved = 0;
	
		while(1){
		GPIOA->ODR |= GREEN;
		Access(RED);
		TaskQueues_t* pcPointer = (TaskQueues_t*)pvPointer;
		
		xQueueSendToBack(pcPointer->xQueueSqrt, (void*)&square, pdMS_TO_TICKS(1000));
		xQueueReceive(pcPointer->xQueueDecr, (void*)&decrement_recieved, pdMS_TO_TICKS(1000));
		
		square = decrement_recieved*decrement_recieved;
		
		Release(RED);
		GPIOA->ODR &= ~GREEN;
		vTaskDelay(pdMS_TO_TICKS(1000));
		
		}
		// Critical section ends here!
		
}


void vTask2(void* pvPointer){
		uint32_t decrement = 0;
		uint32_t square_recieved = 0;
		
	
		while(1){
		GPIOA->ODR |= BLUE;

		
		TaskQueues_t* pcPointer = (TaskQueues_t*)pvPointer;
		xQueueSendToBack(pcPointer->xQueueDecr, (void*)&decrement, pdMS_TO_TICKS(1000));
		Access(RED);
		xQueueReceive(pcPointer->xQueueSqrt, (void*)&square_recieved, pdMS_TO_TICKS(1000));
			
		decrement = square_recieved-1;
		Release(RED);
		GPIOA->ODR &= ~BLUE;
		vTaskDelay(pdMS_TO_TICKS(1000));
		
		}
}

