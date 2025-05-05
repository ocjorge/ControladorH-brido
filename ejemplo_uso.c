// ejemplo_uso.c - Ejemplo de uso del controlador
#include "controlador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    printf("Ejemplo de uso del Controlador Híbrido C/ASM\n");
    printf("-------------------------------------------\n\n");
    
    // Inicializar el controlador
    ctrl_status_t status = ctrl_inicializar();
    if (status != CTRL_OK) {
        printf("Error al inicializar el controlador: %d\n", status);
        return EXIT_FAILURE;
    }
    
    printf("Controlador inicializado correctamente.\n");
    
    // Configurar el controlador en modo 2 (ejemplo)
    status = ctrl_configurar(2);
    if (status != CTRL_OK) {
        printf("Error al configurar el controlador: %d\n", status);
        return EXIT_FAILURE;
    }
    
    // Preparar datos para enviar
    unsigned char datos_tx[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    printf("Enviando datos: ");
    for (unsigned int i = 0; i < sizeof(datos_tx); i++) {
        printf("%02X ", datos_tx[i]);
    }
    printf("\n");
    
    // Enviar datos
    status = ctrl_enviar_datos(datos_tx, sizeof(datos_tx));
    if (status != CTRL_OK) {
        printf("Error al enviar datos: %d\n", status);
        return EXIT_FAILURE;
    }
    
    printf("Datos enviados correctamente.\n");
    
    // Leer datos de respuesta
    unsigned char datos_rx[16];
    memset(datos_rx, 0, sizeof(datos_rx));
    
    status = ctrl_leer_datos(datos_rx, sizeof(datos_rx));
    if (status != CTRL_OK) {
        printf("Error al leer datos: %d\n", status);
        return EXIT_FAILURE;
    }
    
    printf("Datos recibidos: ");
    for (unsigned int i = 0; i < sizeof(datos_rx); i++) {
        printf("%02X ", datos_rx[i]);
    }
    printf("\n");
    
    // Habilitar interrupciones
    printf("Habilitando interrupciones...\n");
    ctrl_habilitar_interrupciones();
    
    // En una aplicación real, aquí habría un bucle de eventos o similar
    printf("Controlador funcionando. Presione Ctrl+C para salir.\n");
    
    // Simulación: esperar a que el usuario termine el programa
    getchar();
    
    // Deshabilitar interrupciones antes de salir
    ctrl_deshabilitar_interrupciones();
    
    printf("Controlador detenido.\n");
    return EXIT_SUCCESS;
}
