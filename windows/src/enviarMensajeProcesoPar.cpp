#include "../include/ProcesoPar.h"

Estado_t enviarMensajeProcesoPar(ProcesoPar_t *procesoPar,
                                 const char *mensaje,
                                 int longitud) {
    // Validar parámetros
    if (procesoPar == NULL || mensaje == NULL || longitud <= 0) {
        return E_PAR_INC;
    }
    
    // Verificar que el proceso esté activo
    if (!procesoPar->activo || procesoPar->hPipeSalida == NULL) {
        return E_PROCESO_NO_ACTIVO;
    }
    
    // Enviar mensaje al proceso hijo
    DWORD bytesEscritos;
    if (!WriteFile(procesoPar->hPipeSalida, mensaje, longitud, &bytesEscritos, NULL)) {
        return E_ERROR_ESCRITURA;
    }
    
    // Verificar que se escribieron todos los bytes
    if ((int)bytesEscritos != longitud) {
        return E_ERROR_ENVIAR;
    }
    
    // Hacer flush del buffer
    FlushFileBuffers(procesoPar->hPipeSalida);
    
    return E_OK;
}
