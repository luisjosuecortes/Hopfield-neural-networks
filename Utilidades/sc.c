// Seleccion De Coordenadas
// Luis Josué Cortés Anzurez
// 2do Semestre
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>

#define FILAS 25
#define COLUMNAS 25
#define MAX_SELECCIONES 625

struct Coordenada {
    int x;
    int y;
};

void mostrar_matriz(char matriz[FILAS][COLUMNAS]);
void leer_entrada_usuario(char matriz[FILAS][COLUMNAS], int *posX, int *posY, struct Coordenada selecciones[MAX_SELECCIONES], int *numSelecciones);
void imprimir_lista(char matriz[FILAS][COLUMNAS]);
char getch();
bool kbhit();

int main() {
    char matriz[FILAS][COLUMNAS];
    int i, j;
    int posX = FILAS / 2;
    int posY = COLUMNAS / 2;
    int numSelecciones = 0;
    struct Coordenada selecciones[MAX_SELECCIONES];

    // Inicializar la matriz con un caracter vacio " "
    for (i = 0; i < FILAS; i++) {
        for (j = 0; j < COLUMNAS; j++) {
            matriz[i][j] = ' ';
        }
    }

    matriz[posX][posY] = 'X'; // Indicar la posición inicial

    // Loop principal
    while (1) {
        system("clear");
        mostrar_matriz(matriz);
        leer_entrada_usuario(matriz, &posX, &posY, selecciones, &numSelecciones);
        usleep(100000);
    }

    return 0;
}

void mostrar_matriz(char matriz[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%s", matriz[i][j] == 'X' ? " ■" : matriz[i][j] == 'S' ? " ■" : " □");
        }
        printf("\n");
    }
}

void leer_entrada_usuario(char matriz[FILAS][COLUMNAS], int *posX, int *posY, struct Coordenada selecciones[MAX_SELECCIONES], int *numSelecciones) {
    if (kbhit()) {
        char c = getch();
        if (c == 'q') {
            imprimir_lista(matriz);
            exit(0);
        } else if (c == '\n' && *numSelecciones < MAX_SELECCIONES) {
            struct Coordenada nuevaSeleccion = {*posX, *posY};
            selecciones[*numSelecciones] = nuevaSeleccion;
            (*numSelecciones)++;
        } else if (c == 'w' && *posX > 0)
            (*posX)--;
        else if (c == 's' && *posX < FILAS - 1)
            (*posX)++;
        else if (c == 'a' && *posY > 0)
            (*posY)--;
        else if (c == 'd' && *posY < COLUMNAS - 1)
            (*posY)++;

        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS; j++) {
                matriz[i][j] = ' ';
            }
        }

        for (int i = 0; i < *numSelecciones; i++) {
            matriz[selecciones[i].x][selecciones[i].y] = 'S';
        }

        matriz[*posX][*posY] = 'X';
    }
}

void imprimir_lista(char matriz[FILAS][COLUMNAS]) {
    printf("[");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (matriz[i][j] == 'S') {
                printf("+1");
            } else {
                printf("-1");
            }
            if (i < FILAS - 1 || j < COLUMNAS - 1) {
                printf(", ");
            }
        }
    }
    printf("]\n");
}

char getch() {
    struct termios oldAttr, newAttr;
    tcgetattr(STDIN_FILENO, &oldAttr);
    newAttr = oldAttr;
    newAttr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);
    char c = getchar();
    return c;
}

bool kbhit() {
    static bool first = true;
    if (first) {
        setbuf(stdin, NULL);
        struct termios newAttr;
        tcgetattr(STDIN_FILENO, &newAttr);
        newAttr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);
        first = false;
    }
    int bytes;
    ioctl(STDIN_FILENO, FIONREAD, &bytes);
    return bytes > 0;
}