#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "inventario.h"

/* Devuelve 1 si carga, 0 si el archivo no existe y -1 si hay un error. */
int persistencia_cargar(Inventario *inventario, const char *ruta);
int persistencia_guardar(const Inventario *inventario, const char *ruta);

#endif
