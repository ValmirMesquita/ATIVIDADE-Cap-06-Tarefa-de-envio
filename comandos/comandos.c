/***************************************************************************************************
Autor: Valmir Linhares de Sousa de Mesquita
ATIVIDADE-Cap-06-Tarefa-de-envio - Comandos 
Data:    22 de Maio de 2025
Link GitHub: https://github.com/ValmirMesquita/ATIVIDADE-Cap-06-Tarefa-de-envio/tree/main/comandos
Link YouTube: https://youtube.com/shorts/cZdh6nyXh0Q
****************************************************************************************************/


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define os pinos GPIO para cada LED
#define LED_VERDE 11      // Pino GPIO para o LED verde
#define LED_AZUL 12       // Pino GPIO para o LED azul
#define LED_VERMELHO 13   // Pino GPIO para o LED vermelho

// Buffer para armazenar os caracteres recebidos via serial
#define BUFFER_SIZE 15      // Tamanho máximo do buffer de comandos
char buffer[BUFFER_SIZE];   // Array para armazenar os caracteres recebidos
int buffer_index = 0;      // Índice para controlar a posição no buffer

// Função para desligar todos os LEDs
void apaga_todos_leds() {
    gpio_put(LED_VERDE, 0); // Desliga LED verde
    gpio_put(LED_AZUL, 0); // Desliga LED verde
    gpio_put(LED_VERMELHO, 0); // Desliga LED vermelho
}

void processa_comando() {
    printf("Comando recebido: %s\n", buffer);// Exibe o comando recebido
    
    apaga_todos_leds();// Garante que todos os LEDs estão desligados antes de acender um

    // Verifica qual comando foi recebido e aciona o LED correspondente
    if (strcmp(buffer, "verde") == 0) {
        gpio_put(LED_VERDE, 1);
        printf("LED Verde aceso!\n");
    } 
    else if (strcmp(buffer, "azul") == 0) {
        gpio_put(LED_AZUL, 1);
        printf("LED Azul aceso!\n");
    }
    else if (strcmp(buffer, "vermelho") == 0) {
        gpio_put(LED_VERMELHO, 1);
        printf("LED Vermelho aceso!\n");
    }
    else {
        printf("Comando desconhecido. Use: verde, azul ou vermelho\n");
    }
    
    buffer_index = 0; // Reseta o buffer após processamento
}

int main() {
    stdio_init_all();// Inicializa a comunicação serial USB
    
    // Configuração dos pinos dos LEDs como saída
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    
    apaga_todos_leds();// Garante que todos os LEDs começam desligados
    
    // Mensagem inicial para o usuário
    printf("Digite 'verde', 'azul' ou 'vermelho' para acender os LEDs:\n");

    while (true) {
        int ch = getchar_timeout_us(0); // Lê um caractere da serial sem bloquear a execução

        if (ch != PICO_ERROR_TIMEOUT) {  // Se recebeu um caractere válido
            char c = (char)ch;
            
            // Ignora caracteres de nova linha
            if (c == '\n' || c == '\r') {
                if (buffer_index > 0) {
                    buffer[buffer_index] = '\0'; // finaliza a string
                    processa_comando();
                }
                continue;
            }
            
            // Armazena o caractere no buffer
            if (buffer_index < BUFFER_SIZE - 1) {
                buffer[buffer_index++] = c;
                buffer[buffer_index] = '\0'; // mantém a string terminada
            }
            
            // Verifica se já temos um comando completo
            if (strcmp(buffer, "verde") == 0 || strcmp(buffer, "VERDE") == 0 ||
                strcmp(buffer, "azul") == 0 ||  strcmp(buffer, "AZUL") == 0 ||
                strcmp(buffer, "vermelho") == 0 ||  strcmp(buffer, "VERMELHO") == 0) {
                processa_comando();
            }
        }
        sleep_ms(10);  // pequeno delay para evitar polling intenso
    }
}