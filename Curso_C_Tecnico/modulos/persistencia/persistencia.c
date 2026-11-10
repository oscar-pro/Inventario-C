#include "persistencia.h"

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const unsigned char FIRMA_ARCHIVO[8] = {'I', 'N', 'V', 'C', '0', '1', '\r', '\n'};
static const uint32_t VERSION_ARCHIVO = 1;
static const uint32_t MAX_PRODUCTOS_ARCHIVO = 1000000;

static int escribir_dato(FILE *archivo, const void *dato, size_t tamano)
{
    return fwrite(dato, tamano, 1, archivo) == 1;
}

static int leer_dato(FILE *archivo, void *dato, size_t tamano)
{
    return fread(dato, tamano, 1, archivo) == 1;
}

int persistencia_guardar(const Inventario *inventario, const char *ruta)
{
    FILE *archivo;
    uint32_t version = VERSION_ARCHIVO;
    uint32_t cantidad;
    uint32_t siguiente_id = inventario->siguiente_id;
    size_t indice;
    int correcto = 1;

    if (inventario->cantidad > UINT32_MAX) {
        return 0;
    }

    archivo = fopen(ruta, "wb");
    if (archivo == NULL) {
        return 0;
    }

    cantidad = (uint32_t)inventario->cantidad;
    correcto = escribir_dato(archivo, FIRMA_ARCHIVO, sizeof FIRMA_ARCHIVO) &&
               escribir_dato(archivo, &version, sizeof version) &&
               escribir_dato(archivo, &cantidad, sizeof cantidad) &&
               escribir_dato(archivo, &siguiente_id, sizeof siguiente_id);

    for (indice = 0; correcto && indice < inventario->cantidad; indice++) {
        const Producto *producto = &inventario->productos[indice];
        correcto = escribir_dato(archivo, &producto->id, sizeof producto->id) &&
                   escribir_dato(archivo, producto->nombre, sizeof producto->nombre) &&
                   escribir_dato(archivo, producto->categoria, sizeof producto->categoria) &&
                   escribir_dato(archivo, &producto->stock, sizeof producto->stock) &&
                   escribir_dato(archivo, &producto->precio, sizeof producto->precio);
    }

    if (fclose(archivo) != 0) {
        correcto = 0;
    }
    return correcto;
}

int persistencia_cargar(Inventario *inventario, const char *ruta)
{
    FILE *archivo = fopen(ruta, "rb");
    unsigned char firma[sizeof FIRMA_ARCHIVO];
    uint32_t version;
    uint32_t cantidad;
    uint32_t siguiente_id;
    uint32_t indice;
    uint32_t id_mayor = 0;
    Inventario temporal;
    int correcto = 1;

    if (archivo == NULL) {
        return errno == ENOENT ? 0 : -1;
    }

    inventario_inicializar(&temporal);
    correcto = leer_dato(archivo, firma, sizeof firma) &&
               memcmp(firma, FIRMA_ARCHIVO, sizeof firma) == 0 &&
               leer_dato(archivo, &version, sizeof version) &&
               version == VERSION_ARCHIVO &&
               leer_dato(archivo, &cantidad, sizeof cantidad) &&
               cantidad <= MAX_PRODUCTOS_ARCHIVO &&
               leer_dato(archivo, &siguiente_id, sizeof siguiente_id) &&
               siguiente_id != 0;

    for (indice = 0; correcto && indice < cantidad; indice++) {
        Producto producto = {0};
        unsigned int id_agregado;

        correcto = leer_dato(archivo, &producto.id, sizeof producto.id) &&
                   leer_dato(archivo, producto.nombre, sizeof producto.nombre) &&
                   leer_dato(archivo, producto.categoria, sizeof producto.categoria) &&
                   leer_dato(archivo, &producto.stock, sizeof producto.stock) &&
                   leer_dato(archivo, &producto.precio, sizeof producto.precio);

        if (!correcto || producto.id == 0 || producto.stock < 0 ||
            !memchr(producto.nombre, '\0', sizeof producto.nombre) ||
            !memchr(producto.categoria, '\0', sizeof producto.categoria) ||
            !isfinite(producto.precio) || producto.precio < 0.0 ||
            inventario_buscar_por_id(&temporal, producto.id) != NULL ||
            !inventario_agregar(&temporal, &producto, &id_agregado)) {
            correcto = 0;
            break;
        }

        temporal.productos[temporal.cantidad - 1].id = producto.id;
        if (producto.id > id_mayor) {
            id_mayor = producto.id;
        }
    }

    if (correcto && (siguiente_id <= id_mayor || fgetc(archivo) != EOF || ferror(archivo))) {
        correcto = 0;
    }

    if (fclose(archivo) != 0) {
        correcto = 0;
    }

    if (!correcto) {
        inventario_liberar(&temporal);
        return -1;
    }

    temporal.siguiente_id = siguiente_id;
    inventario_liberar(inventario);
    *inventario = temporal;
    return 1;
}
