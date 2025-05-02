/*
 * Este código mide la concentración de partículas en el aire usando el sensor GP2Y1010AU0F
 * Circuito requerido:
 * - Resistencia 150Ω en serie con LED
 * - Capacitor 220μF entre Vo y GND
 * hecho por yeffrimic
 */

const int ledPin = 6;      
const int voPin = A4;        
const int samplingTime = 280; // Tiempo de muestreo en μs
const int deltaTime = 40;   // Tiempo entre pulsos en μs
const int sleepTime = 9680; // Tiempo de espera entre lecturas en μs

// Parámetros de calibración (ajustar según mediciones)
const float voCleanAir = 0.6;  // Voltaje en aire limpio (typical 0.9V max)
const float densityCoeff = 0.2; // Coeficiente para conversión a μg/m³

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(voPin, INPUT);

  Serial.println("Proyecto Tzunun - GP2Y1010AU0F con Heltec LoRa 32 V3");
  Serial.println("Densidad(ug/m3),Voltaje(V),ValorADC");
}

void loop() {
  // 1. Activar LED infrarrojo
  digitalWrite(ledPin, LOW);
  delayMicroseconds(samplingTime);

  // 2. Leer valor analógico (ESP32 ADC de 12 bits)
  int rawValue = analogRead(voPin);
  float voltage = rawValue * (3.3 / 4095.0);

  // 3. Desactivar LED
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(deltaTime);

  // 4. Calcular densidad de partículas
  float dustDensity = (voltage - voCleanAir) / densityCoeff;
  dustDensity = dustDensity < 0 ? 0 : dustDensity; // No valores negativos

  // 5. Enviar datos al Serial Plotter
  Serial.print(dustDensity);
  Serial.print(",");
  Serial.print(voltage, 3);
  Serial.print(",");
  Serial.println(rawValue);

  // 6. Esperar para siguiente lectura
  delayMicroseconds(sleepTime);
  delay(1000); // Intervalo entre mediciones completas
}