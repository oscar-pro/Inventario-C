# Curso de C: gestor de inventario

Proyecto de consola para practicar C desde los fundamentos hasta temas propios de un nivel tecnico. El programa permite administrar productos, controlar existencias, consultar reportes y guardar la informacion en un archivo.

## Requisitos

- Windows
- GCC (por ejemplo, el que viene con MSYS2 UCRT64)

## Compilar y ejecutar

Abre una terminal en esta carpeta y ejecuta:

```powershell
.\compilar.bat
.\gestor_inventario.exe
```

Tambien puedes compilar manualmente:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -I modulos\comun -I modulos\inventario -I modulos\persistencia -I modulos\reportes main.c modulos\comun\utilidades.c modulos\inventario\inventario.c modulos\persistencia\persistencia.c modulos\reportes\reportes.c -o gestor_inventario.exe
```

Los datos se guardan en `datos\inventario.dat`, relativo a la carpeta desde la que se ejecuta el programa. No borres ese archivo si quieres conservar tu inventario.

## Organizacion por modulos

- `main.c`: punto de entrada y ciclo principal.
- `modulos\comun`: lectura y validacion de datos.
- `modulos\inventario`: estructura de productos y operaciones CRUD.
- `modulos\persistencia`: lectura y escritura de archivos.
- `modulos\reportes`: listados, ordenamiento y resumen de existencias.
- `datos`: carpeta para el archivo de inventario.

Cada modulo tiene un archivo `.h` con su interfaz y un `.c` con su implementacion. Esta separacion ayuda a practicar compilacion por varios archivos, declaraciones, definiciones y responsabilidades.

## Ruta de aprendizaje

Avanza en este orden. En cada etapa, intenta primero resolver los retos sin mirar las soluciones de los modulos posteriores.

1. **Fundamentos:** variables, tipos, `printf`, `scanf` y expresiones. Agrega al menu una opcion que convierta una cantidad de dolares a otra moneda.
2. **Condiciones y ciclos:** `if`, `switch`, `while` y `for`. Agrega filtros de productos con stock bajo.
3. **Funciones y modulos:** prototipos, parametros, valores de retorno, archivos `.h` y `.c`. Agrega una funcion para calcular el valor de un producto.
4. **Arreglos y cadenas:** arreglos de caracteres, limites de buffer y busqueda de texto. Agrega busqueda de productos por nombre.
5. **Estructuras y enumeraciones:** estudia `Producto` e incorpora un campo para el proveedor o el codigo de barras.
6. **Punteros y memoria dinamica:** revisa como crece el inventario con `realloc`, como se accede a productos y donde se libera la memoria.
7. **Archivos:** examina como se guarda y carga el inventario; agrega una exportacion de reportes a texto.
8. **Algoritmos y calidad:** compara busqueda lineal y ordenamiento, prueba entradas invalidas y separa funciones grandes en funciones pequenas.

## Funciones del programa

- Alta, consulta, actualizacion y baja de productos.
- Validacion de cantidades, precios y textos.
- Busqueda por identificador.
- Ordenamiento alfabetico.
- Reporte de cantidad de productos, unidades y valor total.
- Memoria dinamica y persistencia entre ejecuciones.

El archivo de persistencia es binario y esta pensado para practicar localmente: no es un formato de intercambio entre sistemas o compiladores. Haz una copia de `datos\inventario.dat` antes de experimentar con cambios en su formato.

## Ideas para extenderlo

- Implementar busqueda parcial por nombre.
- Agregar categorias y filtros combinados.
- Exportar e importar CSV.
- Agregar pruebas automatizadas para las operaciones del inventario.
- Guardar un historial de movimientos de stock con fecha y motivo.
