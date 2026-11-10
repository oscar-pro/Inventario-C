#include "utilidades.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int leer_linea(const char *mensaje, char *destino, size_t capacidad)
{
    size_t longitud;
    int caracter;

    if (destino == NULL || capacidad == 0) {
        return 0;
    }

    printf("%s", mensaje);
    if (fgets(destino, (int)capacidad, stdin) == NULL) {
        return 0;
    }

    longitud = strlen(destino);
    if (longitud > 0 && destino[longitud - 1] == '\n') {
        destino[longitud - 1] = '\0';
    } else {
        while ((caracter = getchar()) != '\n' && caracter != EOF) {
            /* Se descarta el resto de una linea demasiado larga. */
        }
    }

    return 1;
}

int leer_entero(const char *mensaje, int minimo, int maximo)
{
    char linea[128];

    for (;;) {
        char *final;
        long valor;

        if (!leer_linea(mensaje, linea, sizeof linea)) {
            printf("\nNo se recibio una entrada. Se usara el valor minimo.\n");
            return minimo;
        }
        errno = 0;
        valor = strtol(linea, &final, 10);
        while (isspace((unsigned char)*final)) {
            final++;
        }

        if (linea[0] != '\0' && *final == '\0' && errno != ERANGE &&
            valor >= minimo && valor <= maximo) {
            return (int)valor;
        }

        printf("Ingresa un numero entero entre %d y %d.\n", minimo, maximo);
    }
}

double leer_real(const char *mensaje, double minimo, double maximo)
{
    char linea[128];

    for (;;) {
        char *final;
        double valor;

        if (!leer_linea(mensaje, linea, sizeof linea)) {
            printf("\nNo se recibio una entrada. Se usara el valor minimo.\n");
            return minimo;
        }

        errno = 0;
        valor = strtod(linea, &final);
        while (isspace((unsigned char)*final)) {
            final++;
        }

        if (linea[0] != '\0' && *final == '\0' && errno != ERANGE &&
            isfinite(valor) && valor >= minimo && valor <= maximo) {
            return valor;
        }

        printf("Ingresa un numero entre %.2f y %.2f.\n", minimo, maximo);
    }
}

int leer_texto(const char *mensaje, char *destino, size_t capacidad)
{
    size_t indice;

    for (;;) {
        if (!leer_linea(mensaje, destino, capacidad)) {
            return 0;
        }

        for (indice = 0; destino[indice] != '\0'; indice++) {
            if (!isspace((unsigned char)destino[indice])) {
                return 1;
            }
        }

        printf("El texto no puede estar vacio.\n");
    }
}
