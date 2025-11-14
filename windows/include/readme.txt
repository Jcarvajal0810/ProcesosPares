================================================================================
  PRÁCTICA: PROCESOS PARES
  Sistema Operativo: Windows 11
  Entorno: MSYS2
  Autor: Jcarvajal0810
  Fecha: Noviembre 2025
================================================================================

DESCRIPCIÓN:
------------
Esta biblioteca implementa el mecanismo de "Proceso Par" que permite crear,
controlar y comunicarse bidireccionalmente con procesos externos usando
pipes anónimos y la API de Windows.

ESTRUCTURA DEL PROYECTO:
------------------------
windows/
├── src/              (Archivos fuente y Makefile)
│   ├── lanzarProcesoPar.cpp
│   ├── destruirProcesoPar.cpp
│   ├── enviarMensajeProcesoPar.cpp
│   ├── establecerFuncionDeEscucha.cpp
│   ├── proceso_hijo.cpp
│   ├── test_procesoPar.cpp
│   └── Makefile
├── lib/              (Biblioteca estática generada)
│   └── libProcesoPar_Jcarvajal0810.a
├── bin/              (Ejecutables generados)
│   ├── test_procesoPar.exe
│   └── proceso_hijo.exe
└── include/          (Archivos de cabecera)
    ├── ProcesoPar.h
    └── readme.txt (este archivo)

REQUISITOS:
-----------
1. Windows 11
2. MSYS2 instalado (https://www.msys2.org/)
3. Paquetes necesarios en MSYS2:
   - mingw-w64-x86_64-gcc
   - mingw-w64-x86_64-g++
   - make

Para instalar los paquetes en MSYS2:
  pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-g++ make

COMPILACIÓN:
------------
1. Abrir MSYS2 MinGW 64-bit

2. Navegar al directorio src:
   cd /c/Users/57304/Downloads/windows/src

3. Compilar todo:
   make

   Esto generará:
   - Biblioteca estática: ../lib/libProcesoPar_Jcarvajal0810.a
   - Programa de test: ../bin/test_procesoPar.exe
   - Proceso hijo: ../bin/proceso_hijo.exe

4. Para ejecutar los tests:
   make test

5. Para limpiar archivos generados:
   make clean

6. Para ver información del proyecto:
   make info

RUTA ÚNICA (ANTI-PLAGIO):
--------------------------
La biblioteca usa la siguiente ruta única para ejecutar procesos hijos:
  C:/Users/57304/Downloads/windows/bin/

Esta ruta está codificada en lanzarProcesoPar.cpp y garantiza que solo
se ejecuten binarios compilados por este proyecto específico.

IMPORTANTE: Los ejecutables generados (proceso_hijo.exe) deben estar en
            ../bin/ para que puedan ser encontrados por la biblioteca.

USO DE LA API:
--------------

1. Incluir el header:
   #include "ProcesoPar.h"

2. Lanzar un proceso par:
   ProcesoPar_t *procesoPar = NULL;
   const char *argv[] = {NULL};
   Estado_t estado = lanzarProcesoPar("proceso_hijo.exe", argv, &procesoPar);

3. Establecer función de escucha:
   Estado_t callback(const char *mensaje, int longitud) {
       printf("Recibido: %s\n", mensaje);
       return E_OK;
   }
   establecerFuncionDeEscucha(procesoPar, callback);

4. Enviar mensaje al proceso hijo:
   const char *msg = "Hola proceso hijo\n";
   enviarMensajeProcesoPar(procesoPar, msg, strlen(msg));

5. Destruir el proceso par:
   destruirProcesoPar(procesoPar);

CÓDIGOS DE ERROR:
-----------------
E_OK                   (0) - Operación exitosa
E_PAR_INC             (1) - Parámetro incorrecto
E_NO_MEMORIA          (2) - Error de memoria
E_ERROR_CREAR_PIPE    (3) - Error al crear pipe
E_ERROR_CREAR_PROCESO (4) - Error al crear proceso
E_ERROR_ENVIAR        (5) - Error al enviar mensaje
E_ERROR_THREAD        (6) - Error al crear thread
E_PROCESO_NO_ACTIVO   (7) - Proceso no activo
E_ERROR_ESCRITURA     (8) - Error en escritura
E_ERROR_LECTURA       (9) - Error en lectura

CARACTERÍSTICAS TÉCNICAS:
-------------------------
- Usa CreateProcess() para crear procesos hijos
- Usa CreatePipe() para comunicación bidireccional
- Usa CreateThread() para escuchar mensajes asíncronamente
- Usa CRITICAL_SECTION para sincronización thread-safe
- Biblioteca estática (.a) para enlace estático
- Soporta mensajes de cualquier tamaño y formato
- Comunicación full-duplex entre procesos

PRUEBAS:
--------
El programa test_procesoPar.cpp realiza las siguientes pruebas:
1. Lanzar un proceso par
2. Establecer función de escucha
3. Enviar múltiples mensajes
4. Recibir respuestas del proceso hijo
5. Destruir el proceso par correctamente

Para ejecutar:
  cd ../bin
  ./test_procesoPar.exe

SOLUCIÓN DE PROBLEMAS:
----------------------
1. Error "No se pudo lanzar el proceso par":
   - Verificar que proceso_hijo.exe esté en ../bin/
   - Verificar permisos de ejecución

2. Error de compilación:
   - Verificar que MSYS2 esté correctamente instalado
   - Verificar que los paquetes gcc, g++ y make estén instalados

3. Error "The program can't start because libgcc_s_seh-1.dll is missing":
   - Usar MSYS2 MinGW 64-bit (no MSYS2 MSYS)
   - O copiar las DLLs necesarias al directorio bin/

CONTACTO:
---------
Usuario GitHub: Jcarvajal0810
Repositorio: https://github.com/Jcarvajal0810

NOTAS ADICIONALES:
------------------
- Esta implementación es específica para Windows
- Usa la API Win32 nativa (no POSIX)
- La biblioteca es thread-safe
- Soporta múltiples procesos pares simultáneos
- El proceso hijo puede ser cualquier ejecutable que use stdin/stdout

================================================================================
FIN DEL README
================================================================================
