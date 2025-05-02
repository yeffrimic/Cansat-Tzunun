/*
 * Lectura de datos GPS con módulo NEO-6M V2
 * Conexiones:
 * - GPS TX -> GPIO38 (RX ESP32)
 * - GPS RX -> GPIO01 (TX ESP32)
 * - Alimentación: 3.3V o 5V (según versión del módulo)
 * 
 * Recomendaciones:
 * 1. Probar en espacio abierto con vista despejada al cielo
 * 2. La primera conexión puede tardar varios minutos (cold start)
 * 3. El LED del módulo parpadeará cuando tenga fix satelital
 */

#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Configuración pines Serial2 (GPIO1 y GPIO38 en Heltec)
#define GPS_RX_PIN 38
#define GPS_TX_PIN 1

// Objetos
TinyGPSPlus gps;
HardwareSerial ss(2);  // Usar Serial2

void setup() {
  Serial.begin(115200);
  ss.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("\nProyecto Tzunun - GPS NEO-6M V2");
  Serial.println("Buscando satélites...");
  Serial.println("Recomendación: Colocar el módulo en espacio abierto");
  Serial.println("==============================================");
}

void loop() {
  // Leer datos del GPS
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayGPSData();
    }
  }

  // Si no hay señal después de 5 segundos
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("Error: No se detectan datos del GPS. Verificar conexiones.");
    while(true);
  }
}

void displayGPSData() {
  static unsigned long lastUpdate = 0;
  
  // Actualizar cada 2 segundos
  if (millis() - lastUpdate < 2000) return;
  lastUpdate = millis();

  Serial.print("\nHora: ");
  if (gps.time.isValid()) {
    Serial.printf("%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
  } else {
    Serial.print("INVALIDA");
  }

  Serial.print(" | Satélites: ");
  if (gps.satellites.isValid()) {
    Serial.print(gps.satellites.value());
  } else {
    Serial.print("0");
  }

  Serial.print(" | Fix: ");
  if (gps.location.isValid()) {
    Serial.print("3D");
  } else {
    Serial.print("NO");
  }

  if (gps.location.isValid()) {
    Serial.print("\nLatitud: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" | Longitud: ");
    Serial.print(gps.location.lng(), 6);
    Serial.print("\nAltitud: ");
    Serial.print(gps.altitude.meters());
    Serial.print(" m | Velocidad: ");
    Serial.print(gps.speed.kmph());
    Serial.print(" km/h");
  }

  Serial.println("\n----------------------------------");
}