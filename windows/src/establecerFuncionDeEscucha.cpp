#include "../include/ProcesoPar.h"

Estado_t establecerFuncionDeEscucha(ProcesoPar_t *procesoPar,
                                    Estado_t (*f)(const char*, int)) {
    // Validar parámetros
    if (procesoPar == NULL || f == NULL) {
        return E_PAR_INC;
    }
    
    // Establecer la función de escucha de forma thread-safe
    EnterCriticalSection(&procesoPar->csLock);
    procesoPar->funcionEscucha = f;
    LeaveCriticalSection(&procesoPar->csLock);
    
    return E_OK;
}
