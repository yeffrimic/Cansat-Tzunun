/*
 * Sistema que:
 * 1. Conecta a WiFi
 * 2. Lee/Escribe en tarjeta MicroSD
 * 3. Envía datos por MQTT
 * 4. Registra eventos en SD
 * 
 * Conexiones:
 * MicroSD:
 * - SCK  = GPIO35
 * - MISO = GPIO33
 * - MOSI = GPIO34
 * - CS   = GPIO36
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <SD.h>

// Configuración WiFi
const char* ssid = "Tu_SSID";
const char* password = "Tu_PASSWORD";

// Configuración MQTT
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "proyectotzunun/datos";
const char* mqtt_client_id = "HeltecSD_01";

// Configuración SPI personalizado para SD
const int SCK_PIN = 35;
const int MISO_PIN = 33;
const int MOSI_PIN = 34;
const int CS_PIN = 36;
SPIClass customSPI(VSPI);

// Objetos
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Iniciar SD con SPI personalizado
  iniciarSD();
  
  // Conectar a WiFi
  conectarWiFi();
  
  // Configurar MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop();

  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) {  // Enviar cada 10 segundos
    String datos = generarDatos();
    
    // Guardar en SD
    guardarEnSD("/datalog.txt", datos);
    
    // Enviar por MQTT
    enviarPorMQTT(datos);
    
    lastSend = millis();
  }
}

void iniciarSD() {
  customSPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  
  Serial.println("Iniciando tarjeta SD...");
  if (!SD.begin(CS_PIN, customSPI)) {
    Serial.println("¡Error al iniciar SD!");
    registrarError("Falló inicio de SD");
    return;
  }
  Serial.println("SD iniciada correctamente");
  guardarEnSD("/log.txt", "Sistema iniciado - " + obtenerFechaHora());
}

void conectarWiFi() {
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 10) {
    delay(500);
    Serial.print(".");
    intentos++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a WiFi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    guardarEnSD("/log.txt", "Conectado WiFi - IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\n¡Error al conectar WiFi!");
    registrarError("Falló conexión WiFi");
  }
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    
    if (client.connect(mqtt_client_id)) {
      Serial.println("Conectado");
      guardarEnSD("/log.txt", "Conectado a MQTT: " + String(mqtt_server));
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" Reintentando en 5s");
      registrarError("Falló MQTT rc=" + String(client.state()));
      delay(5000);
    }
  }
}

void enviarPorMQTT(String mensaje) {
  if (client.publish(mqtt_topic, mensaje.c_str())) {
    Serial.println("Datos enviados por MQTT");
  } else {
    Serial.println("¡Error al enviar por MQTT!");
    registrarError("Falló envío MQTT");
  }
}

String generarDatos() {
  // Simular datos de sensores
  float temperatura = random(200, 350) / 10.0;
  float humedad = random(400, 800) / 10.0;
  
  String datos = "{\"temp\":" + String(temperatura) + 
                 ",\"hum\":" + String(humedad) + 
                 ",\"time\":\"" + obtenerFechaHora() + "\"}";
  
  Serial.println("Datos generados: " + datos);
  return datos;
}

void guardarEnSD(String filename, String contenido) {
  File archivo = SD.open(filename, FILE_APPEND);
  
  if (archivo) {
    archivo.println(contenido);
    archivo.close();
  } else {
    Serial.println("Error al abrir archivo en SD");
  }
}

void registrarError(String mensaje) {
  String errorMsg = obtenerFechaHora() + " - " + mensaje;
  guardarEnSD("/errores.txt", errorMsg);
}

String obtenerFechaHora() {
  // Para producción implementar RTC
  return "2023-11-15 " + String(millis() / 1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  // Guardar mensajes MQTT recibidos
  String mensaje;
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  guardarEnSD("/mqtt_in.txt", obtenerFechaHora() + " - " + String(topic) + ": " + mensaje);
}