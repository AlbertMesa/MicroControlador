#include <stdio.h> //Libreria del lenguaje C
#include <freertos/FreeRTOS.h>//Libreria que nos permite usar los delay
#include <freertos/task.h>
#include <driver/gpio.h> //libreria que nos permite el manejo dde kis oerifericos GPIO
#include <freertos/timers.h> //Libreria para utilizar el timer 
#include "esp_log.h" //Libreria para consultar el resultado

#define LED 2
#define BT 17

int display[] = {14, 27, 18, 25, 26, 12, 13};

char number[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, //0
    {0, 1, 1, 0, 0, 0, 0}, //1 
    {1, 1, 0, 1, 1, 0, 1}, //2
    {1, 1, 1, 1, 0, 0, 1}, //3
    {0, 1, 1, 0, 0, 1, 1}, //4
    {1, 0, 1, 1, 0, 1, 1}, //5
    {0, 0, 1, 1, 1, 1, 1}, //6
    {1, 1, 1, 0, 0, 0, 0}, //7
    {1, 1, 1, 1, 1, 1, 1}, //8
    {1, 1, 1, 0, 0, 1, 1}, //9

};


void hw_init(void);

void app_main() {

    hw_init();

    int count = 0;

    for (int i = 0; i < 7; i++)
    {
       
    }
    

    while (1)
    {
        int state_bt = gpio_get_level(BT);

        for (int i = 0; i < 7; i++)
        {
             gpio_set_level(display[i], number[count][i]);
        }
        
        count = (state_bt && count < 10) ? (count+1) : count;
        count = (count > 9) ? 0 : count;

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    
}

void hw_init(void)
{

    gpio_config_t io_config;

    io_config.mode = GPIO_MODE_INPUT;
    io_config.pin_bit_mask = (1 << BT);
    io_config.pull_down_en = GPIO_PULLDOWN_ONLY;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config);

    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1 << display[0]) |
                             (1 << display[1]) |
                             (1 << display[2]) |
                             (1 << display[3]) |
                             (1 << display[4]) |
                             (1 << display[5]) |
                             (1 << display[6]);
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_config);


    


    
}