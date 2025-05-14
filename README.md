# 🌎 Estación de Calidad del Aire – Proyecto CanSat Tzunun

Este repositorio contiene el código, documentación, archivos de diseño y recursos educativos para implementar una **estación de monitoreo ambiental**, así como su versión miniaturizada adaptada al formato **CanSat**. El proyecto fue desarrollado por estudiantes de la **Universidad Mesoamericana, sede Quetzaltenango**, como una herramienta de medición ambiental portátil, educativa y de bajo costo.

---

## 🚀 Guía de Instalación – Heltec WiFi LoRa 32 (V3) en Arduino IDE

### Paso 1: Instalar el Arduino IDE
Descarga e instala la versión más reciente desde: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

### Paso 2: Añadir URL del Gestor de Placas
1. Abre Arduino IDE.
2. Ve a **Archivo → Preferencias**.
3. En **"Gestor de URLs adicionales de tarjetas"**, añade: https://resource.heltec.cn/download/package_heltec_esp32_index.json
### Paso 3: Instalar el soporte para Heltec
1. Ve a **Herramientas → Placa → Gestor de tarjetas**.
2. Busca **Heltec ESP32** y haz clic en *Instalar*.

### Paso 4: Seleccionar la placa correcta
1. En **Herramientas → Placa**, selecciona:  
**Heltec WiFi LoRa 32 (V3)**.
2. Configura:
- Flash Frequency: 80 MHz  
- Upload Speed: 115200  
- Partition Scheme: Default 4MB with spiffs

### Paso 5: Instalar librerías necesarias
Desde el **Gestor de Librerías**, instala:
- `Wire.h`
- `Adafruit BMP085`
- `AHTxx`
- `LoRa.h`
- `SD.h`
- `U8g2`
- Y las indicadas en la carpeta `codigo/`

---

## 📌 Descripción del Proyecto

La estación mide:
- Temperatura y humedad
- Presión atmosférica
- Gases y partículas
- Aceleración y orientación (para CanSat)
- GPS (en versiones avanzadas)

### Versiones:
- **Estación Fija:** Para instalación permanente.
- **CanSat:** Miniatura para lanzamientos simulados.

Ambas versiones transmiten datos por **LoRa/WiFi** y se visualizan con **Node-RED**.

---

## 🧠 Estructura del Repositorio

├── codigo/ # Códigos para Heltec ESP32 Lora V3

├── modelos_3d/ # Archivos STL para impresión 3D

├── node-red/ # Flujos exportados de Node-RED

├── kicad/ # Archivos del diseño PCB en KiCad




### Contenido detallado

#### `codigo/`
- Lectura de sensores
- Comunicación LoRa/WiFi
- Escritura en SD
- Pantalla OLED

#### `modelos_3d/`
- Archivos STL para impresión FDM
- Versiones fija y CanSat

#### `node-red/`
- Dashboards
- Descarga de datos (CSV, JSON)
- Endpoints y APIs

#### `kicad/`
- Archivos `.kicad_pcb`, `.sch`, `.kicad_pro`
- Gerbers, BOM y diagramas

#### `videos/`
- Armado y soldadura
- Carga de código
- Uso de Node-RED
- Diagnóstico

---

## 🧩 Hardware Utilizado

Diseño PCB en **KiCad**, optimizado para **JLCPCB** con diámetro de 10 cm.

### Componentes:
- Heltec WiFi LoRa 32 V3 (ESP32)
- GPS
- MPU6050
- BMP180
- AHT10
- Módulo GSM
- Sensor de partículas
- Conectores de expansión
- Pantalla OLED

---

## 🌐 Plataforma Node-RED

- Visualización en tiempo real
- Descarga de históricos
- Redireccionamiento automático
- Endpoints personalizados

Instalable en Raspberry Pi, PC o nube.

---

## ✅ Requisitos

- Arduino IDE
- Librerías para sensores (ver `codigo/`)
- Node-RED
- KiCad 7.x
- Fuente 5V 1A
- microSD (<8 GB) para CanSat

---

## 📄 Licencia

Este proyecto está bajo la Licencia **MIT**.  
Puedes usarlo, modificarlo y compartirlo libremente con los créditos correspondientes.

---

## 👨‍🏫 Créditos

**Universidad Mesoamericana** – Facultad de Ingeniería  
**Proyecto CanSat Tzunun 2024** – Sede Quetzaltenango  
Docente responsable: *[Nombre del asesor]*  
