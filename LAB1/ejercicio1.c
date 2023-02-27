#include "lab1.h"



void vTaskFunction(void *pvParameters);


void ejercicio_1(){


	TaskHandle_t vTaskAHandle = NULL;
	BaseType_t result = pdPASS;
	
	result = xTaskCreate(vTaskFunction, "TASK A", 1000, NULL, 1, &vTaskAHandle);
	
		if(result != pdFAIL){
		// Do something
	}
	
	vTaskStartScheduler(); // Para llamar al thread mediante el cronograma 

}

void vTaskFunction(void *pvParameters){

	//Init can be here
	while(1){
		//Task functionality 

	}

}