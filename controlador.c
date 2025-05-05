// controlador.c - Implementación en C
#include "controlador.h"
#include <stdio.h>

// Declaraciones de funciones implementadas en ASM
extern void asm_escribir_registro(unsigned int direccion, unsigned int valor);
extern unsigned int asm_leer_registro(unsigned int direccion);
extern void asm_transferencia_rapida(const unsigned char* datos, unsigned int longitud);
extern void asm_configurar_interrupciones(void);

// Implementación de la API en C
ctrl_status_t ctrl_inicializar(void) {
    printf("Inicializando controlador...\n");
    
    // Resetear el dispositivo escribiendo al registro de control
    asm_escribir_registro(REG_CONTROL, 0x01);
    
    // Verificar si el dispositivo está listo
    unsigned int status = asm_leer_registro(REG_STATUS);
    if ((status & 0x80) == 0) {
        return CTRL_ERROR_INIT;
    }
    
    return CTRL_OK;
}

ctrl_status_t ctrl_configurar(unsigned int modo) {
    printf("Configurando controlador en modo: %u\n", modo);
    
    // Escribir el modo al registro de control
    asm_escribir_registro(REG_CONTROL, 0x10 | (modo & 0x0F));
    
    return CTRL_OK;
}

ctrl_status_t ctrl_enviar_datos(const unsigned char* datos, unsigned int longitud) {
    // Verificar si el dispositivo está listo para recibir datos
    if ((asm_leer_registro(REG_STATUS) & 0x40) == 0) {
        return CTRL_ERROR_COMM;
    }
    
    // Usar la función ASM optimizada para transferir datos rápidamente
    asm_transferencia_rapida(datos, longitud);
    
    return CTRL_OK;
}

ctrl_status_t ctrl_leer_datos(unsigned char* buffer, unsigned int max_longitud) {
    // Verificar si hay datos disponibles para leer
    unsigned int status = asm_leer_registro(REG_STATUS);
    if ((status & 0x20) == 0) {
        return CTRL_ERROR_COMM;
    }
    
    // Leer la cantidad de datos disponibles
    unsigned int disponible = asm_leer_registro(REG_DATA);
    unsigned int a_leer = (disponible < max_longitud) ? disponible : max_longitud;
    
    // Leer los datos byte por byte (simplificado)
    for (unsigned int i = 0; i < a_leer; i++) {
        buffer[i] = (unsigned char)asm_leer_registro(REG_DATA);
    }
    
    return CTRL_OK;
}

void ctrl_habilitar_interrupciones(void) {
    // Configurar las interrupciones usando código ASM optimizado
    asm_configurar_interrupciones();
    
    // Habilitar las interrupciones en el registro de control
    unsigned int ctrl = asm_leer_registro(REG_CONTROL);
    asm_escribir_registro(REG_CONTROL, ctrl | 0x20);
}

void ctrl_deshabilitar_interrupciones(void) {
    // Deshabilitar las interrupciones en el registro de control
    unsigned int ctrl = asm_leer_registro(REG_CONTROL);
    asm_escribir_registro(REG_CONTROL, ctrl & ~0x20);
}
