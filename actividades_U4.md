# 4.1 Dispositivos y manejadores de dispositivos.

Para esta unidad vamos a comenzar hablando de los dos tipos de dispositivos de entrada/salida (e/s).

## Dispositivos de bloque.
Este tipo de dispositivo maneja los datos en segmentos con un tamaño fijo y se denominan como bloques. Estos bloques son las partes más pequeñas que el dispositivo puede leer y dan acceso a los datos de manera aleatoria y secuencial al momento que se accede al bloque individualmente; esta característica los hace ideales para gestionar bases de datos.

Un ejemplo de un dispositivo de bloque es un HDD o SDD ya que se puede acceder a los datos de manera aleatoria y no debes acceder a los datos anteiores para leer el dato requerido.

## Dispositivos de carácter.

Estos dispositivos manejan un flujo continuo de datos, son lineales, y se leen de manera secuencial, no tienen acceso aleatorio y permiten la lectura y escritura de bytes/caracteres.

El ejemplo más típico de un dispositivo de caracter es un teclado ya que el acceso a la escritura de los datos es secuencial y no puedes saltar al futuro para accder a una palabra que todavía no escribes.

>Nikolako. (2024, 18 abril). ¿Qué es un dispositivo de bloque? phoenixNAP IT Glossary. https://phoenixnap.mx/glosario/dispositivo-de-bloque
>Dispositivo de caracteres y dispositivo de bloque - programador clic. (s. f.). https://programmerclick.com/article/34671516564/

Ahora un ejemplo en C sobre un simulador de entrada de un dispositivo virtual:

```c
#include <stdio.h>
#include <string.h>

#define TAMANO_BUFFER 256
#define NOMBRE_DISPOSITIVO "DispositivoVirtual"

typedef struct {
    char buffer[TAMANO_BUFFER];
    int posicion;
} Dispositivo;

void inicializarDispositivo(Dispositivo *dispositivo) {
    memset(dispositivo->buffer, 0, TAMANO_BUFFER);
    dispositivo->posicion = 0;
    printf("Dispositivo %s inicializado.\n", NOMBRE_DISPOSITIVO);
}

int leerDesdeDispositivo(Dispositivo *dispositivo, char *destino, int tamano) {
    if (dispositivo->posicion + tamano > TAMANO_BUFFER) {
        tamano = TAMANO_BUFFER - dispositivo->posicion;
    }
    memcpy(destino, dispositivo->buffer + dispositivo->posicion, tamano);
    dispositivo->posicion += tamano;
    return tamano;
}

void escribirEnDispositivo(Dispositivo *dispositivo, const char *datos) {
    int tamano = strlen(datos);
    if (tamano > TAMANO_BUFFER) {
        tamano = TAMANO_BUFFER;
    }
    memcpy(dispositivo->buffer, datos, tamano);
    dispositivo->posicion = 0;
    printf("Datos escritos en el dispositivo: %s\n", datos);
}

int main() {
    Dispositivo dispositivo;
    inicializarDispositivo(&dispositivo);

    escribirEnDispositivo(&dispositivo, "Hola, este es un dispositivo virtual.");

    char destino[50];
    int leido = leerDesdeDispositivo(&dispositivo, destino, sizeof(destino) - 1);
    destino[leido] = '\0';

    printf("Datos leídos desde el dispositivo: %s\n", destino);

    return 0;
}
```
---

# 4.2  Mecanismos y funciones de los manejadores de dispositivos.

Para entender los mecanismos de los manejadores de E/S, analicemos su funcionamiento: 

Una interrupción por E/S es una señal enviada por un dispositivo de hardware (como un teclado, disco duro o impresora) al procesador para que tenga en cuenta que necesita atención. Estas interrupciones permiten al procesador que evite tener que esperar a que un dispositivo complete sus operaciones de E/S y que pueda seguir ejecutando otros procesos y atender al dispositivo cuando lo necesite.

También tiene un manejador de interrupciones que llama a la función específica que el sistema debe realizar o rutina de servicio de interrupciones (ISR). Una vez atendida la interrupción restaura el contexto con el apuntador de programa o el espacio en el bufer.

```
InicializarDispositivo(E/S)

ProcesoPrincipal() {
    mientras (true) {
        RealizarOperacionesPrincipales()
        
        si (InterrupcionE/S) {
            ManejadorInterrupcionE/S()
        }
    }
}

ManejadorInterrupcionE/S() {
    GuardarContexto()
    
    RealizarOperacionE/S()
    RestaurarContexto()
}

InicializarDispositivo(E/S) {
    Aqui se va a iniciar el dispositivo ya sea una impresora o un teclado.
}

RealizarOperacionesPrincipales() {
    Se inicia el ordenador.
}

RealizarOperacionE/S() {
    En el caso de un teclado, se activan funciones para escribir los bytes en la pantalla.
}

GuardarContexto() {
    Guarda el contexto (registro de CPU, contador de programa, etc.)
}

RestaurarContexto() {
    Restaura el contexto guardado
}
```

![Interrupción](Interrupcion.jpg "Interrupción")

>E/S por interrupción: gestión de interrupciones. (2013, 7 febrero). Tareas Universitarias. https://tareasuniversitarias.com/es-por-interrupcion-gestion-de-interrupciones.html

Veremos una simulación básica de cómo entraría una interrupción al sistema. OJO: el programa debe detenerse manualmente, de lo contrario no dejará de ejecutarse.

```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define TAMANO_BUFFER 256

typedef struct {
    char buffer[TAMANO_BUFFER];
    int posicion;
} Dispositivo;

Dispositivo dispositivo;

void inicializarDispositivo(Dispositivo *dispositivo) {
    memset(dispositivo->buffer, 0, TAMANO_BUFFER);
    dispositivo->posicion = 0;
    printf("Dispositivo inicializado.\n");
}

void manejarInterrupcion(int signal) {
    if (signal == SIGUSR1) {
        printf("Interrupción por E/S recibida.\n");
        leerDesdeDispositivo(&dispositivo);
    }
}

void leerDesdeDispositivo(Dispositivo *dispositivo) {
    printf("Leyendo desde el dispositivo...\n");
    strcpy(dispositivo->buffer, "Datos leídos del dispositivo\n");
    dispositivo->posicion = strlen(dispositivo->buffer);
    printf("Datos: %s\n", dispositivo->buffer);
}

void procesoPrincipal(Dispositivo *dispositivo) {
    while (1) {
        printf("Proceso principal ejecutándose...\n");
        sleep(2);

        raise(SIGUSR1);
    }
}

int main() {
    inicializarDispositivo(&dispositivo);

    signal(SIGUSR1, manejarInterrupcion);

    procesoPrincipal(&dispositivo);

    return 0;
}
```
---
# 4.3 Estructuras de datos para manejo de dispositivos.

Los dispositivos, al igual que los procesos, se manejan por colas de prioridades, organizando las más importantes hasta adelante y se van recorriendo hasta llegar a la menos importante. 

Una cola de E/S (Entrada/Salida) es una estructura de datos utilizada por los sistemas operativos para gestionar las solicitudes de E/S de los dispositivos periféricos, como discos duros, impresoras, y dispositivos de red. Las solicitudes de E/S se colocan en una cola y se procesan en orden. En algunos casos, las colas de E/S pueden manejar prioridades para garantizar que las solicitudes más importantes se atiendan primero.

>Tedhudek. (2023, 15 junio). Usos de ejemplo de colas de E/S - Windows drivers. Microsoft Learn. https://learn.microsoft.com/es-es/windows-hardware/drivers/wdf/example-uses-of-i-o-queues

Veamos un ejemplo con un código en C.

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct _nodo {
    int valor;
    int prioridad;
    struct _nodo *siguiente;
} nodo;
int numero_aleatorio() {
    return (rand() % 4) + 1;
}
nodo* genera_proceso() {
    static int contador = 1;
    nodo* nuevo = (nodo*)malloc(sizeof(nodo));
    if (nuevo == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }
    nuevo->valor = contador++;
    nuevo->prioridad = numero_aleatorio();
    nuevo->siguiente = NULL;
    return nuevo;
}
void insertar_final(nodo** cabeza) {
    nodo* nuevoNodo = genera_proceso();
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        nodo* temp = *cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
}
void imprimir_lista(nodo* cabeza) {
    nodo* temp = cabeza;
    while (temp != NULL) {
        printf("Proceso %d (Prioridad %d) -> ", temp->valor, temp->prioridad);
        temp = temp->siguiente;
    }
    printf("NULL\n");
}
void atender_prioridad_mas_alta(nodo** cabeza) {
    if (*cabeza == NULL) {
        printf("No hay procesos para atender.\n");
        return;
    }
    nodo* temp = *cabeza;
    nodo* maxNodo = temp;
    nodo* maxNodoPrevio = NULL;
    nodo* previo = NULL;
    // Buscar prioridad más alta
    while (temp != NULL) {
        if (temp->prioridad > maxNodo->prioridad) {
            maxNodo = temp;
            maxNodoPrevio = previo;
        }
        previo = temp;
        temp = temp->siguiente;
    }
    printf("Atendiendo proceso %d con prioridad %d\n", maxNodo->valor, maxNodo->prioridad);
    // Eliminar
    if (maxNodoPrevio == NULL) {
        *cabeza = maxNodo->siguiente; 
    } else {
        maxNodoPrevio->siguiente = maxNodo->siguiente;
    }
    free(maxNodo);
}
int main() {
    srand(time(NULL));
    nodo* cabeza = NULL;
    int op = 0;
    do {
        printf("1. Genera proceso\n");
        printf("2. Atiende proceso\n");
        printf("3. Mostrar Lista de Procesos\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &op);
        switch (op) {
            case 1:
                insertar_final(&cabeza);
                break;
            case 2:
                atender_prioridad_mas_alta(&cabeza);
                break;
            case 3:
                imprimir_lista(cabeza);
                break;
            case 4:
                printf("Adiós\n");
                break;
            default:
                printf("Opción inválida\n");
        }
    } while (op != 4);
    return 0;
}
```
---
# 4.4  Operaciones de Entrada/Salida.

Una operación muy frecuente que se usaba hace algunos años, era el escribir y leer datos de un disco magnético (HDD) ya que usaban cabezal y tenían todo un proceso que descubiré en seguida.

Cuando un sistema operativo lee un archivo desde un disco magnético, el proceso puede seguir estos pasos generales:

1. Solicitud de Lectura: Un programa olicita al sistema operativo que lea un archivo específico.

2. Traducción del Nombre del Archivo: El sistema operativo traduce el nombre del archivo a una dirección de bloque utilizando el sistema de archivos.

3. Posicionamiento del Cabezal: El controlador del disco mueve el cabezal de lectura/escritura a la posición correcta del disco.

4. Lectura del Bloque: El controlador del disco lee el bloque de datos en la posición especificada.

5. Transferencia de Datos: Los datos leídos se transfieren desde el disco a la memoria principal del sistema.

6. Entrega al Programa: El sistema operativo entrega los datos leídos al programa que hizo la solicitud.

Veamos un texto plano con el flujo del proceso:

```
[Inicio]
   |
   v
[Solicitud de Lectura del Archivo]
   |
   v
[Traducción del Nombre del Archivo a Dirección de Bloque]
   |
   v
[Posicionamiento del Cabezal del Disco]
   |
   v
[Lectura del Bloque desde el Disco]
   |
   v
[Transferencia de Datos a Memoria Principal]
   |
   v
[Entrega de Datos al Programa]
   |
   v
[Fin]
```
>Magnetic Storage: Devices, Advantages & Capacity | StudySmarter. (s. f.). StudySmarter UK. https://www-studysmarter-co-uk.translate.goog/explanations/computer-science/computer-organisation-and-architecture/magnetic-storage/?_x_tr_sl=en&_x_tr_tl=es&_x_tr_hl=es&_x_tr_pto=sge#:~:text=Data%20is%20read%20from%20magnetic,data%20(0s%20and%201s).

Ahora una vista básica en C:

```c
#include <stdio.h>
#include <string.h>

#define TAMANO_DISCO 1024
#define TAMANO_BLOQUE 64 

typedef struct {
    int bloque_id;
    char datos[TAMANO_BLOQUE];
} BloqueDisco;

typedef struct {
    BloqueDisco bloques[TAMANO_DISCO];
} Disco;

void inicializarDisco(Disco *disco) {
    for (int i = 0; i < TAMANO_DISCO; i++) {
        disco->bloques[i].bloque_id = i;
        sprintf(disco->bloques[i].datos, "Datos del bloque %d", i);
    }
}

void leerBloque(Disco *disco, int bloque_id, char *buffer) {
    int i;
    for (i = 0; i < TAMANO_BLOQUE; i++) {
        buffer[i] = disco->bloques[bloque_id].datos[i];
    }
    buffer[i] = '\0'; 
}

int main() {
    Disco disco;
    inicializarDisco(&disco);

    int bloque_id = 10;
    char buffer[TAMANO_BLOQUE];

    printf("Leyendo el bloque %d del disco...\n", bloque_id);
    leerBloque(&disco, bloque_id, buffer);

    printf("Datos leídos: %s\n", buffer);

    return 0;
}
```
---
# Integración.

código:

```c
#include <stdio.h>
#include <string.h>

#define TAMANO_DISCO 1024
#define TAMANO_BLOQUE 64 

typedef struct {
    int bloque_id;
    char datos[TAMANO_BLOQUE];
} BloqueDisco;

typedef struct {
    BloqueDisco bloques[TAMANO_DISCO];
} Disco;

void inicializarDisco(Disco *disco) {
    for (int i = 0; i < TAMANO_DISCO; i++) {
        disco->bloques[i].bloque_id = i;
        sprintf(disco->bloques[i].datos, "Datos del bloque %d", i);
    }
}

void leerBloque(Disco *disco, int bloque_id, char *buffer) {
    int i;
    for (i = 0; i < TAMANO_BLOQUE; i++) {
        buffer[i] = disco->bloques[bloque_id].datos[i];
    }
    buffer[i] = '\0'; 
}

int main() {
    Disco disco;
    inicializarDisco(&disco);

    int bloque_id = 10;
    char buffer[TAMANO_BLOQUE];

    printf("Leyendo el bloque %d del disco...\n", bloque_id);
    leerBloque(&disco, bloque_id, buffer);

    printf("Datos leídos: %s\n", buffer);

    return 0;
}
```

# Avanzado.

En pocas palabras, la gestión de la memoria se refiere a cómo un sistema operativo supervisa el uso de la memoria de la computadora. Garantiza que diferentes programas y tareas puedan acceder a lo que necesitan desde la memoria sin encontrar problemas. El sistema operativo utiliza varias técnicas para organizar y asignar recursos de memoria de manera eficiente para ayudar a garantizar un rendimiento informático fluido y evitar fallas.

La gestión de la memoria en los sistemas operativos es compleja e involucra varios componentes vitales que trabajan juntos para asignar y utilizar los recursos de memoria de manera efectiva.

![GestionE/S](GES.jpg "Gestión Entradas y salidas.")

>Aguilera, A. (2024, 26 enero). Gestión de memoria en el sistema operativo: lo que debes saber. Tecno Simple. https://tecno-simple.com/gestion-de-memoria-en-el-sistema-operativo-lo-que-debes-saber/#Gestion_de_memoria_en_sistemas_operativos_una_definicion_exacta