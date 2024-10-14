#include "SSD1306.h"
#include <stdarg.h>
#include <stdio.h>

SSD1306::SSD1306(I2C& i2c, uint8_t address) : _i2c(i2c), _address(address) {}

void SSD1306::init() {
    send_command(0xAE);  // Apagar el display
    send_command(0xD5);  // Configurar frecuencia de reloj
    send_command(0x80);  // Establecer frecuencia a 0x80
    send_command(0xA8);  // Configurar multiplex ratio
    send_command(0x3F);  // Establecer multiplex ratio en 64
    send_command(0xD3);  // Ajustar desplazamiento de la pantalla
    send_command(0x00);  // Sin desplazamiento
    send_command(0x40);  // Configurar la línea base de la pantalla en 0x40
    send_command(0x8D);  // Activar el modo de carga de la pantalla
    send_command(0x14);  // Habilitar la carga de la pantalla
    send_command(0xA1);  // Configurar la dirección de la columna
    send_command(0xC8);  // Configurar dirección de la página
    send_command(0xDA);  // Configurar la configuración del hardware
    send_command(0x12);  // Seleccionar configuración de hardware
    send_command(0x81);  // Establecer el contraste
    send_command(0x7F);  // Contraste máximo
    send_command(0xA4);  // Habilitar la memoria RAM
    send_command(0xA6);  // Configurar la polaridad normal
    send_command(0xAF);  // Encender la pantalla
}

void SSD1306::clear() {
    for (uint16_t i = 0; i < 1024; i++) {
        send_data(0x00);  // Enviar 0 a cada posición de la pantalla
    }
}

void SSD1306::set_cursor(uint8_t x, uint8_t y) {
    // Verificar si la posición está dentro de los límites de la pantalla
    if (x >= 128 || y >= 8) {
        printf("Warning: Cursor position out of bounds (x: %d, y: %d)\n", x, y);
        return;  // Si los valores son incorrectos, salir
    }

    send_command(0xB0 + y);  // Configurar la página (línea de pantalla)
    send_command(0x00 + (x & 0x0F));  // Configurar columna baja
    send_command(0x10 + ((x >> 4) & 0x0F));  // Configurar columna alta
}

void SSD1306::printf(const char* format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    for (size_t i = 0; buffer[i] != '\0'; i++) {
        // Enviar el código ASCII de cada carácter
        send_data(buffer[i]);
    }
}

void SSD1306::set_contrast(uint8_t contrast) {
    send_command(0x81);  // Comando para establecer el contraste
    send_command(contrast);  // Establecer contraste
}

void SSD1306::draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
    set_cursor(x, y);
    send_data(color);  // Enviar datos para dibujar un píxel
}

void SSD1306::send_command(uint8_t command) {
    char data[2] = {0x00, command};  // Primer byte indica que es un comando
    int result = _i2c.write(_address, data, 2);
    if (result != 0) {
        printf("Error sending command: %d\n", result);  // Mensaje de error si no se pudo enviar
    }
}

void SSD1306::send_data(uint8_t data) {
    char buf[2] = {0x40, data};  // Primer byte indica que es un dato
    int result = _i2c.write(_address, buf, 2);
    if (result != 0) {
        printf("Error sending data: %d\n", result);  // Mensaje de error si no se pudo enviar
    }
}

void SSD1306::write_data(const uint8_t* data, uint16_t size) {
    if (size == 0) {
        return;  // Si no hay datos que enviar, salir
    }

    char buf[size + 1];
    buf[0] = 0x40;  // Indicar que los siguientes son datos
    memcpy(buf + 1, data, size);  // Copiar datos al búfer

    int result = _i2c.write(_address, buf, size + 1);
    if (result != 0) {
        printf("Error sending data block: %d\n", result);  // Manejo de errores
    }
}
