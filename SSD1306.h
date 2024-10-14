#ifndef SSD1306_H
#define SSD1306_H

#include "mbed.h"

class SSD1306 {
public:
    SSD1306(I2C& i2c, uint8_t address = 0x3C);  // Constructor con dirección I2C predeterminada

    void init();                    // Inicializar el display
    void clear();                   // Limpiar la pantalla
    void set_cursor(uint8_t x, uint8_t y);  // Establecer el cursor en (x, y)
    void printf(const char* format, ...);   // Función de impresión con formato
    void set_contrast(uint8_t contrast);   // Establecer el contraste del display
    void draw_pixel(uint8_t x, uint8_t y, uint8_t color); // Dibujar un píxel en (x, y)

private:
    void send_command(uint8_t command);    // Enviar comando al display
    void send_data(uint8_t data);         // Enviar datos al display
    void write_data(const uint8_t* data, uint16_t size); // Enviar bloque de datos

    I2C& _i2c;          // Objeto I2C
    uint8_t _address;   // Dirección del display SSD1306
};

#endif // SSD1306_H
