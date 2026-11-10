#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stddef.h>

int leer_linea(const char *mensaje, char *destino, size_t capacidad);
int leer_entero(const char *mensaje, int minimo, int maximo);
double leer_real(const char *mensaje, double minimo, double maximo);
int leer_texto(const char *mensaje, char *destino, size_t capacidad);

#endif
