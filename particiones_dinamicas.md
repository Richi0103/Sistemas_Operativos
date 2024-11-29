# Simulador de memoria dinámica

se añadió malloc al número de particiones

```c
#include <stdio.h>
#include <stdlib.h>
typedef struct {
    int tamano;
    int id_de_proceso; 
} Particion;
void imprime_memoria(Particion* particiones, int numero_de_particiones) {
    printf("\nEstado de la memoria:\n");
    for (int i = 0; i < numero_de_particiones; i++) {
        if (particiones[i].id_de_proceso == -1) {
            printf("Partición %d: %d KB (Libre)\n", i + 1, particiones[i].tamano);
        } else {
            printf("Partición %d: %d KB (Proceso %d)\n", i + 1, particiones[i].tamano, particiones[i].id_de_proceso);
        }
    }
    printf("\n");
}
int main() {
    int memoria_total, numero_de_particiones;
    Particion* particiones;
    printf("Ingrese el tamaño total de la memoria (KB): ");
    scanf("%d", &memoria_total);
    printf("Ingrese el número de particiones: ");
    scanf("%d", &numero_de_particiones);
    
    // Asignar memoria para las particiones
    particiones = (Particion*)malloc(numero_de_particiones * sizeof(Particion));
    if (particiones == NULL) {
        printf("Error al asignar memoria para las particiones.\n");
        return 1;
    }
    int tamano_total_particion = 0;
    for (int i = 0; i < numero_de_particiones; i++) {
        printf("Ingrese el tamaño de la partición %d (KB): ", i + 1);
        scanf("%d", &particiones[i].tamano);
        particiones[i].id_de_proceso = -1;
        tamano_total_particion += particiones[i].tamano;
    }
    if (tamano_total_particion > memoria_total) {
        printf("Error: El tamaño total de las particiones excede el tamaño de la memoria.\n");
        free(particiones);
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
                    if (particiones[i].id_de_proceso == -1 && particiones[i].tamano >= tam_proceso) {
                        particiones[i].id_de_proceso = id_de_proceso;
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
                    if (particiones[i].id_de_proceso == id_de_proceso) {
                        particiones[i].id_de_proceso = -1;
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
                imprime_memoria(particiones, numero_de_particiones);
                break;
            case 4:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (op != 4);
    free(particiones);
    return 0;
}
```