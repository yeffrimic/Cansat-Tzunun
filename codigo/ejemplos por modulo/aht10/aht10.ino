/*
 * Este código muestra cómo usar el sensor AHT10 con el ESP32 (Heltec LoRa 32 V3).
 * El AHT10 es un sensor de temperatura y humedad de alta precisión.
 * - Configura comunicación I2C.
 * - Lee datos del sensor y los muestra por Serial.
 * - Compatible con Heltec LoRa 32 V3.
 * hecho por Yeffrimic
 */

#include <Wire.h>          // Biblioteca para comunicación I2C
#include <Adafruit_AHTX0.h> // Biblioteca para el sensor AHT10

Adafruit_AHTX0 aht; // Objeto para interactuar con el sensor

void setup() {
  Serial.begin(115200); // Inicia comunicación serial (115200 baudios)
  while (!Serial) {     // Espera a que el puerto serial esté listo (solo para USB)
    delay(10);
  }

  Serial.println("\nProyecto Tzunun - Sensor AHT10 + Heltec LoRa 32 V3"); // Mensaje de inicio

  if (!aht.begin()) { // Intenta inicializar el AHT10
    Serial.println("¡Error! Sensor AHT10 no detectado. Verifica:");
    Serial.println("- Conexiones I2C (SDA/SCL)");
    Serial.println("- Alimentación (3.3V)");
    while (1) delay(10); // Bucle infinito si falla
  }

  Serial.println("AHT10 configurado correctamente");
}

void loop() {
  sensors_event_t humidity, temp; // Estructuras para almacenar datos

  aht.getEvent(&humidity, &temp); // Lee temperatura (°C) y humedad (%)
  
  // Muestra datos por Serial:
  Serial.print("Temperatura: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.print("Humedad: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  delay(2000); // Espera 2 segundos entre lecturas
}