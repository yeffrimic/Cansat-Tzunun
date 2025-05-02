/*
 * Uso del módulo SIM800L con TinyGSM para Heltec LoRa 32 V3
 * Conexiones:
 * - Alimentación: Pines de batería de la placa (VBAT)
 * - TX SIM800L -> GPIO47 (RX ESP32)
 * - RX SIM800L -> GPIO48 (TX ESP32)
 * - GND -> GND común
 * 
 * APNs para Guatemala:
 * - Tigo: internet.tigo.gt
 * - Claro: internet.ideasclaro
 * - Movistar: internet.movistar.com
 * 
 * Funcionalidades:
 * - Conexión a internet móvil
 * - Consulta a API pública de ejemplo (httpbin.org)
 * - Recepción básica de SMS
 */

#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

// Configuración de pines
#define SIM800_RX_PIN 47  // Conectar a TX del SIM800L
#define SIM800_TX_PIN 48  // Conectar a RX del SIM800L

// Configuración APN (descomentar el de tu operadora)
const char apn[] = "internet.tigo.gt";        // Tigo Guatemala
//const char apn[] = "internet.ideasclaro";    // Claro Guatemala
//const char apn[] = "internet.movistar.com";  // Movistar

const char gprsUser[] = "";  // Usuario (normalmente vacío)
const char gprsPass[] = "";  // Contraseña (normalmente vacía)

// API pública de prueba
const char server[] = "httpbin.org";
const int port = 80;

SoftwareSerial sim800Serial(SIM800_RX_PIN, SIM800_TX_PIN);
TinyGsm modem(sim800Serial);

void setup() {
  Serial.begin(115200);
  delay(3000);  // Esperar inicialización Serial

  Serial.println("\nProyecto Tzunun - SIM800L con TinyGSM");
  Serial.println("Inicializando módulo GSM...");
  
  // Inicializar comunicación con módem
  sim800Serial.begin(9600);
  delay(3000);  // Esperar inicio del módulo

  // Inicializar modem
  if (!modem.init()) {
    Serial.println("Fallo al inicializar modem");
    while (true);
  }

  Serial.println("Conectando a red móvil...");
  if (!modem.waitForNetwork()) {
    Serial.println("Fallo al conectar a red");
    while (true);
  }

  Serial.println("Conectando a APN...");
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    Serial.println("Fallo al conectar a APN");
    while (true);
  }

  Serial.println("Conectado a GPRS");
  printModemInfo();
}

void loop() {
  // Consultar API cada 2 minutos
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 120000) {
    consultarAPI();
    lastUpdate = millis();
  }

  // Verificar SMS entrantes
  checkSMS();

  delay(1000);
}

void printModemInfo() {
  Serial.println("\n=== Información del Módulo ===");
  Serial.print("Operador: ");
  Serial.println(modem.getOperator());
  Serial.print("Calidad señal: ");
  Serial.println(modem.getSignalQuality());
  Serial.print("Dirección IP: ");
  Serial.println(modem.getLocalIP());
}

void consultarAPI() {
  Serial.println("\nConsultando API pública...");
  
  TinyGsmClient client(modem);
  
  if (!client.connect(server, port)) {
    Serial.println("Fallo al conectar con el servidor");
    return;
  }

  // Enviar petición GET a httpbin.org/ip
  client.print("GET /ip HTTP/1.1\r\n");
  client.print("Host: httpbin.org\r\n");
  client.print("Connection: close\r\n\r\n");

  // Esperar respuesta
  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      timeout = millis();
    }
  }
  client.stop();
  Serial.println("\nConsulta completada");
}

void checkSMS() {
  if (modem.getSimStatus() != SimStatus::READY) return;

  // Verificar SMS no leídos
  int smsCount = modem.getUnreadCount();
  if (smsCount > 0) {
    Serial.println("\n=== SMS Recibidos ===");
    for (int i = 1; i <= smsCount; i++) {
      String sender = modem.getSender(i);
      String message = modem.readSMS(i);
      
      Serial.print("De: ");
      Serial.println(sender);
      Serial.print("Mensaje: ");
      Serial.println(message);
      
      modem.deleteSMS(i);  // Eliminar SMS leído
    }
  }
}