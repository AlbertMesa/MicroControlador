#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
//Librerias utilizadas

//Definiendo la estructura a utilizar
typedef struct {
    char nombre[50];
    int edad;
    int nota;
} ESTRUCTURA;

//Funciones utilizadas
void grabarDato(void);
void leerDisco(void);
void salir(void);

//funcion principal
int main() {

    system("color 70"); //Cambiar el color del programa
    int opcion;
    do {    //bucle para cuando terminemos una de las opciones se mantenga en el programa
        printf("\t*********Menu:**********\n");
        printf("1- Grabar Dato\n");
        printf("2- Leer Disco\n");
        printf("3- Salir\n");
        printf("~~OPCION~~: ");
        scanf("%d", &opcion);   //Escaneo de la opcion ingresada

        switch(opcion) {    
            case 1: //si es la primera opcion ira a la funcion de grabar dato
                grabarDato();
                system("cls");
                break;
            case 2: //si es la segunda opcion ira a la funcion de leer disco
                leerDisco();
                system("pause");
                system("cls");
                break;
            case 3: //si es la tercera opcion ira a la funcion de salir del programa
                salir();
                break;
            default:    //si no es una de la opciones disponible vuelve al incio
                printf("Opcion no valida.\n");
                system("pause");
                system("cls");
        }
    } while(opcion != 3); //se mantendra el bucle mientras la opcion sea diferente a 3

    return 0;
}

void grabarDato(void) {

    FILE *archivo = fopen("alumno.dat", "ab"); //creacion de un puntero tipo archivo 

    if (!archivo) { //si es 0 entonces el archivo no se abrio
        perror("Error al abrir el archivo");
        return;
    }

    ESTRUCTURA *alumno; //creacion de un puntero a la estructura creada
    alumno = (ESTRUCTURA*) malloc(sizeof(ESTRUCTURA)); //asignacion de memoria del tamaño de la estructura castiandolo a una estructura
                                                        //para que me otorgue el bloque de memoria y me otorgue el primero del bloque de memoria
    if (!alumno) {//SI es 0 entoces no se asigno la memoria correctamente
        perror("Error al asignar memoria");
        return;
    }

    char otro = 's';    //igualando la variable otro a s para que entre al bucle

    while (otro == 's') {   //Se mantendra en el bucle mientras otros sea s

        //bloque para ingresar los datos en la direccion de memoria asignada
        printf("Ingrese el nombre del alumno: ");
        scanf("%s", alumno->nombre);
        fflush(stdin);
        printf("Ingrese la edad del alumno: ");
        scanf("%d", &alumno->edad);
        fflush(stdin);
        printf("Ingrese la nota del alumno: ");
        scanf("%d", &alumno->nota);
        fflush(stdin);

        fwrite(alumno, sizeof(ESTRUCTURA), 1, archivo);//Escribiendo lo que tiene la memoria asignada en el archivo

        printf("Desea ingresar otro registro? (s/n): ");
        scanf(" %c", &otro);    //Escaneo de la opcion ingresada para saber si se seguira en el bucle o no
    }

    free(alumno);   //liberando la memoria asignada
    fclose(archivo);    //cerrando el archivo que abrimos
}

void leerDisco(void) {

    FILE *archivo = fopen("alumno.dat", "rb");  
    if (!archivo) {
        perror("Error al abrir el archivo");
        return;
    }

    ESTRUCTURA *alumno; 
    alumno = (ESTRUCTURA*) malloc(sizeof(ESTRUCTURA));
    if (!alumno) {
        perror("Error al asignar memoria");
        return;
    }

    while (fread(alumno, sizeof(ESTRUCTURA), 1, archivo)) { //Lee la informacion del archivo
        printf("Nombre: %s, Edad: %d, Nota: %d\n", alumno->nombre, alumno->edad, alumno->nota); //Imprimiendo en pantalla lo ingresado
    }

    free(alumno);   //liberando la memoria
    fclose(archivo);
}

void salir(void) {
    system("cls");  //limpiando pantalla
    printf("\n\nSALIENDO DEL PROGRAMA.\n\n"); //INDICANDO QUE ESTAMOS SALIENDO DEL PROMGRAMA
}