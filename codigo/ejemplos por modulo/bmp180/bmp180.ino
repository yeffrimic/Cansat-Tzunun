/*
 * Este código muestra cómo usar el sensor BMP180 con el ESP32 (Heltec LoRa 32 V3).
 * El BMP180 es un sensor de presión barométrica y temperatura.
 * - Configura comunicación I2C.
 * - Lee presión (hPa), altitud aproximada (m) y temperatura (°C).
 * hecho por yeffrimic
 */

#include <Wire.h>          // Biblioteca para I2C
#include <Adafruit_BMP085.h> // Biblioteca para BMP180

Adafruit_BMP085 bmp; // Objeto para el sensor

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("\nProyecto Tzunun - Sensor BMP180 + Heltec LoRa 32 V3");


  if (!bmp.begin()) { // Intenta inicializar el BMP180
    Serial.println("¡Error! BMP180 no detectado. Verifica:");
    Serial.println("- Conexiones I2C (SDA/SCL)");
    Serial.println("- Alimentación (3.3V)");
    while (1) delay(10);
  }

  Serial.println("BMP180 configurado correctamente");
}

void loop() {
  // Lectura de datos
  float temperatura = bmp.readTemperature(); // °C
  float presion = bmp.readPressure() / 100.0; // Convertir Pa a hPa
  float altitud = bmp.readAltitude(1013.25); // Ajustar presión a nivel del mar (hPa)

  // Muestra resultados por Serial
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  Serial.print("Presión: ");
  Serial.print(presion);
  Serial.println(" hPa");

  Serial.print("Altitud aproximada: ");
  Serial.print(altitud);
  Serial.println(" m");

  Serial.println("-----------------------");
  delay(2000); // Espera 2 segundos
}