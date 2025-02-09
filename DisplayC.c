#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h" // Biblioteca básica para o Raspberry Pi Pico
#include <string.h>
#include "inc/font.h" // Arquivo de fontes customizadas
#include "hardware/gpio.h" // Para manipulação de pinos GPIO
#include "hardware/i2c.h" // Para comunicação I2C
#include "hardware/clocks.h" // Para controle de clock
#include "led.pio.h" // Programa PIO para controle de LEDs
#include "inc/ssd1306.h" // Para controle do display OLED

// Definindo o endereço do display I2C
#define endereco 0x3C

// Definindo o número de LEDs na matriz (5x5 = 25 LEDs)
#define NUM_PIXELS 25

// Definição dos pinos GPIO para LEDs RGB e botões
#define LED_PIN_RED 13
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define BUTTON_A 5
#define BUTTON_B 6

// Definições dos pinos I2C
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_PORT i2c1

// Variáveis globais
PIO pio; // Estrutura para a PIO (Programmable IO) do Raspberry Pi Pico
uint sm; // Máquina de estado para a PIO
ssd1306_t ssd; // Estrutura do display OLED
double r = 0.0, g = 0, b = 0.3;  // Cor inicial dos LEDs RGB (no formato RGB)


// Definição dos padrões dos números (0-9) como matrizes de LEDs
double numeros[11][25] = {
                            // Padrões de números de 0 a 9
                            // Cada linha de um número é uma linha de LEDs que será iluminada
                            // O número 0.0 é usado para representar LEDs apagados e 1.0 é usado para representar LEDs Ligados
                        { // Número 0
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },
                        { // Número 1
                            0.0, 0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 1.0, 0.0,
                            0.0, 0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0, 0.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },
                        { // Número 2
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            0.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },
                        { // Número 3
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },
                        { // Número 4
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 1.0
                        },
                        { // Número 5
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            0.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },
                        { // Número 6
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            0.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },

                        { // Número 7
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0, 0.0, 0.0
                        },
                        { // Número 8
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0
                        },
                        { // Número 9
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 1.0
                        },
                        { // LEDs apagados
                            0.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.0, 0.0
                        }
};

// Sequência dos LEDs em ordem para o controle
int sequencia[25] = {
    0, 1, 2, 3, 4,
    9, 8, 7, 6, 5,
    10, 11, 12, 13, 14,
    19, 18, 17, 16, 15,
    20, 21, 22, 23, 24
};

// Função para converter os valores RGB para o formato de cor para WS2812
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    // Converte os valores de cor RGB para 0-255
    R = r * 255;
    G = g * 255;
    B = b * 255;
    // Retorna o valor combinado das cores no formato G-R-B para o WS2812
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para atualizar o display OLED com o texto fornecido
void atualizar_display(ssd1306_t *ssd, const char *linha1, const char *linha2) {
    ssd1306_fill(ssd, false);
    ssd1306_draw_string(ssd, linha1, 30, 20);  // Primeira linha
    ssd1306_draw_string(ssd, linha2, 30, 40);  // Segunda linha
    ssd1306_send_data(ssd);
}

// Função para desenhar um número específico na matriz de LEDs
void desenho (int n) {
    uint32_t valor_led;  // Variável para armazenar o valor de cor para o LED
    double *desenho = numeros[n];  // Padrão para o número a ser exibido

    // Itera sobre todos os LEDs da matriz e aplica o padrão
    for (int16_t i = 0; i < NUM_PIXELS; i++) {        
        valor_led = matrix_rgb(r * desenho[24 - i], g * desenho[24 - i], b * desenho[24 - i]); // Converte o valor da matriz para o valor RGB do LED
                pio_sm_put_blocking(pio, sm, valor_led); // Envia o valor para a máquina de estado da PIO
    }
}

// Variável para armazenar o tempo do último evento (debounce)
static volatile uint32_t ultima = 0; 

// Função de interrupção que é chamada quando um botão é pressionado
static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual

    // Inverte a cor da tela para efeito visual
    bool cor = true;
    cor = !cor; 

    bool estado_led_verde = false;  // Inicializa o estado do LED verde como apagado
    bool estado_led_azul = false;  // Inicializa o estado do LED azul como apagado
    
    // Implementação do debounce para evitar múltiplas leituras rápidas
    if (tempo_atual - ultima > 200) {        
         // Verifica qual botão foi pressionado    
        if (gpio == BUTTON_A) { 
            // Inverte o estado do LED verde ao pressionar o botão A
            gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));// Inverte o estado do LED verde
            estado_led_verde = gpio_get(LED_PIN_GREEN);
            printf("Botão A pressionado! LED Verde %s\n", estado_led_verde ? "Ligado" : "Desligado");
                                  
            // Atualiza o display OLED com a mensagem sobre o estado do LED verde
            if (gpio_get(LED_PIN_GREEN)) {
                // LED Verde Ligado
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
                ssd1306_draw_string(&ssd, "LED Verde", 30, 20); 
                ssd1306_draw_string(&ssd, "Ligado", 30, 40); 
                desenho(10); // Desenha LEDs apagados na matriz
            } else {
                // LED Verde Desligado
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo                
                ssd1306_draw_string(&ssd, "LED Verde", 30, 20); 
                ssd1306_draw_string(&ssd, "Desligado", 30, 40); 
            }
            ssd1306_send_data(&ssd); // Atualiza o display
            
                       

        } else if (gpio == BUTTON_B) {
            // Inverte o estado do LED azul ao pressionar o botão B
            gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
            estado_led_azul = gpio_get(LED_PIN_BLUE);
            printf("Botão A pressionado! LED Azul %s\n", estado_led_azul ? "Ligado" : "Desligado");
            
            // Atualiza o display OLED com a mensagem sobre o estado do LED azul
            if (gpio_get(LED_PIN_BLUE)) {
                // LED Azul Ligado
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
                ssd1306_draw_string(&ssd, "LED Azul", 30, 20); 
                ssd1306_draw_string(&ssd, "Ligado", 30, 40); 
                desenho(10); // Desenha LEDs apagados na matriz
            } else {
                // LED Azul Desligado
                ssd1306_fill(&ssd, !cor); // Limpa o display
                ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo                
                ssd1306_draw_string(&ssd, "LED Azul", 30, 20); 
                ssd1306_draw_string(&ssd, "Desligado", 30, 40); 
            }
            ssd1306_send_data(&ssd); // Atualiza o display
        }               
            ultima = tempo_atual; // Atualiza o tempo do último evento
        }
}

int main()
{
    // Inicializa a entrada/saída padrão
    stdio_init_all();

    // Inicializa os botões e LEDs
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);

    // Inicializa a PIO para controlar os LEDs RGB
    pio = pio0;
    uint offset = pio_add_program(pio, &led_program); 
    uint sm = pio_claim_unused_sm(pio, true);
    led_program_init(pio, sm, offset, 7);

    // Inicializa a comunicação I2C e configura o display OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    char mensagem[2] = {0};  // Buffer para armazenar a mensagem

    // Configura a interrupção para os botões A e B
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);


// Loop principal
    while (true) {
        int number;
        int c = getchar_timeout_us(1000000);  // Aguarda até 1 segundo por um caractere

        if (c != PICO_ERROR_TIMEOUT && c >= 32 && c <= 126) {  // Verifica se o caractere é imprimível
            mensagem[0] = (char)c;
            mensagem[1] = '\0';  // Termina a string corretamente

            // Verifica se o caractere é um número
            if (c >= '0' && c <= '9') {
                number = c - '0'; // Converte o caractere para número
                // Exibe o número no display
                switch (number) {
                    case 0:
                        desenho(0);
                        break;
                    case 1:
                        desenho(1);
                        break;
                    case 2:
                        desenho(2);
                        break;
                    case 3:
                        desenho(3);
                        break;
                    case 4:
                        desenho(4);
                        break;
                    case 5:
                        desenho(5);
                        break;
                    case 6:
                        desenho(6);
                        break;
                    case 7:
                        desenho(7);
                        break;
                    case 8:
                        desenho(8);
                        break;
                    case 9:
                        desenho(9);
                        break;
                }
            }

            // Limpa o display e exibe o número
            ssd1306_fill(&ssd, false);
            char str[10];  // Buffer para string
            snprintf(str, sizeof(str), "Numero %c", c);  // Formata a string "Numero X"
            ssd1306_draw_string(&ssd, str, 30, 20);  // Exibe o número
            ssd1306_send_data(&ssd);
        }

        sleep_ms(1000);  // Aguarda um segundo antes de verificar novamente
    }

    return 0;
}