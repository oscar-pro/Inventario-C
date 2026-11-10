#ifndef REPORTES_H
#define REPORTES_H

#include "inventario.h"

void reporte_mostrar_producto(const Producto *producto);
void reporte_listar(const Inventario *inventario);
void reporte_resumen(const Inventario *inventario);

#endif
