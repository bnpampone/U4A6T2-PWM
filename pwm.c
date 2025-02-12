#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_GPIO 22  // Pino do Servo
#define LED_GPIO 12  // Pino do LED
#define PWM_FREQ 50 // Frequência de 50Hz
#define WRAP_VALUE 20000 // Tempo total para o PWM 50Hz

// Configuração de inicialização do PWM para servo & LED
void setup_pwm(){ 
    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM); // Habilita o pino do servo como PWM
    gpio_set_function(LED_GPIO, GPIO_FUNC_PWM); // Habilita o pino do LED como PWM
    uint slice_servo = pwm_gpio_to_slice_num(PWM_GPIO); // Obtém o slice do servo
    uint slice_led = pwm_gpio_to_slice_num(LED_GPIO);   // Obtém o slice do LED

    pwm_set_wrap(slice_servo, WRAP_VALUE); // Configura o PWM do servo
    pwm_set_wrap(slice_led, WRAP_VALUE);   // Configura o PWM do LED
    
    pwm_set_clkdiv(slice_servo, 125.0f); // Mantém o clock do servo padrão (125hz)
    pwm_set_clkdiv(slice_led, 125.0f);   // Mantém o clock do LED padrão (125hz)

    pwm_set_enabled(slice_servo, true); // Ativa o pwm do servo
    pwm_set_enabled(slice_led, true); // Ativa o pwm do led
}

// Função para movimentação suave do servo e controle do LED
void smooth_mov(){

    // Movimento suave de 0° até 180° (500us → 2500us)
    for (uint level = 500; level <= 2500; level += 5) {
        pwm_set_gpio_level(PWM_GPIO, level);
        pwm_set_gpio_level(LED_GPIO, WRAP_VALUE - level); // LED vai aumentando conforme o servo sobe
        sleep_ms(10); // Atraso para suavidade
    }
    
    // Movimento suave de 180° de volta para 0° (2500us → 500us)
    for (uint level = 2500; level >= 500; level -= 5) {
        pwm_set_gpio_level(PWM_GPIO, level);
        pwm_set_gpio_level(LED_GPIO, WRAP_VALUE - level); // LED vai apagando conforme o servo desce
        sleep_ms(10); // Atraso para suavidade
    }

    // Garante que o LED está apagado no final
    pwm_set_gpio_level(LED_GPIO, false); // Intensidade de 0 - 255, Logo desliga o led totalmente
}

// Função principal
int main(){
    stdio_init_all();
    setup_pwm();


    // Movimentos iniciais do servo
    pwm_set_gpio_level(PWM_GPIO, 2400); // 180 graus
    pwm_set_gpio_level(LED_GPIO, WRAP_VALUE); // LED no brilho máximo
    sleep_ms(5000);

    pwm_set_gpio_level(PWM_GPIO, 1470); // 90 graus
    pwm_set_gpio_level(LED_GPIO, WRAP_VALUE / 2); // LED em metade do brilho
    sleep_ms(5000);

    pwm_set_gpio_level(PWM_GPIO, 500); // 0 graus
    pwm_set_gpio_level(LED_GPIO, 0); // LED completamente apagado
    sleep_ms(5000);

    while (true) {
        smooth_mov(); // Movimentação contínua do servo e LED
    }
}
