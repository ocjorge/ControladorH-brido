#include "controlador.h"
#include <stdio.h>

int main() {
    // Inicializar el controlador
    if (ctrl_inicializar() != CTRL_OK) {
        printf("Error al inicializar el controlador\n");
        return 1;
    }
    
    // Configurar el controlador
    ctrl_configurar(0x02);
    
    // Preparar datos para enviar
    unsigned char datos[] = {0x01, 0x02, 0x03, 0x04};
    
    // Enviar datos al dispositivo
    if (ctrl_enviar_datos(datos, sizeof(datos)) != CTRL_OK) {
        printf("Error al enviar datos\n");
        return 1;
    }
    
    // Habilitar interrupciones
    ctrl_habilitar_interrupciones();
    
    printf("Controlador funcionando correctamente\n");
    return 0;
}
