/*
 * Manejo de tarjeta MicroSD con SPI personalizado
 * Conexiones Heltec LoRa 32 V3:
 * - SCK  = GPIO35
 * - MISO = GPIO33
 * - MOSI = GPIO34
 * - CS   = GPIO36
 * 
 * Librería SD modificada para usar pines no estándar
 */

#include <SPI.h>
#include <SD.h>

// Configuración de pines SPI personalizados
const int SCK_PIN = 35;
const int MISO_PIN = 33;
const int MOSI_PIN = 34;
const int CS_PIN = 36;

// Objeto SPI personalizado
SPIClass customSPI(VSPI);

void setup() {
  Serial.begin(115200);
  while(!Serial); // Esperar conexión serial en placas con USB nativo

  Serial.println("\nProyecto Tzunun - Sistema de Archivos en MicroSD");
  
  // Configurar SPI con pines personalizados
  customSPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  
  Serial.println("Inicializando tarjeta SD...");
  if(!SD.begin(CS_PIN, customSPI)) {
    Serial.println("¡Error al inicializar la tarjeta SD!");
    Serial.println("Verifica:");
    Serial.println("1. Conexiones de los pines");
    Serial.println("2. Formato de la tarjeta (FAT32)");
    Serial.println("3. Que la tarjeta esté insertada correctamente");
    return;
  }
  
  Serial.println("Tarjeta SD inicializada correctamente");
  
  // Crear archivo de prueba
  crearArchivo("/prueba.txt");
  
  // Leer archivo
  leerArchivo("/prueba.txt");
  
  // Listar directorio raíz
  listarDirectorio("/");
}

void loop() {
  // Nada en el loop para este ejemplo
}

// Función para crear/sobreescribir archivo
void crearArchivo(const char* filename) {
  File archivo = SD.open(filename, FILE_WRITE);
  
  if(archivo) {
    Serial.print("\nEscribiendo en archivo: ");
    Serial.println(filename);
    
    archivo.println("Datos de prueba - Proyecto Tzunun");
    archivo.println("Fecha: " + obtenerFecha());
    archivo.println("Hora: " + obtenerHora());
    archivo.println("Valor aleatorio: " + String(random(100)));
    
    archivo.close();
    Serial.println("Escritura completada");
  } else {
    Serial.print("Error al abrir ");
    Serial.println(filename);
  }
}

// Función para leer archivo
void leerArchivo(const char* filename) {
  File archivo = SD.open(filename);
  
  if(archivo) {
    Serial.print("\nLeyendo archivo: ");
    Serial.println(filename);
    
    while(archivo.available()) {
      Serial.write(archivo.read());
    }
    archivo.close();
  } else {
    Serial.print("Error al leer ");
    Serial.println(filename);
  }
}

// Función para listar directorios
void listarDirectorio(const char* dirname) {
  Serial.print("\nContenido del directorio: ");
  Serial.println(dirname);

  File root = SD.open(dirname);
  
  if(!root) {
    Serial.println("Error al abrir directorio");
    return;
  }
  
  if(!root.isDirectory()) {
    Serial.println("No es un directorio");
    return;
  }

  File entry = root.openNextFile();
  while(entry) {
    if(entry.isDirectory()) {
      Serial.print("DIR : ");
      Serial.println(entry.name());
    } else {
      Serial.print("FILE: ");
      Serial.print(entry.name());
      Serial.print("\tTAMAÑO: ");
      Serial.println(entry.size());
    }
    entry = root.openNextFile();
  }
  
  root.close();
}

// Función para obtener fecha ficticia (implementar RTC para precisión)
String obtenerFecha() {
  return "2023-11-15";
}

// Función para obtener hora ficticia
String obtenerHora() {
  return "14:30:00";
}