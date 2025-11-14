#include "../include/ProcesoPar.h"
#include <stdlib.h>

Estado_t destruirProcesoPar(ProcesoPar_t *procesoPar) {
    // Validar parámetro
    if (procesoPar == NULL) {
        return E_PAR_INC;
    }
    
    // Marcar como inactivo
    procesoPar->activo = FALSE;
    
    // Cerrar pipes primero para que el thread de escucha termine
    if (procesoPar->hPipeSalida != NULL) {
        CloseHandle(procesoPar->hPipeSalida);
        procesoPar->hPipeSalida = NULL;
    }
    
    if (procesoPar->hPipeEntrada != NULL) {
        CloseHandle(procesoPar->hPipeEntrada);
        procesoPar->hPipeEntrada = NULL;
    }
    
    // Esperar a que el thread de escucha termine
    if (procesoPar->hThreadEscucha != NULL) {
        WaitForSingleObject(procesoPar->hThreadEscucha, 2000);
        CloseHandle(procesoPar->hThreadEscucha);
        procesoPar->hThreadEscucha = NULL;
    }
    
    // Terminar el proceso hijo si aún está activo
    if (procesoPar->hProcesoHijo != NULL) {
        DWORD exitCode;
        if (GetExitCodeProcess(procesoPar->hProcesoHijo, &exitCode)) {
            if (exitCode == STILL_ACTIVE) {
                TerminateProcess(procesoPar->hProcesoHijo, 0);
                WaitForSingleObject(procesoPar->hProcesoHijo, 2000);
            }
        }
        CloseHandle(procesoPar->hProcesoHijo);
        procesoPar->hProcesoHijo = NULL;
    }
    
    if (procesoPar->hThreadHijo != NULL) {
        CloseHandle(procesoPar->hThreadHijo);
        procesoPar->hThreadHijo = NULL;
    }
    
    // Liberar recursos
    DeleteCriticalSection(&procesoPar->csLock);
    free(procesoPar);
    
    return E_OK;
}
