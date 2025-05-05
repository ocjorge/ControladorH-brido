; controlador_asm.asm - Implementación en ensamblador (x86)
section .text
global asm_escribir_registro
global asm_leer_registro
global asm_transferencia_rapida
global asm_configurar_interrupciones

; void asm_escribir_registro(unsigned int direccion, unsigned int valor)
asm_escribir_registro:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp+8]  ; direccion
    mov eax, [ebp+12] ; valor
    mov [edx], eax    ; escribir valor a la dirección
    
    ; Asegurar que la escritura se complete antes de continuar (memoria ordenada)
    mfence
    
    pop ebp
    ret

; unsigned int asm_leer_registro(unsigned int direccion)
asm_leer_registro:
    push ebp
    mov ebp, esp
    
    mov edx, [ebp+8]  ; direccion
    mov eax, [edx]    ; leer valor desde la dirección
    
    pop ebp
    ret

; void asm_transferencia_rapida(const unsigned char* datos, unsigned int longitud)
asm_transferencia_rapida:
    push ebp
    mov ebp, esp
    push esi
    push edi
    
    mov esi, [ebp+8]  ; puntero a datos
    mov ecx, [ebp+12] ; longitud
    mov edi, 0x1008   ; REG_DATA (dirección fija para optimización)
    
    ; Verificar si podemos usar instrucciones SIMD
    cmp ecx, 16
    jl .transferencia_byte
    
.transferencia_simd:
    ; Usar instrucciones SIMD para transferir 16 bytes a la vez
    cmp ecx, 16
    jl .transferencia_byte
    
    movdqa xmm0, [esi]
    movntdq [edi], xmm0  ; Escritura no temporal (bypass cache)
    
    add esi, 16
    sub ecx, 16
    jmp .transferencia_simd
    
.transferencia_byte:
    ; Transferir bytes restantes uno por uno
    cmp ecx, 0
    je .fin
    
    mov al, [esi]
    mov [edi], al
    
    inc esi
    dec ecx
    jmp .transferencia_byte
    
.fin:
    ; Asegurar que todas las transferencias se completen
    mfence
    
    pop edi
    pop esi
    pop ebp
    ret

; void asm_configurar_interrupciones(void)
asm_configurar_interrupciones:
    push ebp
    mov ebp, esp
    
    ; Guardar estado actual de interrupciones
    pushfd
    
    ; Deshabilitar interrupciones durante la configuración
    cli
    
    ; Configurar vector de interrupción (ejemplo simplificado)
    mov eax, handler_interrupcion
    mov edx, 0x40     ; Vector de interrupción hipotético
    mov [edx*4], eax  ; Almacenar dirección del handler en la tabla de vectores
    
    ; Restaurar estado anterior de interrupciones
    popfd
    
    pop ebp
    ret

; Rutina de manejo de interrupción
handler_interrupcion:
    push eax
    push edx
    
    ; Leer registro de estado para identificar la causa de la interrupción
    mov edx, 0x1004   ; REG_STATUS
    mov eax, [edx]
    
    ; Procesar la interrupción según el estado
    ; (Código simplificado)
    
    ; Enviar EOI (End of Interrupt)
    mov edx, 0x1000   ; REG_CONTROL
    mov eax, [edx]
    or eax, 0x80      ; Bit de EOI
    mov [edx], eax
    
    pop edx
    pop eax
    iret
