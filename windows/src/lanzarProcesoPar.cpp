#include "../include/ProcesoPar.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Thread que escucha mensajes del proceso hijo
DWORD WINAPI threadEscucha(LPVOID lpParam) {
    ProcesoPar_t *procesoPar = (ProcesoPar_t*)lpParam;
    char buffer[4096];
    DWORD bytesLeidos;
    
    while (procesoPar->activo) {
        if (ReadFile(procesoPar->hPipeEntrada, buffer, sizeof(buffer) - 1, &bytesLeidos, NULL)) {
            if (bytesLeidos > 0) {
                buffer[bytesLeidos] = '\0';
                
                // Llamar a la función de escucha si está definida
                EnterCriticalSection(&procesoPar->csLock);
                if (procesoPar->funcionEscucha != NULL) {
                    procesoPar->funcionEscucha(buffer, bytesLeidos);
                }
                LeaveCriticalSection(&procesoPar->csLock);
            }
        } else {
            // Si falla la lectura, probablemente el proceso hijo terminó
            DWORD error = GetLastError();
            if (error == ERROR_BROKEN_PIPE) {
                break;
            }
        }
    }
    
    return 0;
}

Estado_t lanzarProcesoPar(const char *nombreArchivoEjecutable,
                          const char **listaLineaComando,
                          ProcesoPar_t **procesoPar) {
    // Validar parámetros
    if (nombreArchivoEjecutable == NULL || procesoPar == NULL) {
        return E_PAR_INC;
    }
    
    // Asignar memoria para la estructura ProcesoPar_t
    *procesoPar = (ProcesoPar_t*)malloc(sizeof(ProcesoPar_t));
    if (*procesoPar == NULL) {
        return E_NO_MEMORIA;
    }
    
    // Inicializar estructura
    memset(*procesoPar, 0, sizeof(ProcesoPar_t));
    InitializeCriticalSection(&(*procesoPar)->csLock);
    (*procesoPar)->activo = TRUE;
    (*procesoPar)->funcionEscucha = NULL;
    
    // Atributos de seguridad para los pipes
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    // Crear pipe de entrada (para recibir datos del hijo)
    if (!CreatePipe(&(*procesoPar)->hPipeEntrada, 
                    &(*procesoPar)->hPipeEntradaWrite, 
                    &sa, 0)) {
        free(*procesoPar);
        return E_ERROR_CREAR_PIPE;
    }
    
    // Crear pipe de salida (para enviar datos al hijo)
    if (!CreatePipe(&(*procesoPar)->hPipeSalidaRead, 
                    &(*procesoPar)->hPipeSalida, 
                    &sa, 0)) {
        CloseHandle((*procesoPar)->hPipeEntrada);
        CloseHandle((*procesoPar)->hPipeEntradaWrite);
        free(*procesoPar);
        return E_ERROR_CREAR_PIPE;
    }
    
    // Asegurar que los handles que no debe heredar el hijo no sean heredables
    SetHandleInformation((*procesoPar)->hPipeEntrada, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation((*procesoPar)->hPipeSalida, HANDLE_FLAG_INHERIT, 0);
    
    // Construir línea de comando
    char cmdLine[4096] = {0};
    
    // Obtener la ruta del ejecutable actual (proceso padre)
    char rutaActual[MAX_PATH];
    GetModuleFileNameA(NULL, rutaActual, MAX_PATH);
    
    // Extraer el directorio del ejecutable actual
    char *ultimaBarra = strrchr(rutaActual, '\\');
    if (ultimaBarra != NULL) {
        *ultimaBarra = '\0';
    }
    
    // Construir ruta única para el proceso hijo
    // Formato: [DirectorioActual]\[nombreArchivo]_Jcarvajal0810.exe
    char rutaUnicaEjecutable[MAX_PATH];
    
    // Verificar si el nombre ya tiene extensión .exe
    const char *extension = strrchr(nombreArchivoEjecutable, '.');
    if (extension != NULL && strcmp(extension, ".exe") == 0) {
        // Ya tiene .exe, solo agregar el identificador único
        char nombreSinExt[MAX_PATH];
        strncpy(nombreSinExt, nombreArchivoEjecutable, extension - nombreArchivoEjecutable);
        nombreSinExt[extension - nombreArchivoEjecutable] = '\0';
        snprintf(rutaUnicaEjecutable, sizeof(rutaUnicaEjecutable), 
                 "%s\\%s_Jcarvajal0810.exe", rutaActual, nombreSinExt);
    } else {
        // No tiene .exe, agregar identificador y extensión
        snprintf(rutaUnicaEjecutable, sizeof(rutaUnicaEjecutable), 
                 "%s\\%s_Jcarvajal0810.exe", rutaActual, nombreArchivoEjecutable);
    }
    
    // Si no existe con el identificador único, intentar con el nombre original
    DWORD attrs = GetFileAttributesA(rutaUnicaEjecutable);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        // No existe con identificador único, intentar ruta original en mismo directorio
        snprintf(rutaUnicaEjecutable, sizeof(rutaUnicaEjecutable), 
                 "%s\\%s", rutaActual, nombreArchivoEjecutable);
    }
    
    // Construir línea de comando completa
    strncpy(cmdLine, rutaUnicaEjecutable, sizeof(cmdLine) - 1);
    
    if (listaLineaComando != NULL) {
        for (int i = 0; listaLineaComando[i] != NULL; i++) {
            strncat(cmdLine, " ", sizeof(cmdLine) - strlen(cmdLine) - 1);
            strncat(cmdLine, listaLineaComando[i], sizeof(cmdLine) - strlen(cmdLine) - 1);
        }
    }
    
    // Configurar STARTUPINFO
    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = (*procesoPar)->hPipeSalidaRead;
    si.hStdOutput = (*procesoPar)->hPipeEntradaWrite;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags |= STARTF_USESTDHANDLES;
    
    // Información del proceso
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    
    // Crear proceso hijo con ruta única
    if (!CreateProcessA(rutaUnicaEjecutable,  // Ruta única del ejecutable
                       cmdLine,               // Línea de comando
                       NULL,                  // Atributos de seguridad del proceso
                       NULL,                  // Atributos de seguridad del thread
                       TRUE,                  // Heredar handles
                       0,                     // Flags de creación
                       NULL,                  // Entorno
                       NULL,                  // Directorio actual
                       &si,                   // STARTUPINFO
                       &pi)) {                // PROCESS_INFORMATION
        CloseHandle((*procesoPar)->hPipeEntrada);
        CloseHandle((*procesoPar)->hPipeEntradaWrite);
        CloseHandle((*procesoPar)->hPipeSalida);
        CloseHandle((*procesoPar)->hPipeSalidaRead);
        DeleteCriticalSection(&(*procesoPar)->csLock);
        free(*procesoPar);
        return E_ERROR_CREAR_PROCESO;
    }
    
    // Guardar handles del proceso
    (*procesoPar)->hProcesoHijo = pi.hProcess;
    (*procesoPar)->hThreadHijo = pi.hThread;
    
    // Cerrar handles que ya no necesitamos en el proceso padre
    CloseHandle((*procesoPar)->hPipeEntradaWrite);
    CloseHandle((*procesoPar)->hPipeSalidaRead);
    (*procesoPar)->hPipeEntradaWrite = NULL;
    (*procesoPar)->hPipeSalidaRead = NULL;
    
    // Crear thread de escucha
    (*procesoPar)->hThreadEscucha = CreateThread(NULL, 0, threadEscucha, *procesoPar, 0, NULL);
    if ((*procesoPar)->hThreadEscucha == NULL) {
        TerminateProcess((*procesoPar)->hProcesoHijo, 1);
        CloseHandle((*procesoPar)->hProcesoHijo);
        CloseHandle((*procesoPar)->hThreadHijo);
        CloseHandle((*procesoPar)->hPipeEntrada);
        CloseHandle((*procesoPar)->hPipeSalida);
        DeleteCriticalSection(&(*procesoPar)->csLock);
        free(*procesoPar);
        return E_ERROR_THREAD;
    }
    
    return E_OK;
}
