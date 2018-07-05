#include "board.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"
#include "ciaaIO.h"
#include "main.h"

SemaphoreHandle_t semaforo;

static void initHardware(void)
{
    SystemCoreClockUpdate();
    Board_Init();
    Board_LED_Set(0, false);
    ciaaIOInit();
}

void funcionserie(void * parametro)
{
	while (1) {
        int x=0;
        int leds= 5; 
        int tiempo= 1000; 
        
        for(x=0; x<=leds; x++){
		  xSemaphoreTake(semaforo, portMAX_DELAY);
		  Board_LED_Set(leds, 1);
		  vTaskDelay(tiempo);
		  Board_LED_Set(leds, 0);
        }
	}
}


void botton(void * p)
{
	while (1) {
		while (ciaaReadInput(1) == 1) {
			vTaskDelay(5);
		}

		while (ciaaReadInput(1) == 0) {
			vTaskDelay(5);
		}
		xSemaphoreGive(semaforo);
	}
}

int main(void)
{

	initHardware();


	xTaskCreate(tarea1, "funcionserie", 512, 0, 1, 0);
	xTaskCreate(tarea2, "botton", 512, 0, 1, 0);

	semaforo = xSemaphoreCreateBinary();

	vTaskStartScheduler();

	while (1) {

	}
}
