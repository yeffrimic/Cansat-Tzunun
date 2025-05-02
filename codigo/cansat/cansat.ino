#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_MPU6050.h>
#include <GP2YDustSensor.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_VERSION MQTT_VERSION_3_1_1
#define REASSIGN_PINS
int sck = 35;
int miso = 33;
int mosi = 34;
int cs = 36;

unsigned long previousMillis = 0;
const long interval = 1000;  // Aumentar el intervalo a 5 segundos

const uint8_t SHARP_LED_PIN = 6;
const uint8_t SHARP_VO_PIN = A3;

const char* ssid = "Xibalba Hackerspace";
const char* password = "ESTALIBRE";

const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "/cansat/umes/mision";
char mqtt_client_id[20];

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
Adafruit_MPU6050 mpu;
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1014AU0F, SHARP_LED_PIN, SHARP_VO_PIN);

const float BMP_TEMP_MIN = -20.0;
const float BMP_TEMP_MAX = 85.0;
const float PRESSURE_MIN = 500.0;
const float PRESSURE_MAX = 900.0;
const float ALTITUDE_MIN = -200.0;
const float ALTITUDE_MAX = 5000.0;
const float AHT_TEMP_MIN = -40.0;
const float AHT_TEMP_MAX = 85.0;
const float HUMIDITY_MIN = 0.0;
const float HUMIDITY_MAX = 100.0;
const float ACCEL_MIN = -16.0;
const float ACCEL_MAX = 16.0;
const float DUST_DENSITY_MIN = 0.0;
const float DUST_DENSITY_MAX = 500.0;

float bmpTemperature = 0.0;
float pressure = 0.0;
float altitude = 0.0;
float ahtTemperature = 0.0;
float ahtHumidity = 0.0;
float accelX = 0.0;
float accelY = 0.0;
float accelZ = 0.0;
float dustDensity = 0.0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  snprintf(mqtt_client_id, sizeof(mqtt_client_id), "cansat%d", random(0, 9999));
  Serial.println(mqtt_client_id);
  while (!client.connected()) {
    Serial.print("Intentando conectar al broker MQTT...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

bool validateSensorValue(float value, float minValue, float maxValue) {
  return (value >= minValue && value <= maxValue);
}

void setup() {
  Serial.begin(115200);

  //SPI.begin(sck, miso, mosi, cs);
/*
  if (!SD.begin(cs)) {
    Serial.println("Error al montar la tarjeta SD");
    return;
  }
*/
  if (!bmp.begin()) {
    Serial.println("BMP180 no detectado");
    return;
  }

  if (!aht.begin()) {
    Serial.println("AHT10 no detectado");
    return;
  }
/*
  if (!mpu.begin()) {
    Serial.println("MPU6050 no detectado");
    return;
  }
  */

  dustSensor.begin();

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  Serial.println("Sensores inicializados correctamente");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float newBmpTemperature = bmp.readTemperature();
    float newPressure = bmp.readPressure() / 100.0F;
    float newAltitude = bmp.readAltitude();

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    float newAhtTemperature = temp.temperature;
    float newAhtHumidity = humidity.relative_humidity;
/*
    sensors_event_t accel, gyro, mpuTemp;
    mpu.getEvent(&accel, &gyro, &mpuTemp);
    float newAccelX = accel.acceleration.x;
    float newAccelY = accel.acceleration.y;
    float newAccelZ = accel.acceleration.z;
*/
    float newDustDensity = dustSensor.getDustDensity();

    if (validateSensorValue(newBmpTemperature, BMP_TEMP_MIN, BMP_TEMP_MAX)) {
      bmpTemperature = newBmpTemperature;
    }
    if (validateSensorValue(newPressure, PRESSURE_MIN, PRESSURE_MAX)) {
      pressure = newPressure;
    }
    if (validateSensorValue(newAltitude, ALTITUDE_MIN, ALTITUDE_MAX)) {
      altitude = newAltitude;
    }
    if (validateSensorValue(newAhtTemperature, AHT_TEMP_MIN, AHT_TEMP_MAX)) {
      ahtTemperature = newAhtTemperature;
    }
    if (validateSensorValue(newAhtHumidity, HUMIDITY_MIN, HUMIDITY_MAX)) {
      ahtHumidity = newAhtHumidity;
    }
    /*
    if (validateSensorValue(newAccelX, ACCEL_MIN, ACCEL_MAX)) {
      accelX = newAccelX;
    }
    if (validateSensorValue(newAccelY, ACCEL_MIN, ACCEL_MAX)) {
      accelY = newAccelY;
    }
    if (validateSensorValue(newAccelZ, ACCEL_MIN, ACCEL_MAX)) {
      accelZ = newAccelZ;
    }
    */
    if (validateSensorValue(newDustDensity, DUST_DENSITY_MIN, DUST_DENSITY_MAX)) {
      dustDensity = newDustDensity;
    }

    char dataString[250];
    snprintf(dataString, sizeof(dataString), "%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f",
             bmpTemperature, pressure, altitude, ahtTemperature, ahtHumidity, 0, 0, 0, dustDensity);
/*
    appendFile(SD, "/sensor_data.txt", dataString);
    appendFile(SD, "/sensor_data.txt", "\n");
    */
    Serial.print("Datos guardados: ");
    Serial.println(dataString);

    client.publish(mqtt_topic, dataString);
    Serial.print("Datos publicados: ");
    Serial.println(dataString);
  }
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Agregando al archivo: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Error al abrir el archivo para agregar datos");
    return; 
  }
  if (file.print(message)) {
    Serial.println("Mensaje agregado");
  } else {
    Serial.println("Error al agregar al archivo");
  }
  file.close();
}