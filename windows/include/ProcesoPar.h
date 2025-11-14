#ifndef PROCESOPAR_H
#define PROCESOPAR_H

#include <windows.h>

// Definición del tipo Estado_t
typedef unsigned int Estado_t;

// Definición de códigos de estado
#define E_OK                    0   /* La función se comportó bien */
#define E_PAR_INC              1   /* Parámetro incorrecto */
#define E_NO_MEMORIA           2   /* Error de memoria */
#define E_ERROR_CREAR_PIPE     3   /* Error al crear pipe */
#define E_ERROR_CREAR_PROCESO  4   /* Error al crear proceso */
#define E_ERROR_ENVIAR         5   /* Error al enviar mensaje */
#define E_ERROR_THREAD         6   /* Error al crear thread de escucha */
#define E_PROCESO_NO_ACTIVO    7   /* El proceso no está activo */
#define E_ERROR_ESCRITURA      8   /* Error en escritura a pipe */
#define E_ERROR_LECTURA        9   /* Error en lectura de pipe */

// Estructura del proceso par
typedef struct ProcesoPar {
    HANDLE hProcesoHijo;           // Handle del proceso hijo
    HANDLE hThreadHijo;            // Handle del thread del proceso hijo
    HANDLE hPipeEntrada;           // Pipe para recibir datos del hijo
    HANDLE hPipeEntradaWrite;      // Extremo de escritura del pipe de entrada
    HANDLE hPipeSalida;            // Pipe para enviar datos al hijo
    HANDLE hPipeSalidaRead;        // Extremo de lectura del pipe de salida
    HANDLE hThreadEscucha;         // Thread que escucha mensajes del hijo
    Estado_t (*funcionEscucha)(const char*, int); // Función callback
    BOOL activo;                   // Indica si el proceso está activo
    CRITICAL_SECTION csLock;       // Para sincronización
} ProcesoPar_t;

// Prototipos de funciones
Estado_t lanzarProcesoPar(const char *nombreArchivoEjecutable,
                          const char **listaLineaComando,
                          ProcesoPar_t **procesoPar);

Estado_t destruirProcesoPar(ProcesoPar_t *procesoPar);

Estado_t enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
                                 const char *mensaje,
                                 int longitud);

Estado_t establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
                                    Estado_t (*f)(const char*, int));

#endif // PROCESOPAR_H
