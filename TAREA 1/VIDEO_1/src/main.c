#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
 
#define LED 2  //Utilizando el pin 2 del ESP que contiene el led interno del micro 
#define BT 17 //Definiendo la variable BT como 17
 

void app_main() {   //Funcion Principal

    gpio_set_direction(LED, GPIO_MODE_OUTPUT);//Declarando el pin 2 como una salida
    gpio_set_direction(BT, GPIO_MODE_INPUT);

    gpio_set_pull_mode(BT, GPIO_PULLDOWN_ONLY);

while (1)
{
    int state_bt = gpio_get_level(BT);

    if(state_bt){

        gpio_set_level(LED, 1);//mandando al pin un 1 logico
    }
    else
    {
        gpio_set_level(LED, 0);//mandando al pin un 0 logico   
    }
    
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
}
}