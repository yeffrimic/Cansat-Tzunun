/*
 * Uso de SIM800L con MQTT para Heltec LoRa 32 V3
 * Conexiones:
 * - TX SIM800L -> GPIO47 (RX ESP32)
 * - RX SIM800L -> GPIO48 (TX ESP32)
 * - Alimentación desde pines VBAT (3.7-4.2V)
 * 
 * APNs para Guatemala:
 * - Tigo: internet.tigo.gt
 * - Claro: internet.ideasclaro
 * 
 * Broker MQTT público de prueba: broker.hivemq.com
 * Tópico: proyectotzunun/[ID_DISPOSITIVO]
 */

#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

// Configuración de pines
#define SIM800_RX_PIN 47
#define SIM800_TX_PIN 48

// Configuración APN (descomentar tu operadora)
const char apn[] = "internet.tigo.gt";        // Tigo Guatemala
//const char apn[] = "internet.ideasclaro";    // Claro Guatemala

const char gprsUser[] = "";
const char gprsPass[] = "";

// Configuración MQTT
const char* broker = "broker.hivemq.com";
const int port = 1883;
const char* topic = "proyectotzunun/heltec_01"; // Cambiar por tu ID único
const char* mqttUser = "";
const char* mqttPass = "";

SoftwareSerial sim800Serial(SIM800_RX_PIN, SIM800_TX_PIN);
TinyGsm modem(sim800Serial);
TinyGsmClient gsmClient(modem);
PubSubClient mqtt(gsmClient);

void setup() {
  Serial.begin(115200);
  delay(3000); // Esperar inicialización Serial

  Serial.println("\nProyecto Tzunun - MQTT con SIM800L");
  
  // Inicializar módem
  sim800Serial.begin(9600);
  delay(6000); // Tiempo extra para inicialización SIM800L

  Serial.println("Inicializando modem...");
  if (!modem.init()) {
    Serial.println("Fallo al inicializar modem");
    while(true);
  }

  Serial.println("Conectando a red...");
  if (!modem.waitForNetwork()) {
    Serial.println("Fallo al conectar a red");
    while(true);
  }

  Serial.println("Conectando a APN...");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Fallo al conectar a APN");
    while(true);
  }

  Serial.println("Conectado a GPRS");
  printModemInfo();

  // Configurar MQTT
  mqtt.setServer(broker, port);
  mqtt.setCallback(mqttCallback);
}

void loop() {
  static unsigned long lastReconnectAttempt = 0;
  static unsigned long lastMsgTime = 0;

  // Reconectar MQTT si se perdió la conexión
  if (!mqtt.connected()) {
    if (millis() - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = millis();
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
      }
    }
    delay(100);
    return;
  }

  // Publicar datos cada 60 segundos
  if (millis() - lastMsgTime > 60000) {
    sendMQTTData();
    lastMsgTime = millis();
  }

  mqtt.loop();
}

bool mqttConnect() {
  Serial.print("Conectando a MQTT...");
  
  // ID único para este dispositivo
  String clientId = "Heltec_" + String(random(0xffff), HEX);
  
  if (mqtt.connect(clientId.c_str(), mqttUser, mqttPass)) {
    Serial.println("Conectado");
    
    // Suscribirse a tópicos
    mqtt.subscribe(topic);
    return true;
  } else {
    Serial.print("Fallo, rc=");
    Serial.println(mqtt.state());
    return false;
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  
  for (int i = 0; i < len; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void sendMQTTData() {
  // Crear mensaje JSON con datos simulados
  String msg = "{\"temp\":" + String(random(20, 30)) + 
               ",\"hum\":" + String(random(40, 80)) + 
               ",\"volt\":" + String(random(350, 420)/100.0) + "}";
  
  if (mqtt.publish(topic, msg.c_str())) {
    Serial.println("Datos enviados:");
    Serial.println(msg);
  } else {
    Serial.println("Fallo al enviar datos");
  }
}

void printModemInfo() {
  Serial.println("\n=== Información del Módulo ===");
  Serial.print("Operador: ");
  Serial.println(modem.getOperator());
  Serial.print("RSSI: ");
  Serial.println(modem.getSignalQuality());
  Serial.print("Dirección IP: ");
  Serial.println(modem.getLocalIP());
}