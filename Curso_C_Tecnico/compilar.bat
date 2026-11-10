@echo off
setlocal

gcc -std=c11 -Wall -Wextra -Wpedantic ^
  -I modulos\comun -I modulos\inventario -I modulos\persistencia -I modulos\reportes ^
  main.c ^
  modulos\comun\utilidades.c ^
  modulos\inventario\inventario.c ^
  modulos\persistencia\persistencia.c ^
  modulos\reportes\reportes.c ^
  -o gestor_inventario.exe

if errorlevel 1 (
    echo.
    echo Erda y tales algo salio mal mani.
    exit /b 1
)

echo.
echo Compilacion correcta: gestor_inventario.exe
