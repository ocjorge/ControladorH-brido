// controlador.h - Interfaz del controlador
#ifndef CONTROLADOR_H
#define CONTROLADOR_H

// Definiciones de registros del dispositivo
#define REG_CONTROL     0x1000
#define REG_STATUS      0x1004
#define REG_DATA        0x1008

// Códigos de estado
typedef enum {
    CTRL_OK = 0,
    CTRL_ERROR_INIT = -1,
    CTRL_ERROR_COMM = -2,
    CTRL_TIMEOUT = -3
} ctrl_status_t;

// Funciones de la API del controlador
ctrl_status_t ctrl_inicializar(void);
ctrl_status_t ctrl_configurar(unsigned int modo);
ctrl_status_t ctrl_enviar_datos(const unsigned char* datos, unsigned int longitud);
ctrl_status_t ctrl_leer_datos(unsigned char* buffer, unsigned int max_longitud);
void ctrl_habilitar_interrupciones(void);
void ctrl_deshabilitar_interrupciones(void);

#endif // CONTROLADOR_H
