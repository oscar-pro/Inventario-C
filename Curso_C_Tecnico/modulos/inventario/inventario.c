#include "inventario.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

void inventario_inicializar(Inventario *inventario)
{
    inventario->productos = NULL;
    inventario->cantidad = 0;
    inventario->capacidad = 0;
    inventario->siguiente_id = 1;
}

void inventario_liberar(Inventario *inventario)
{
    free(inventario->productos);
    inventario_inicializar(inventario);
}

static int inventario_reservar(Inventario *inventario, size_t capacidad_minima)
{
    size_t nueva_capacidad;
    Producto *nuevos_productos;

    if (inventario->capacidad >= capacidad_minima)
    {
        return 1;
    }

    nueva_capacidad = inventario->capacidad == 0 ? 8 : inventario->capacidad;
    while (nueva_capacidad < capacidad_minima)
    {
        if (nueva_capacidad > (size_t)-1 / 2)
        {
            nueva_capacidad = capacidad_minima;
            break;
        }
        nueva_capacidad *= 2;
    }

    if (nueva_capacidad > (size_t)-1 / sizeof *nuevos_productos)
    {
        return 0;
    }

    nuevos_productos = realloc(inventario->productos,
                               nueva_capacidad * sizeof *nuevos_productos);
    if (nuevos_productos == NULL)
    {
        return 0;
    }

    inventario->productos = nuevos_productos;
    inventario->capacidad = nueva_capacidad;
    return 1;
}

int inventario_agregar(Inventario *inventario, const Producto *producto,
                       unsigned int *id_generado)
{
    Producto nuevo_producto;

    if (inventario->siguiente_id == 0 || inventario->siguiente_id == UINT_MAX ||
        !inventario_reservar(inventario, inventario->cantidad + 1))
    {
        return 0;
    }

    nuevo_producto = *producto;
    nuevo_producto.id = inventario->siguiente_id;
    inventario->productos[inventario->cantidad] = nuevo_producto;
    inventario->cantidad++;

    if (id_generado != NULL)
    {
        *id_generado = nuevo_producto.id;
    }
    inventario->siguiente_id++;
    return 1;
}

Producto *inventario_buscar_por_id(Inventario *inventario, unsigned int id)
{
    size_t indice;

    for (indice = 0; indice < inventario->cantidad; indice++)
    {
        if (inventario->productos[indice].id == id)
        {
            return &inventario->productos[indice];
        }
    }

    return NULL;
}

int inventario_eliminar(Inventario *inventario, unsigned int id)
{
    size_t indice;

    for (indice = 0; indice < inventario->cantidad; indice++)
    {
        if (inventario->productos[indice].id == id)
        {
            size_t elementos_restantes = inventario->cantidad - indice - 1;
            memmove(&inventario->productos[indice],
                    &inventario->productos[indice + 1],
                    elementos_restantes * sizeof *inventario->productos);
            inventario->cantidad--;
            return 1;
        }
    }

    return 0;
}

static int comparar_productos_por_nombre(const void *izquierda, const void *derecha)
{
    const Producto *producto_izquierdo = izquierda;
    const Producto *producto_derecho = derecha;

    return strcmp(producto_izquierdo->nombre, producto_derecho->nombre);
}

void inventario_ordenar_por_nombre(Inventario *inventario)
{
    if (inventario->cantidad < 2)
    {
        return;
    }

    qsort(inventario->productos, inventario->cantidad,
          sizeof *inventario->productos, comparar_productos_por_nombre);
}
