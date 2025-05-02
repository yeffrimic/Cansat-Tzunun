
# 🌍 Estación de Calidad de Aire & CanSat Universitario tzunun

Este repositorio contiene toda la documentación técnica, modelos 3D, código fuente y recursos necesarios para la construcción, programación y operación de la **Estación de Monitoreo de Calidad de Aire** y su versión portátil tipo **CanSat**, desarrollados por los estudiantes Karen Yax, Nesho Pablo, Daniel Sanchez, Yeffri Salazar de la **Universidad Mesoamericana**.

---

## 📦 Contenido del Proyecto

### 🧪 Estación de Calidad de Aire
Dispositivo fijo diseñado para el monitoreo ambiental. Registra datos en tiempo real de:
- Temperatura y humedad (AHT10)
- Presión atmosférica (BMP180)
- Calidad del aire (sensor de polvo)
- Movimiento y orientación (MPU6050)
- Ubicación GPS
- Y más sensores digitales/analógicos conectables

Los datos se envían automáticamente a una **plataforma visual** en **Node-RED**, donde se pueden visualizar, consultar y descargar.

---

### 🚀 Versión CanSat
Versión compacta, autónoma y portátil de la estación, ideal para misiones experimentales de recolección de datos en movimiento. Incluye:
- Almacenamiento en tarjeta microSD
- Registro de telemetría con GPS, sensores ambientales y acelerómetro
- Posibilidad de transmisión inalámbrica mediante WiFi o GSM o LoRa

---

### 🧰 Recursos Incluidos

- 📁 **Modelos 3D**: Archivos `.STL` y `.3MF` para impresión en 3D de la carcasa del CanSat y soportes de sensores.
- 💾 **Códigos de ejemplo**: Programas en Arduino listos para cargar en las placas Heltec LoRa 32 V3.
- 🔧 **Guías técnicas**:
  - Resolución de problemas
  - Manual de ensamblaje y prueba
- 📹 **Videos tutoriales**: Guías paso a paso para el uso del hardware, la programación y la plataforma web.
- 🌐 **Plataforma en Node-RED**: Exportación del flujo completo del servidor local con interfaz de usuario para visualización, configuración y descarga de datos.

---

## 📚 Documentación

Toda la información técnica está organizada en carpetas:

\`\`\`
📁 /docs
   ├── Manual Tecnico.pdf
   ├── Instructivo de Ensamblaje.pdf
   ├── Solución de Problemas.pdf

📁 /code
   ├── estacion_fija/
   ├── version_cansat/
   └── pruebas_individuales/

📁 /models_3d
   ├── carcasa_cansat.stl
   ├── soporte_gps.stl
   └── soporte_bateria.3mf

📁 /node-red
   ├── flow_estacion.json
   └── README_node-red.md
\`\`\`

---

## ▶️ Requisitos

- Arduino IDE (con soporte para placas Heltec LoRa 32 V3)
- Node-RED (preferiblemente en una Raspberry Pi o servidor local)
- Impresora 3D para carcasas
- Sensores compatibles (ver lista en el manual técnico)

---

## 💡 Créditos

Proyecto desarrollado por estudiantes de Ingeniería Electrónica en la **Universidad Mesoamericana - Sede Quetzaltenango** bajo asesoría docente, como parte de un proyecto de vinculación científica y tecnológica.

---

¿Quieres contribuir o replicar el proyecto en tu comunidad? ¡Contáctanos o realiza un fork del repositorio!
