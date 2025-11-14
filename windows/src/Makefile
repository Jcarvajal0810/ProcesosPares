# Makefile para Proceso Par - Windows MSYS2
# Autor: Jcarvajal0810
# Makefile genérico para cualquier prueba

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -I../include
AR = ar
ARFLAGS = rcs

# Directorios
LIB_DIR = ../lib
INCLUDE_DIR = ../include
BIN_DIR = ../bin

# Nombre de la biblioteca (con identificador único para evitar plagio)
LIB_NAME = libProcesoPar_Jcarvajal0810.a
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)

# Archivos fuente de la biblioteca
LIB_SOURCES = lanzarProcesoPar.cpp \
              destruirProcesoPar.cpp \
              enviarMensajeProcesoPar.cpp \
              establecerFuncionDeEscucha.cpp

# Archivos objeto de la biblioteca
LIB_OBJECTS = $(LIB_DIR)/lanzarProcesoPar.o \
              $(LIB_DIR)/destruirProcesoPar.o \
              $(LIB_DIR)/enviarMensajeProcesoPar.o \
              $(LIB_DIR)/establecerFuncionDeEscucha.o

# Archivos de prueba y procesos hijos (detecta automáticamente)
# Todos los archivos que empiezan con "test_" serán compilados como tests
# Todos los archivos que empiezan con "proceso_" serán compilados como procesos hijos
TEST_SOURCES = $(wildcard test_*.cpp)
PROCESO_SOURCES = $(wildcard proceso_*.cpp)

# Generar nombres de ejecutables
TEST_EXES = $(patsubst %.cpp,$(BIN_DIR)/%_Jcarvajal0810.exe,$(TEST_SOURCES))
PROCESO_EXES = $(patsubst %.cpp,$(BIN_DIR)/%_Jcarvajal0810.exe,$(PROCESO_SOURCES))

# Regla principal
all: dirs $(LIB_PATH) $(PROCESO_EXES) $(TEST_EXES)
	@echo "=========================================="
	@echo "  Compilación completa - Jcarvajal0810"
	@echo "=========================================="
	@echo "Biblioteca: $(LIB_PATH)"
	@echo ""
	@echo "Procesos hijos compilados:"
	@for exe in $(PROCESO_EXES); do echo "  - $$exe"; done
	@echo ""
	@echo "Tests compilados:"
	@for exe in $(TEST_EXES); do echo "  - $$exe"; done
	@echo "=========================================="

# Crear directorios necesarios
dirs:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(BIN_DIR)

# Compilar biblioteca estática
$(LIB_PATH): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Biblioteca estática creada: $@"

# Reglas para compilar objetos de la biblioteca
$(LIB_DIR)/lanzarProcesoPar.o: lanzarProcesoPar.cpp $(INCLUDE_DIR)/ProcesoPar.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/destruirProcesoPar.o: destruirProcesoPar.cpp $(INCLUDE_DIR)/ProcesoPar.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/enviarMensajeProcesoPar.o: enviarMensajeProcesoPar.cpp $(INCLUDE_DIR)/ProcesoPar.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_DIR)/establecerFuncionDeEscucha.o: establecerFuncionDeEscucha.cpp $(INCLUDE_DIR)/ProcesoPar.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla genérica para compilar cualquier test (test_*.cpp)
$(BIN_DIR)/test_%_Jcarvajal0810.exe: test_%.cpp $(LIB_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@ -L$(LIB_DIR) -lProcesoPar_Jcarvajal0810
	@echo "Test compilado: $@"

# Regla genérica para compilar cualquier proceso hijo (proceso_*.cpp)
$(BIN_DIR)/proceso_%_Jcarvajal0810.exe: proceso_%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "Proceso hijo compilado: $@"

# Ejecutar todos los tests automáticamente
test: all
	@echo "=========================================="
	@echo "  Ejecutando todos los tests..."
	@echo "=========================================="
	@for test in $(TEST_EXES); do \
		echo ""; \
		echo "Ejecutando: $$test"; \
		echo "----------------------------------------"; \
		cd $(BIN_DIR) && ./`basename $$test` || exit 1; \
		echo "----------------------------------------"; \
	done
	@echo ""
	@echo "=========================================="
	@echo "  Todos los tests completados"
	@echo "=========================================="

# Ejecutar un test específico
# Uso: make run-test NAME=nombre_test (sin test_ ni .cpp)
run-test:
ifndef NAME
	@echo "ERROR: Debe especificar NAME=nombre_test"
	@echo "Ejemplo: make run-test NAME=procesoPar"
else
	@if [ -f "$(BIN_DIR)/test_$(NAME)_Jcarvajal0810.exe" ]; then \
		echo "Ejecutando test_$(NAME)..."; \
		cd $(BIN_DIR) && ./test_$(NAME)_Jcarvajal0810.exe; \
	else \
		echo "ERROR: No existe test_$(NAME)_Jcarvajal0810.exe"; \
		echo "Tests disponibles:"; \
		ls -1 $(BIN_DIR)/test_*_Jcarvajal0810.exe 2>/dev/null | xargs -n1 basename || echo "  (ninguno)"; \
	fi
endif

# Limpiar archivos generados
clean:
	rm -rf $(LIB_DIR)/*.o $(LIB_DIR)/*.a
	rm -rf $(BIN_DIR)/*.exe
	@echo "Archivos limpiados"

# Limpiar todo incluyendo directorios
distclean: clean
	rm -rf $(LIB_DIR) $(BIN_DIR)
	@echo "Limpieza completa"

# Mostrar información del proyecto
info:
	@echo "=========================================="
	@echo "  INFORMACIÓN DEL PROYECTO"
	@echo "=========================================="
	@echo "Autor: Jcarvajal0810"
	@echo "Biblioteca: $(LIB_NAME)"
	@echo "Ruta biblioteca: $(LIB_PATH)"
	@echo "Compilador: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"
	@echo ""
	@echo "Tests encontrados:"
	@for src in $(TEST_SOURCES); do echo "  - $$src"; done
	@echo ""
	@echo "Procesos hijos encontrados:"
	@for src in $(PROCESO_SOURCES); do echo "  - $$src"; done
	@echo "=========================================="

# Listar todos los tests disponibles
list-tests:
	@echo "Tests disponibles:"
	@ls -1 $(BIN_DIR)/test_*_Jcarvajal0810.exe 2>/dev/null | xargs -n1 basename || echo "  (ninguno compilado aún)"

# Crear un nuevo test a partir de plantilla
# Uso: make new-test NAME=mi_nuevo_test
new-test:
ifndef NAME
	@echo "ERROR: Debe especificar NAME=nombre_test"
	@echo "Ejemplo: make new-test NAME=mi_prueba"
else
	@echo "Creando nuevo test: test_$(NAME).cpp"
	@echo '#include <stdio.h>' > test_$(NAME).cpp
	@echo '#include "../include/ProcesoPar.h"' >> test_$(NAME).cpp
	@echo '' >> test_$(NAME).cpp
	@echo 'int main() {' >> test_$(NAME).cpp
	@echo '    printf("=== TEST $(NAME) ===\\n");' >> test_$(NAME).cpp
	@echo '    // Agregar código de prueba aquí' >> test_$(NAME).cpp
	@echo '    return 0;' >> test_$(NAME).cpp
	@echo '}' >> test_$(NAME).cpp
	@echo "Archivo test_$(NAME).cpp creado exitosamente"
	@echo "Ejecute 'make' para compilar"
endif

.PHONY: all dirs test run-test clean distclean info list-tests new-test
