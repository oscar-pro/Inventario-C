#include "reportes.h"

#include <stdio.h>

void reporte_mostrar_producto(const Producto *producto)
{
    printf("\nID: %u\n", producto->id);
    printf("Nombre: %s\n", producto->nombre);
    printf("Categoria: %s\n", producto->categoria);
    printf("Existencias: %d\n", producto->stock);
    printf("Precio: $%.2f\n", producto->precio);
}

void reporte_listar(const Inventario *inventario)
{
    size_t indice;

    if (inventario->cantidad == 0) {
        printf("El inventario esta vacio.\n");
        return;
    }

    printf("\n%-6s %-24s %-18s %10s %12s\n",
           "ID", "NOMBRE", "CATEGORIA", "EXISTENCIAS", "PRECIO");
    printf("--------------------------------------------------------------------------\n");

    for (indice = 0; indice < inventario->cantidad; indice++) {
        const Producto *producto = &inventario->productos[indice];
        printf("%-6u %-24.24s %-18.18s %10d $%11.2f\n",
               producto->id, producto->nombre, producto->categoria,
               producto->stock, producto->precio);
    }
}

void reporte_resumen(const Inventario *inventario)
{
    size_t indice;
    size_t unidades = 0;
    double valor_total = 0.0;
    size_t productos_bajos = 0;

    for (indice = 0; indice < inventario->cantidad; indice++) {
        const Producto *producto = &inventario->productos[indice];
        unidades += (size_t)producto->stock;
        valor_total += producto->precio * producto->stock;
        if (producto->stock <= 5) {
            productos_bajos++;
        }
    }

    printf("\n=== RESUMEN DEL INVENTARIO ===\n");
    printf("Tipos de producto: %zu\n", inventario->cantidad);
    printf("Unidades almacenadas: %zu\n", unidades);
    printf("Productos con cinco unidades o menos: %zu\n", productos_bajos);
    printf("Valor total estimado: $%.2f\n", valor_total);
}
