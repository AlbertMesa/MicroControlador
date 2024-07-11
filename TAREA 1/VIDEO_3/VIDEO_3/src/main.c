#include<stdio.h>
#include<freertos/FreeRTOS.h>//Libreria que nos permite usar los delay
#include<freertos/task.h>
#include<driver/gpio.h> //libreria que nos permite el manejo dde kis oerifericos GPIO
#include<driver/adc.h>
#include <math.h>



#define LED 2
#define bt 17

adc1_channel_t adc_pot = ADC1_CHANNEL_6;

void init_esp(void);

void app_main() {


    init_esp();
 
    float tiempo = 300/portTICK_PERIOD_MS; //determina el tiempo en milisegundos
    int state = 0;
    int State_bt;
    int State_bt_anterior = 0;
    float voltaje = 0;

    while (1)
    {
        
        
        State_bt = gpio_get_level(bt); //oBTENER EL ESTADO DEL BUTTON 
        
        if (State_bt == 1 && State_bt_anterior == 0)
        {
            state = !state;
        }
        


//DETERMINA SI EL CIRCUITO ESTA ENCENDIDO O APAGADO
        if (state == 1)
        {
            int pot = adc1_get_raw(adc_pot); //GUARDAR LO CONVERTIDO DEL ADC EN LA VARIABLE
            voltaje = (pot*3300)/4095;
            voltaje = voltaje / 1000;
            gpio_set_level(LED, 1); //Enciende un led para indicar que el circuito esta encendido
            printf("\n ADC = %d", pot); //IMPRIME EN EL MONITOR SERIAL EL ADC
            printf("    Voltaje = %.2f", voltaje);
            vTaskDelay(tiempo); //TIEMPO DE 0.3 SEGUNDOS
        }
        else
        {
            gpio_set_level(LED, 0);
            printf("\nAPAGADO.");
        }
        
        
    }
    vTaskDelay(tiempo); //TIEMPO DE 0.3 SEGUNDOS
    
}

void init_esp(void){

    gpio_config_t io_config;

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(adc_pot, ADC_ATTEN_DB_12);


    /****************************************************************************************** */
    io_config.mode = GPIO_MODE_INPUT; // CONFIGURA EL MODOD DEL PUERTO UTILIZADO, SI ES ENTRADA O SALIDA
    io_config.pin_bit_mask = (1 << bt); //es la funcion que nos permite indicarle a nuestra estructura que pines recibiran la configuracion 
                                        //se coloca una or para indicar mas pines
    io_config.pull_down_en = GPIO_PULLDOWN_ONLY; //Habilitamos una pull down en el pin de entrada
    io_config.pull_up_en = GPIO_PULLUP_DISABLE; //deshabilitamos la pull up ya que no la utilizaremos
    io_config.intr_type = GPIO_INTR_DISABLE; //Deshabilitamos las interrupciones en el pin
    gpio_config(&io_config);

    io_config.mode = GPIO_MODE_OUTPUT; // CONFIGURA EL MODOD DEL PUERTO UTILIZADO, SI ES ENTRADA O SALIDA
    io_config.pin_bit_mask = (1 << LED); //es la funcion que nos permite indicarle a nuestra estructura que pines recibiran la configuracion 
                                        //se coloca una or para indicar mas pines
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE; //deshabilitamos una pull down en el pin de salida
    io_config.pull_up_en = GPIO_PULLUP_DISABLE; //deshabilitamos la pull up ya que no la utilizaremos
    io_config.intr_type = GPIO_INTR_DISABLE; //Deshabilitamos las interrupciones en el pin
    gpio_config(&io_config);
}