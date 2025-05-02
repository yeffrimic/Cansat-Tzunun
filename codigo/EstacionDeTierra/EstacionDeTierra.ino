#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_AHTX0.h>
#include <GP2YDustSensor.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define MQTT_VERSION MQTT_VERSION_3_1_1
#define REASSIGN_PINS

unsigned long previousMillis = 0;
const long interval = 1000 * 60 * 5;  // Aumentar el intervalo a 5 segundos

const uint8_t SHARP_LED_PIN = 6;
const uint8_t SHARP_VO_PIN = A4;

const char* ssid = "Xibalba Hackerspace";
const char* password = "ESTALIBRE";

const char* mqtt_server = "broker.emqx.io";
const char* mqtt_topic = "/cansat/umes/";
char mqtt_client_id[20];

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_BMP085 bmp;
Adafruit_AHTX0 aht;
GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1014AU0F, SHARP_LED_PIN, SHARP_VO_PIN);
#define ID 1
#define BMP_TEMP_MIN -20.0
#define BMP_TEMP_MAX 85.0
#define PRESSURE_MIN 500.0
#define PRESSURE_MAX 900.0
#define ALTITUDE_MIN -200.0
#define ALTITUDE_MAX 5000.0
#define AHT_TEMP_MIN -40.0
#define AHT_TEMP_MAX 85.0
#define HUMIDITY_MIN 0.0
#define HUMIDITY_MAX 100.0
#define DUST_DENSITY_MIN 0.0
#define DUST_DENSITY_MAX 1000.0

float bmpTemperature = 0.0;
float pressure = 0.0;
float altitude = 0.0;
float ahtTemperature = 0.0;
float ahtHumidity = 0.0;
float dustDensity = 0.0;

void setup_wifi() {
  delay(1000);
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

  WiFi.mode(WIFI_STA);

  setup_wifi();
  delay(1000);
  if (!bmp.begin()) {
    Serial.println("BMP180 no detectado");
    return;
  }

  if (!aht.begin()) {
    Serial.println("AHT10 no detectado");
    return;
  }

  dustSensor.begin();

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
    if (validateSensorValue(newDustDensity, DUST_DENSITY_MIN, DUST_DENSITY_MAX)) {
      dustDensity = newDustDensity;
    }

    // Leer la fotoresistencia en el pin A6
    int photoResistorValue = analogRead(A6);

    char dataString[250];
    snprintf(dataString, sizeof(dataString), "%d,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%d",
             ID,
             bmpTemperature,
             pressure,
             altitude,
             ahtTemperature,
             ahtHumidity,
             0.0,  // Ensure all floating-point values are properly formatted
             0.0,
             0.0,
             dustDensity,
             photoResistorValue);
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

void appendFile(fs::FS& fs, const char* path, const char* message) {
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