/*
 * Este código lee una fotorresistencia (LDR) conectada al GPIO7
 * y muestra los valores de luz en el Serial Plotter.
 * 
 * Características:
 * - Lectura analógica del LDR (0-4095 en ESP32)
 * - Mapeo a porcentaje de luminosidad (0-100%)
 * - Filtro para suavizar lecturas
 * - Salida compatible con Serial Plotter
 */

const int pinLDR = 7;       // Pin GPIO7 para la fotorresistencia
const int muestras = 10;    // Número de muestras para el filtro promedio

void setup() {
  Serial.begin(115200);
  pinMode(pinLDR, INPUT);   // Configura el pin como entrada

  Serial.println("Proyecto Tzunun - Fotorresistencia en Heltec LoRa 32 V3");
  Serial.println("Luminosidad(%),Valor_Crudo");  // Encabezados para Serial Plotter
}

void loop() {
  int valorRaw = 0;
  
  // Filtro de promedio móvil
  for (int i = 0; i < muestras; i++) {
    valorRaw += analogRead(pinLDR);
    delay(10);
  }
  valorRaw /= muestras;

  // Mapear a porcentaje (ajustar según tus valores mín/máx)
  int porcentaje = map(valorRaw, 0, 4095, 0, 100);

  // Enviar datos al Serial Plotter
  Serial.print(porcentaje);
  Serial.print(",");
  Serial.println(valorRaw);

  delay(100);  // Intervalo entre lecturas
}