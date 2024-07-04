/*
IINPUTS
LSC = PIN13
LSA = PIN 12
FC = PIN1 4
CC = PIN 27
CA = PIN 26

OUTPUTS
LED_M = PIN 4
LED_E = PIN16
MC = PIN 17
MA = PIN 5
*/

//Librerias utilizadas
#include <stdio.h> //Libreria del lenguaje C
#include <stdlib.h>
#include <freertos/FreeRTOS.h>//Libreria que nos permite usar los delay
#include <freertos/task.h>
#include <driver/gpio.h> //libreria que nos permite el manejo dde kis oerifericos GPIO
#include <freertos/timers.h> //Libreria para utilizar el timer 
#include "esp_log.h" //Libreria para consultar el resultado
#include "freertos/timers.h"

//definiciones de variables

static const char *tag = "Main";

#define TRUE 1
#define FALSE 0

#define ESTADO_INIT         0
#define ESTADO_ABRIENDO     1
#define ESTADO_CERRANDO     2
#define ESTADO_CERRADO      3
#define ESTADO_ABIERTO      4
#define ESTADO_EMERGENCIA   5
#define ESTADO_ERROR        6
#define ESTADO_ESPERA       7

//Definicion de la variable utilizada como handle
TimerHandle_t xTimers;
int timerID = 1;
int INTERVALO = 50;

//DECLARACION DE FUNCIONES
esp_err_t INTERRUPCION_50MS(void); //Funcion de lectura de los pines cada interrupcion
esp_err_t SET_TIMER(void);

int Func_ESTADO_INIT();
int Func_ESTADO_ABRIENDO();
int Func_ESTADO_CERRANDO();
int Func_ESTADO_CERRADO();
int Func_ESTADO_ABIERTO();
int Func_ESTADO_EMERGENCIA();
int Func_ESTADO_ERROR();
int Func_ESTADO_ESPERA();

/*****************************************************/

#define TRUE 1
#define FALSE 0

#define ESTADO_INIT         0
#define ESTADO_ABRIENDO     1
#define ESTADO_CERRANDO     2
#define ESTADO_CERRADO      3
#define ESTADO_ABIERTO      4
#define ESTADO_EMERGENCIA   5
#define ESTADO_ERROR        6
#define ESTADO_ESPERA       7



volatile int ESTADO_ACTUAL = ESTADO_INIT;
volatile int ESTADO_SIGUIENTE = ESTADO_INIT;
volatile int ESTADO_ANTERIOR = ESTADO_INIT;
volatile unsigned int TimeCa = 0;


volatile struct INPUTS{
    unsigned int LSA: 1;
    unsigned int LSC: 1;
    unsigned int CA:  1;
    unsigned int CC:  1;
    unsigned int FC:  1;
}inputs;
volatile struct OUTPUTS{
    unsigned int MC: 1;
    unsigned int MA: 1;
    unsigned int LED_EMERGENCIA: 1;
    unsigned int LED_MOVIMIENTO: 1;
}outputs;


/**************************************************** */

void vTimerCallback( TimerHandle_t pxTimer){

    ESP_LOGE(tag, "INTERRUPCION HECHA.");
    INTERRUPCION_50MS();
}

void app_main() {

    ESTADO_SIGUIENTE = Func_ESTADO_INIT();
    SET_TIMER();


    for (;;)
    {    
        if (ESTADO_SIGUIENTE == ESTADO_INIT)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_INIT();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ESPERA)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ESPERA();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ABRIENDO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ABRIENDO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_CERRANDO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_CERRANDO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_CERRADO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_CERRADO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ABIERTO)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ABIERTO();
        }
        if (ESTADO_SIGUIENTE == ESTADO_EMERGENCIA)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_EMERGENCIA();
        }
        if (ESTADO_SIGUIENTE == ESTADO_ERROR)
        {
            ESTADO_SIGUIENTE = Func_ESTADO_ERROR();
        }
        
    }

}

//TERMINADA
int Func_ESTADO_INIT(){

    ESP_LOGE(tag, "INICIANDO PROGRAMA");

    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_INIT;


    /********************************************************************************* */
    ESP_LOGE(tag, "CONFIGURACION DE LOS PINES");
     //Estructura de configuracion de pines
    gpio_config_t IO_CONFIG;

    //CONFIGURANDO LAS ENTRADAS
    IO_CONFIG.mode = GPIO_MODE_INPUT;
    IO_CONFIG.pin_bit_mask = (1 << 13) | (1 << 12) | (1 << 14) | (1 << 27) | (1 << 26);
    IO_CONFIG.pull_down_en = GPIO_PULLDOWN_ENABLE;
    IO_CONFIG.pull_up_en = GPIO_PULLUP_DISABLE;
    IO_CONFIG.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&IO_CONFIG);

    //CONFIGURANDO LAS SALIDAS
    IO_CONFIG.mode = GPIO_MODE_OUTPUT;
    IO_CONFIG.pin_bit_mask = (1 << 4) | (1 << 16) | (1 << 17) | (1 << 5);
    IO_CONFIG.pull_down_en = GPIO_PULLDOWN_DISABLE;
    IO_CONFIG.pull_up_en = GPIO_PULLUP_DISABLE;
    IO_CONFIG.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&IO_CONFIG);

    //INICIALIZACION DE LAS INTERRUPCIONES

    /******************************************** */
    for (;;)
    {
        return ESTADO_ESPERA;
    }
}

//SIN TERMINAR
int Func_ESTADO_ABRIENDO(){

    printf("ABRIENDO EL PUERTON");

    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABRIENDO;

    //Determianndo el comportamiento de las salida en este estado
    outputs.LED_MOVIMIENTO = TRUE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = TRUE;
    outputs.MC = FALSE;

    for (;;)
    {
        //Si se activa el Limit Swicth de abierta
        if(inputs.LSA == TRUE){

            return ESTADO_ABIERTO; //Retorna el estado del puerton abierto
        }
        if (inputs.LSA == TRUE && inputs.LSC == TRUE)
        {
            return ESTADO_ERROR;
        }
        if (inputs.FC == TRUE)
        {
            return ESTADO_EMERGENCIA;
        }
        if(inputs.CC == TRUE ){

            return ESTADO_CERRANDO;
        }
        /*FAALTA COMPROBAR SI TIENE MAS DE 3 MINUTOS ABRIENDO
        ESTO LO ENVIA AL ESTADO DE ERROR*/  
    }
}


int Func_ESTADO_CERRANDO(){

    printf("CERRANDO EL PUERTON");

    //ACTUALIZAR Variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRANDO;

    outputs.LED_MOVIMIENTO = TRUE;
    outputs.LED_EMERGENCIA = FALSE;
    //COLOCAR TIEMPO DE ESPERA 10ms A 25ms
    outputs.MA = FALSE;
    outputs.MC = TRUE;

    for (;;)
    {
        if (inputs.LSC == TRUE )
        {
            return ESTADO_CERRADO;
        }

        if (inputs.LSA == TRUE && inputs.LSC == TRUE)
        {
            return ESTADO_ERROR;
        }
        //todo  verificar FC y salir a emergencia
        if (inputs.FC == TRUE)
        {
            return ESTADO_EMERGENCIA;
        }
        if (inputs.CA == TRUE)
        {
            return ESTADO_ABRIENDO;
        }
        

        
        
        //todo Verificar tiempo

        //todo verificar cambio de giro.
        //detener el motor y esperar 3 segundos para cambiar giro
    }
}


int Func_ESTADO_CERRADO(){

    printf("PUERTON CERRADO ");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_CERRADO;

    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;

    for (;;)
    {
        return ESTADO_ESPERA;
    }

}


int Func_ESTADO_ABIERTO(){

    printf("PUERTON ABIERTO ");
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ABIERTO;

    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = FALSE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;

    for (;;)
    {
        return ESTADO_ESPERA;
    }
}


int Func_ESTADO_EMERGENCIA(){

    printf("EMERGENCIA!!!!!!!");

    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_EMERGENCIA;

    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = TRUE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;
    
    

    for (;;)
    {
        vTaskDelay(1500/portTICK_PERIOD_MS);
        if (inputs.FC == FALSE)
        {
            return ESTADO_ANTERIOR;
        }
          
    }
}


int Func_ESTADO_ERROR(){

    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ERROR;

    outputs.LED_MOVIMIENTO = FALSE;
    outputs.LED_EMERGENCIA = TRUE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;

    printf("\nERROR!!!!");
    printf("\nERROR!!!!");
    printf("\nERROR!!!!");

    for (;;)
    {
        vTaskDelay(500/portTICK_PERIOD_MS);
        outputs.LED_EMERGENCIA = FALSE;
        vTaskDelay(500/portTICK_PERIOD_MS);
        outputs.LED_EMERGENCIA = TRUE;
        vTaskDelay(500/portTICK_PERIOD_MS);
        outputs.LED_EMERGENCIA = FALSE;

        return ESTADO_ESPERA;
        
    }
}


int Func_ESTADO_ESPERA(){

    
    //Actualizar Variables de estado
    ESTADO_ANTERIOR = ESTADO_ACTUAL;
    ESTADO_ACTUAL = ESTADO_ESPERA;

    //Valores de salidas
    outputs.LED_EMERGENCIA = FALSE;
    outputs.LED_MOVIMIENTO = FALSE;
    outputs.MA = FALSE;
    outputs.MC = FALSE;
    ESP_LOGE(tag, "ESTADO ESPERA");

    for (;;)
    {//VERIFICANDO ENTRADAS PARA EL SIGUIENTE MOVIMIENTO
        
        if (inputs.LSA == FALSE && inputs.FC == FALSE && inputs.LSC == FALSE)//Si se encuentra en un estado entre abierto
                                                                             //Por falla en el sistema o por falta de luz
        {
            return ESTADO_CERRANDO;
        }
        if (inputs.CA == TRUE && inputs.FC == FALSE && inputs.LSA == FALSE && inputs.FC == FALSE)
        {
            return ESTADO_ABRIENDO;
        }
        if (inputs.CC == TRUE && inputs.FC == FALSE)
        {
            return ESTADO_CERRANDO;
        }
        if (inputs.CA == TRUE && inputs.FC == FALSE)
        {
            return ESTADO_ABRIENDO;
        }
        
        if (inputs.FC == TRUE)
        {
            return ESTADO_EMERGENCIA;
        }
        if(inputs.LSA == TRUE && inputs.LSC == TRUE)
        {
            return ESTADO_ERROR;
        }
        
    }
}


esp_err_t SET_TIMER(void){

    printf("Inicializando configuracion del timer...");

    xTimers = xTimerCreate("Timer", 
                          (pdMS_TO_TICKS(INTERVALO)),
                          pdTRUE, 
                          ( void * )timerID, 
                          vTimerCallback
);
        
        if (xTimers == NULL)
        {
            printf("El timer no fue creado");
        }else 
        {
            if (xTimerStart(xTimers, 0) != pdPASS)
            {
                printf("El timer podria no ser seteado en el estado activo");
            }
            
        }

    return ESP_OK;
}

esp_err_t INTERRUPCION_50MS(void){


    //SETEANDO LOS PINES DEL MICRO
    //Pines de entradas
    //Limit swicth de puerta cerrada
    inputs.LSC = (gpio_get_level(13) == TRUE)?TRUE:FALSE;
    //Limit swicth de puerta abierta
    inputs.LSA = (gpio_get_level(12) == TRUE)?TRUE:FALSE;
    //Sensor de intercepcion
    inputs.FC = (gpio_get_level(14) == TRUE)?TRUE:FALSE;
    //Comando cerrar
    inputs.CC = (gpio_get_level(27) == TRUE)?TRUE:FALSE;
    //Comando abirir
    inputs.CA = (gpio_get_level(26) == TRUE)?TRUE:FALSE;

    //Pines de salida
    //Led de movimiento
    if (outputs.LED_MOVIMIENTO == TRUE)
    {
        gpio_set_level(4,TRUE);
    }else
    {
        gpio_set_level(4,FALSE);
    }
    //Led de Emergencia
    if (outputs.LED_EMERGENCIA == TRUE)
    {
        gpio_set_level(16,TRUE);
    }else
    {
        gpio_set_level(16,FALSE);
    }

    //Led de Motor para abir
    if (outputs.MC == TRUE)
    {
        gpio_set_level(17,TRUE);
    }else
    {
        gpio_set_level(17,FALSE);
    }
    //Led de Motor para cerrar
    if (outputs.MA == TRUE)
    {
        gpio_set_level(5,TRUE);
    }else
    {
        gpio_set_level(5,FALSE);
    }


    return ESP_OK;
}


