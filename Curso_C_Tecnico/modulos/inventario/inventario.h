#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <stddef.h>

#define PRODUCTO_NOMBRE_MAX 64
#define PRODUCTO_CATEGORIA_MAX 40

typedef struct {
    unsigned int id;
    char nombre[PRODUCTO_NOMBRE_MAX];
    char categoria[PRODUCTO_CATEGORIA_MAX];
    int stock;
    double precio;
} Producto;

typedef struct {
    Producto *productos;
    size_t cantidad;
    size_t capacidad;
    unsigned int siguiente_id;
} Inventario;

void inventario_inicializar(Inventario *inventario);
void inventario_liberar(Inventario *inventario);
int inventario_agregar(Inventario *inventario, const Producto *producto, unsigned int *id_generado);
Producto *inventario_buscar_por_id(Inventario *inventario, unsigned int id);
int inventario_eliminar(Inventario *inventario, unsigned int id);
void inventario_ordenar_por_nombre(Inventario *inventario);

#endif
