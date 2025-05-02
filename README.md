# Estación de Calidad del Aire – Proyecto CanSat Tzunun

Este repositorio contiene toda la documentación, archivos, código fuente y guías necesarias para la implementación de una estación de monitoreo ambiental y su versión adaptada al formato CanSat. El proyecto fue desarrollado por estudiantes de la Universidad Mesoamericana, sede Quetzaltenango, como parte de un sistema de medición portátil de calidad del aire.

## 📌 Descripción del Proyecto

La estación de calidad del aire es un dispositivo diseñado para recolectar datos ambientales como temperatura, humedad, presión atmosférica, gases y partículas en el aire. Estos datos son útiles para estudios ambientales, salud pública, y proyectos educativos.

El proyecto se divide en dos versiones:
- **Estación Fija:** Diseñada para ser instalada en puntos estratégicos dentro o fuera de la universidad.
- **Versión CanSat:** Adaptación miniaturizada para misiones de simulación satelital en caída libre.

Ambas versiones integran múltiples sensores y comunicación LoRa/WiFi, y cuentan con visualización en tiempo real a través de una plataforma web basada en Node-RED.

---

## 🎯 Objetivos

1. Documentar el funcionamiento de la estación de calidad del aire para que técnicos y estudiantes puedan operar y mantener el sistema sin dificultades.
2. Brindar recursos técnicos como esquemas eléctricos, modelos 3D, códigos y videos tutoriales para facilitar la réplica del sistema.
3. Ofrecer una solución educativa e investigativa de bajo costo con tecnologías modernas de sensores y transmisión de datos.

---

## 🧠 Contenido del Repositorio

### 1. `codigo/`
Contiene los códigos de ejemplo para ESP32 Heltec Lora 32 v3, incluyendo:
- Lectura de sensores
- Comunicación LoRa y WiFi
- Escritura de datos en tarjeta SD
- Visualización local en pantalla OLED

### 2. `modelos_3d/`
Modelos STL y archivos de diseño 3D para imprimir la carcasa de la estación (versión fija y CanSat). Diseñados para impresión FDM.

### 3. `node-red/`
Flujos exportados desde Node-RED que muestran:
- Visualización de datos en dashboards
- Descarga de archivos en JSON o CSV
- Envío de datos desde el dispositivo
- Configuración de endpoints y APIs

### 4. `videos/`
Guías en video paso a paso:
- Soldadura y armado de la estación
- Carga de código en el dispositivo
- Uso de Node-RED
- Solución de problemas frecuentes

---

## 🛠️ Hardware

La placa principal fue desarrollada en **KiCad**, respetando un diámetro de 10 cm para abaratar costos al ser fabricada en **JLCPCB**. El diseño pasó por varias iteraciones para optimizar espacio, funcionalidad y disposición de componentes.

### Componentes integrados:
- ESP32 Heltec LoRa 32 v3
- GPS
- MPU6050 (acelerómetro + giroscopio)
- BMP180 (presión)
- AHT10 (temperatura y humedad)
- Módulo GSM
- Conectores extra para ADC, I2C y pines digitales
- Conector para sensor de polvo

La placa fue probada y validada con soldadura manual, y se realizaron ensayos funcionales para garantizar la estabilidad de la comunicación y la alimentación de sensores.

---

## 📂 Archivos KiCad

En la carpeta `kicad/` se encuentran todos los archivos relacionados con el diseño de la PCB:
- Archivos del proyecto `.kicad_pro`, `.kicad_pcb`, `.sch`
- Archivos Gerber listos para ser enviados a fabricación
- Lista de materiales (BOM)
- Diagramas de conexión y posicionamiento

---

## 📡 Plataforma Node-RED

La interfaz web desarrollada en Node-RED permite:
- Visualizar los datos recibidos en tiempo real
- Descargar archivos históricos en JSON o CSV
- Redireccionamiento automático al dashboard
- Gestión de endpoints personalizados

Se puede instalar en una Raspberry Pi local, servidor o incluso servicios cloud compatibles.

---

## ✅ Requisitos

- Arduino IDE
- Librerías para los sensores (ver `codigo/`)
- Node-RED
- KiCad 7.x
- Fuente de alimentación 5V 1A
- Tarjeta microSD menor a 8 GB para versión CanSat

---

## 🔧 Resolución de Problemas

Consulta el archivo `manual_tecnico.pdf` incluido en la raíz para conocer los procedimientos ante fallos como:
- Sensores no detectados
- Lecturas incorrectas
- Fallo en la comunicación o envío de datos
- Problemas con la tarjeta SD

---

## 📄 Licencia

Este proyecto se encuentra bajo la Licencia MIT. Puedes utilizar, modificar y compartir libremente dando el crédito correspondiente.

---

## 👨‍🏫 Créditos

Universidad Mesoamericana – Facultad de Ingeniería  
Estudiantes de proyecto CanSat 2024 – Sede Quetzaltenango  
Docente encargado: [Nombre del asesor]

