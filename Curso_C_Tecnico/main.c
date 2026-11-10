#include <stdio.h>

#include "inventario.h"
#include "persistencia.h"
#include "reportes.h"
#include "utilidades.h"

#define RUTA_DATOS "datos/inventario.dat"

static void mostrar_menu(void);
static void agregar_producto(Inventario *inventario);
static void buscar_producto(Inventario *inventario);
static void actualizar_producto(Inventario *inventario);
static void eliminar_producto(Inventario *inventario);

int main(void)
{
    Inventario inventario;
    int opcion;
    int estado_carga;

    inventario_inicializar(&inventario);
    estado_carga = persistencia_cargar(&inventario, RUTA_DATOS);

    if (estado_carga < 0) {
        fprintf(stderr, "Error: no se pudo leer %s. No se modificaron los datos.\n", RUTA_DATOS);
        inventario_liberar(&inventario);
        return 1;
    }

    if (estado_carga == 0) {
        printf("No hay un inventario guardado. Se iniciara uno nuevo.\n");
    } else {
        printf("Inventario cargado: %zu producto(s).\n", inventario.cantidad);
    }

    do {
        mostrar_menu();
        opcion = leer_entero("Selecciona una opcion: ", 0, 7);

        switch (opcion) {
        case 1:
            reporte_listar(&inventario);
            break;
        case 2:
            agregar_producto(&inventario);
            break;
        case 3:
            buscar_producto(&inventario);
            break;
        case 4:
            actualizar_producto(&inventario);
            break;
        case 5:
            eliminar_producto(&inventario);
            break;
        case 6:
            inventario_ordenar_por_nombre(&inventario);
            reporte_listar(&inventario);
            break;
        case 7:
            reporte_resumen(&inventario);
            break;
        case 0:
            printf("Cerrando el programa...\n");
            break;
        default:
            printf("Opcion no valida.\n");
            break;
        }
    } while (opcion != 0);

    if (!persistencia_guardar(&inventario, RUTA_DATOS)) {
        fprintf(stderr, "Error: no se pudo guardar el inventario en %s.\n", RUTA_DATOS);
        inventario_liberar(&inventario);
        return 1;
    }

    inventario_liberar(&inventario);
    printf("Inventario guardado correctamente.\n");
    return 0;
}

static void mostrar_menu(void)
{
    printf("\n=== GESTOR DE INVENTARIO ===\n");
    printf("1. Listar productos\n");
    printf("2. Agregar producto\n");
    printf("3. Buscar producto por ID\n");
    printf("4. Actualizar existencias y precio\n");
    printf("5. Eliminar producto\n");
    printf("6. Ordenar por nombre y listar\n");
    printf("7. Ver resumen del inventario\n");
    printf("0. Guardar y salir\n");
}

static void agregar_producto(Inventario *inventario)
{
    Producto producto = {0};
    unsigned int id;

    if (!leer_texto("Nombre: ", producto.nombre, sizeof producto.nombre) ||
        !leer_texto("Categoria: ", producto.categoria, sizeof producto.categoria)) {
        printf("No se pudo leer la informacion del producto.\n");
        return;
    }

    producto.stock = leer_entero("Existencias: ", 0, 1000000000);
    producto.precio = leer_real("Precio por unidad: ", 0.0, 1000000000.0);

    if (!inventario_agregar(inventario, &producto, &id)) {
        printf("No se pudo agregar el producto: se agoto la memoria o el rango de identificadores.\n");
        return;
    }

    printf("Producto agregado con ID %u.\n", id);
}

static void buscar_producto(Inventario *inventario)
{
    unsigned int id = (unsigned int)leer_entero("ID del producto: ", 1, 2147483647);
    const Producto *producto = inventario_buscar_por_id(inventario, id);

    if (producto == NULL) {
        printf("No existe un producto con ese ID.\n");
        return;
    }

    reporte_mostrar_producto(producto);
}

static void actualizar_producto(Inventario *inventario)
{
    unsigned int id = (unsigned int)leer_entero("ID del producto: ", 1, 2147483647);
    Producto *producto = inventario_buscar_por_id(inventario, id);

    if (producto == NULL) {
        printf("No existe un producto con ese ID.\n");
        return;
    }

    printf("Producto seleccionado: %s\n", producto->nombre);
    producto->stock = leer_entero("Nuevas existencias: ", 0, 1000000000);
    producto->precio = leer_real("Nuevo precio por unidad: ", 0.0, 1000000000.0);
    printf("Producto actualizado.\n");
}

static void eliminar_producto(Inventario *inventario)
{
    unsigned int id = (unsigned int)leer_entero("ID del producto a eliminar: ", 1, 2147483647);

    if (inventario_eliminar(inventario, id)) {
        printf("Producto eliminado.\n");
    } else {
        printf("No existe un producto con ese ID.\n");
    }
}
