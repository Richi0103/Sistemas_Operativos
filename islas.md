# Conteo de islas

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FILAS 6
#define COLUMNAS 8
void generarMatriz(int matriz[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            matriz[i][j] = 0;
        }
    }
    srand(time(NULL));
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (rand() % 100 < 10) {
                matriz[i][j] = 1;
                if (i > 0 && rand() % 100 < 50) matriz[i - 1][j] = 1; // Arriba
                if (j > 0 && rand() % 100 < 50) matriz[i][j - 1] = 1; // Izquierda
                if (i < FILAS - 1 && rand() % 100 < 50) matriz[i + 1][j] = 1; // Abajo
                if (j < COLUMNAS - 1 && rand() % 100 < 50) matriz[i][j + 1] = 1; // Derecha
            }
        }
    }
}
void imprimirMatriz(int matriz[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}
// Función DFS
void dfs(int matriz[FILAS][COLUMNAS], int visitado[FILAS][COLUMNAS], int x, int y) {
    // Direcciones para moverse
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    // Marcar la celda actual como visitada
    visitado[x][y] = 1;
    // Explorar las 8 direcciones posibles
    for (int d = 0; d < 8; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < FILAS && ny >= 0 && ny < COLUMNAS && 
            matriz[nx][ny] == 1 && !visitado[nx][ny]) {
            dfs(matriz, visitado, nx, ny); //recursividad
        }
    }
}
void contarIslas(int matriz[FILAS][COLUMNAS]) {
    int visitado[FILAS][COLUMNAS] = {0}; 
    int total_islas = 0;
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (matriz[i][j] == 1 && !visitado[i][j]) {
                total_islas++;
                dfs(matriz, visitado, i, j); 
            }
        }
    }
    printf("\nLa cantidad de islas es: %d\n", total_islas);
}
int main() {
    int matriz[FILAS][COLUMNAS];
    generarMatriz(matriz);
    imprimirMatriz(matriz);
    contarIslas(matriz);
    return 0;
}
```