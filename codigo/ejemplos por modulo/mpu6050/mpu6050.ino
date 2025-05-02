/*
 * Este código lee datos del sensor MPU6050 (acelerómetro + giroscopio)
 * y los envía al Serial Plotter de Arduino para visualización gráfica.
 * 
 * Características:
 * - Mide aceleración en ejes X/Y/Z (en g)
 * - Mide rotación en ejes X/Y/Z (en grados/segundo)
 * - Filtro de ruido básico
 * - Formato compatible con Serial Plotter
 * hecho por yeffrimic
 
 */

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("Proyecto Tzunun - MPU6050 + Heltec LoRa 32 V3");


  if (!mpu.begin()) {
    Serial.println("¡Error! MPU6050 no detectado");
    while (1) {
      delay(10);
    }
  }

  // Configuración del sensor
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);   // Rango de ±8g
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);        // Rango de ±500°/s
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);     // Filtro de 21Hz
  Serial.println("MPU6050 configurado correctamente");
  
  // Encabezados para Serial Plotter
  Serial.println("Aceleracion_X,Aceleracion_Y,Aceleracion_Z,Rotacion_X,Rotacion_Y,Rotacion_Z");
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp); // Lee datos

  // Formato para Serial Plotter (csv)
  Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print(g.gyro.x); Serial.print(",");
  Serial.print(g.gyro.y); Serial.print(",");
  Serial.println(g.gyro.z);

  delay(50); // Muestra 20 puntos/segundo (óptimo para ploteo)
}