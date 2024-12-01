# 3.1 Politica y filosofía.

Para empezar, hablemos de los tipos de fragmentación que puede tener la memoria que son dos: frgamentación interna y fragmentación externa.

## Fragmentación interna.

La fragmentación interna se basa en asignar bloques de memoria con particiones fijas, pero muchas veces le da a los procesos más memoria de la que realmente necesita.

Este tipo de fragmentación es muy ineficiente ya que el hecho de que las particiones sean fijas y divididas casi del mismo tamaño, hace que los procesos pequeños tengan memoria desperdiciada que se puede usar para que entren otros procesos.

## Fragmentación externa.

Este tipo de fragmentación se encarga de dividir la memoria dinámicamente ya que la particiona de tal manera que se asigna la memoria exacta que el proceso necesita.

El uso de esta fragmentación tiene ventajas y desventajas; una de sus ventajas es que no hay memoria desperdicada y por eso puede tener más procesos ejecutandose simultáneamente, pero esto resulta en que el sistema tiene que hacer más operaciones de gestión de memoria que puede afectar al rendimiento del equipo.

![Fragmentaciones](Fragmentacion.jpg "Peterson")

> Fragmentación interna y externa :: Administración de Sistemas Informáticos. (s. f.). https://asirclaret-com.webnode.es/news/fragmentacion-interna-y-externa/

---
## Politicas de reemplazo de páginas.

Ahora hablaremos de las politicas de reemplazo de páginas en sistemas operativos. Estas politicas son algoritmos que administran que página debe ser reemplazada cuando entra una nueva, hablaré de los 5 algoritmos más utilizados:

- Primeras entradas, primeras salidas (FIFO): Este algoritmo se basa como funcionan las filas en la vida real, las primeras páginas en llegar, son las primeras que se atienden y se liberan y una vez que se atendió va con la segunda que llegó y así sucesivamente. Este algoritmo no considera el tiempo en el que se atiende.
- Menos recientemente utilizada (LRU): El LRU es más eficiente que FIFO y fácil de implementar, se basa en ver cuál es la página menos utilizada y esa es la que reemplaza.
- Menor frecuencia de uso (MFU): La lógica que usa MFU es que las paginas menos utilizadas son las que les da menor importancia.
- Teoricamente óptico (OPT): En teoria, este es el algoritmo más eficiente, pero es prácticamente imposible de utilizar en los sistemas operativos y la razón es que este algoritmo intenta predecir cuál será la página que más se va a utilizar para darle más prioridad. 
- Algoritmo de conjunto de trabajo (WSC): Divide la memoria en conjuntos de trabajo y reemplaza las páginas dentro de cada conjunto. Es una combinación de LRU y MFU y es eficiente para sistemas con múltiples procesos.

Para mí, el algoritmo más eficiente de utilizar es el LRU, ya que es muy fácil de implementar y es eficiente porque considera un patrón de uso de cada página.

> Algoritmo de reemplazo de página _ AcademiaLab. (s. f.). https://academia-lab.com/enciclopedia/algoritmo-de-reemplazo-de-pagina/

---
# 3.2 Memoria real

A continuación, un programa en C que simula la administración de
memoria mediante particiones fijas.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTICIONES 100

typedef struct {
    int tamano;
    int id_de_proceso; 
} Particion;

void imprime_memoria(Particion Particions[], int numero_de_particiones) {
    printf("\nEstado de la memoria:\n");
    for (int i = 0; i < numero_de_particiones; i++) {
        if (Particions[i].id_de_proceso == -1) {
            printf("Partición %d: %d KB (Libre)\n", i + 1, Particions[i].tamano);
        } else {
            printf("Partición %d: %d KB (Proceso %d)\n", i + 1, Particions[i].tamano, Particions[i].id_de_proceso);
        }
    }
    printf("\n");
}

int main() {
    int memoria_total, numero_de_particiones;
    Particion Particions[MAX_PARTICIONES];

    printf("Ingrese el tamaño total de la memoria (KB): ");
    scanf("%d", &memoria_total);

    printf("Ingrese el número de particiones: ");
    scanf("%d", &numero_de_particiones);

    if (numero_de_particiones > MAX_PARTICIONES) {
        printf("Número máximo de particiones excedido (%d).\n", MAX_PARTICIONES);
        return 1;
    }

    int tamano_total_particion = 0;
    for (int i = 0; i < numero_de_particiones; i++) {
        printf("Ingrese el tamaño de la partición %d (KB): ", i + 1);
        scanf("%d", &Particions[i].tamano);
        Particions[i].id_de_proceso = -1;
        tamano_total_particion += Particions[i].tamano;
    }

    if (tamano_total_particion > memoria_total) {
        printf("Error: El tamaño total de las particiones excede el tamaño de la memoria.\n");
        return 1;
    }

    int op;
    do {
        printf("\n--- Menú ---\n");
        printf("1. Asignar proceso\n");
        printf("2. Liberar proceso\n");
        printf("3. Mostrar estado de la memoria\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &op);

        switch (op) {
            case 1: {
                int id_de_proceso, tam_proceso;
                printf("Ingrese el ID del proceso: ");
                scanf("%d", &id_de_proceso);
                printf("Ingrese el tamaño del proceso (KB): ");
                scanf("%d", &tam_proceso);

                int asignado = 0;
                for (int i = 0; i < numero_de_particiones; i++) {
                    if (Particions[i].id_de_proceso == -1 && Particions[i].tamano >= tam_proceso) {
                        Particions[i].id_de_proceso = id_de_proceso;
                        asignado = 1;
                        printf("Proceso %d asignado a la partición %d.\n", id_de_proceso, i + 1);
                        break;
                    }
                }
                if (!asignado) {
                    printf("No se encontró una partición disponible para el proceso %d.\n", id_de_proceso);
                }
                break;
            }
            case 2: {
                int id_de_proceso;
                printf("Ingrese el ID del proceso a liberar: ");
                scanf("%d", &id_de_proceso);

                int libre = 0;
                for (int i = 0; i < numero_de_particiones; i++) {
                    if (Particions[i].id_de_proceso == id_de_proceso) {
                        Particions[i].id_de_proceso = -1;
                        libre = 1;
                        printf("Proceso %d liberado de la partición %d.\n", id_de_proceso, i + 1);
                        break;
                    }
                }
                if (!libre) {
                    printf("No se encontró el proceso %d en ninguna partición.\n", id_de_proceso);
                }
                break;
            }
            case 3:
                imprime_memoria(Particions, numero_de_particiones);
                break;
            case 4:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (op != 4);

    return 0;
}
```

Ahora un algoritmo para calcular qué procesos pueden ser asignados
a un sistema con memoria real limitada utilizando el algoritmo de
"primera cabida".

```c
#include <stdio.h>

#define MAX_PROCESSES 10

void first_fit(int memory_size, int process_sizes[], int num_processes) {
    int memory[MAX_PROCESSES];
    int assignments[MAX_PROCESSES];

    memory[0] = memory_size;
    for (int i = 1; i < MAX_PROCESSES; i++) {
        memory[i] = 0;
    }

    for (int i = 0; i < num_processes; i++) {
        assignments[i] = -1; 
        for (int j = 0; j < MAX_PROCESSES; j++) {
            if (memory[j] >= process_sizes[i]) {
                assignments[i] = j; 
                memory[j] -= process_sizes[i]; 
                if (memory[j] == 0 && j < MAX_PROCESSES - 1) {
                    for (int k = j; k < MAX_PROCESSES - 1; k++) {
                        memory[k] = memory[k + 1];
                    }
                    memory[MAX_PROCESSES - 1] = 0;
                }
                break;
            }
        }
    }

    for (int i = 0; i < num_processes; i++) {
        if (assignments[i] != -1) {
            printf("Proceso %d asignado al bloque %d\n", i+1, assignments[i]);
        } else {
            printf("Proceso %d no pudo ser asignado\n", i);
        }
    }
}

int main() {
    int memory_size = 100;
    int process_sizes[] = {10, 20, 30, 40};
    int num_processes = sizeof(process_sizes) / sizeof(process_sizes[0]);

    first_fit(memory_size, process_sizes, num_processes);

    return 0;
}
```
---
# 3.3 Organización de memoria virtual.

Para entender la memoria virtual hay que tener dos conceptos claros, la diferencia entre paginación y segmentación.

## Paginación

Esta tecnica funciona algo similar a la fragmentación interna, divide la memoria en bloques de tamaños fijos llamados **páginas**, son del mismo tamaño y se le asigna un proceso a cada página. La memoria física (HD) también se divide en bloques iguales llamados **marcos**.

#### Ventajas:

- Facilidad de gestión de memoria.
- No hay fragmentación externa.
- Protección de memoria.

#### Desventajas:

- Puede cambiar a fragmentación interna si no coincide el tamaño con la página y se dividirá diferente la memoria.
- La paginación multinivel puede provocar una sobrecarga de referencias de memoria.

## Segmentación

La segmentación es una tecnica que divide la memoria en segmentos de tamaño variable, cada segmento corresponde a una parte lógica del programa como el código, los datos, la pila, etc. y cada segmento tiene un punto de inicio y un tamaño.

#### Ventajas:

- Cada segmento de memoria tiene sus permisos.
- Reduce la fragmentación interna.
- Modularidad.

#### Desventajas:

- Complejidad.
- Puede consumir memoria adicional para algunos procesos.
---
Aquí un ejemplo de una pequeña simulación de memoria virtual donde se le asigna a una página un proceso y lo busca.

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGE_SIZE 1024 
#define NUM_PAGES 16 
#define MEMORY_SIZE (PAGE_SIZE * NUM_PAGES) 

typedef struct {
    int num; 
    int valid;
} PageTableEntry;

void iniciar_tabla(PageTableEntry *pag_tabla, int numero_pag) {
    for (int i = 0; i < numero_pag; i++) {
        pag_tabla[i].num = -1;
        pag_tabla[i].valid = 0; 
    }
}

void acceso_memoria(PageTableEntry *pagina, int numero_pag) {
    if (pagina[numero_pag].valid) {
        printf("Página %d está en el marco %d.\n", numero_pag, pagina[numero_pag].num);
    } else {
        printf("Página %d no está en memoria.\n", numero_pag);
        pagina[numero_pag].num = rand() % NUM_PAGES;
        pagina[numero_pag].valid = 1;
        printf("Cargando página %d en el marco %d.\n", numero_pag, pagina[numero_pag].num);
    }
}

int main() {
    srand(time(NULL));

    PageTableEntry pag_tabla[NUM_PAGES];
    iniciar_tabla(pag_tabla, NUM_PAGES);

    for (int i = 0; i < 10; i++) {
        int random_pag = rand() % NUM_PAGES;
        printf("Accediendo a la página %d...\n", random_pag);
        acceso_memoria(pag_tabla, random_pag);
    }

    return 0;
}
```

> Brooks, N. (2024, 12 agosto). Paging in Operating System (OS): What is, Advantages, Example. Guru99. https://www.guru99.com/es/paging-in-operating-system.html

> ¿Cómo se compara la segmentación con otras técnicas de administración de memoria, como la paginación o la memoria virtual? (2023, 4 marzo). www.linkedin.com. https://es.linkedin.com/advice/3/how-do-you-compare-segmentation-other-memory?lang=es
---
# 3.4 Administrador de memoria virtual.

Un ejemplo en C sobre un algoritmo de administrador de memoria que vimos previamente "Least Recently Used" (LRU).

```c
#include <stdio.h>
#include <stdlib.h>

#define NUM_MARCOS 4 
#define NUM_REFERENCIAS 10 

void imprimirMarcos(int marcos[], int num_marcos) {
    printf("[");
    for (int i = 0; i < num_marcos; i++) {
        if (marcos[i] == -1) {
            printf(" - ");
        } else {
            printf(" %d ", marcos[i]);
        }
    }
    printf("]\n");
}

int encontrarLRU(int tiempos[], int num_marcos) {
    int minimo = tiempos[0], posicion = 0;
    for (int i = 1; i < num_marcos; i++) {
        if (tiempos[i] < minimo) {
            minimo = tiempos[i];
            posicion = i;
        }
    }
    return posicion;
}

void LRU(int paginas[], int num_paginas, int num_marcos) {
    int marcos[num_marcos];
    int tiempos[num_marcos];
    int contador = 0, fallos_pagina = 0;

    for (int i = 0; i < num_marcos; i++) {
        marcos[i] = -1;
    }

    for (int i = 0; i < num_paginas; i++) {
        int encontrado = 0;
        for (int j = 0; j < num_marcos; j++) {
            if (marcos[j] == paginas[i]) {
                encontrado = 1;
                tiempos[j] = ++contador;
                break;
            }
        }

        if (!encontrado) {
            int posicion = encontrarLRU(tiempos, num_marcos);
            marcos[posicion] = paginas[i];
            tiempos[posicion] = ++contador;
            fallos_pagina++;
        }

        printf("Referencia a página %d: ", paginas[i]);
        imprimirMarcos(marcos, num_marcos);
    }

    printf("\nTotal de fallos de página: %d\n", fallos_pagina);
}

int main() {
    int paginas[NUM_REFERENCIAS] = {2, 3, 4, 2, 1, 3, 2, 1, 4, 3};
    LRU(paginas, NUM_REFERENCIAS, NUM_MARCOS);
    return 0;
}
```

Seguimos con una imágen del proceso de traducción de direcciones virtuales a físicas en un sistema con memoria virtual.

![Diagrama](Diagrama.jpg "Diagrama")
---
# Integración.

Vamos a analizar el sistema operativo Linux, vamos a ver la manera en la que administra la memoria virtual.

En Linux, la memoria virtual se gestiona a través de un componente de hardware llamado Unidad de Administración de Memoria (MMU). 

La MMU convierte las direcciones de memoria física en direcciones virtuales, permitiendo que el sistema operativo utilice tanto la memoria RAM como el espacio en el disco duro o SSD como memoria virtual. Numeraré dos puntos importantes que la MMU hace:

- Swapping: cuando la RAM esta llena, se utiliza parte del SSD para continuar los procesos.
- Páginas y Espacio de Direcciones: La memoria virtual se divide en páginas, que pueden estar ubicadas en la RAM, el disco duro o el SSD.

>Gestionar y optimizar la memoria en Linux: Site24x7. (s. f.). Site24x7. https://www.site24x7.com/es/learn/linux/optimize-memory.html

Para entender un poco más de lo que trata el Swapping, a continuación un pequeño código en C:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_MEMORIA 100 
#define NUM_PROCESOS 5    

typedef struct {
    char nombre[10];
    int tamano;
    int en_memoria;
} Proceso;

void imprimirEstado(Proceso procesos[], int num_procesos) {
    printf("\nEstado de los procesos:\n");
    for (int i = 0; i < num_procesos; i++) {
        printf("Proceso %s - Tamaño: %d - En memoria: %s\n", 
               procesos[i].nombre, procesos[i].tamano, 
               procesos[i].en_memoria ? "Sí" : "No");
    }
}

void cargarEnMemoria(Proceso *proceso) {
    proceso->en_memoria = 1;
    printf("Cargando proceso %s en memoria...\n", proceso->nombre);
}

void moverADisco(Proceso *proceso) {
    proceso->en_memoria = 0;
    printf("Moviendo proceso %s a disco...\n", proceso->nombre);
}

int main() {
    Proceso procesos[NUM_PROCESOS] = {
        {"A", 30, 0},
        {"B", 40, 0},
        {"C", 20, 0},
        {"D", 50, 0},
        {"E", 10, 0}
    };

    int memoria_disponible = TAMANO_MEMORIA;
    int intercambios = 0;

    for (int i = 0; i < NUM_PROCESOS; i++) {
        if (procesos[i].tamano <= memoria_disponible) {
            cargarEnMemoria(&procesos[i]);
            memoria_disponible -= procesos[i].tamano;
        } else {
            printf("No hay suficiente memoria para el proceso %s. Necesario: %d, Disponible: %d\n", 
                   procesos[i].nombre, procesos[i].tamano, memoria_disponible);

            for (int j = 0; j < i; j++) {
                if (procesos[j].en_memoria) {
                    moverADisco(&procesos[j]);
                    memoria_disponible += procesos[j].tamano;
                    intercambios++;
                    break;
                }
            }
            cargarEnMemoria(&procesos[i]);
            memoria_disponible -= procesos[i].tamano;
        }
        imprimirEstado(procesos, NUM_PROCESOS);
    }

    printf("\nTotal de intercambios realizados: %d\n", intercambios);

    return 0;
}
```