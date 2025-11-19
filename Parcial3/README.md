# 🧠 GSEA — Gestión Segura y Eficiente de Archivos  
**(Generador de Sufijos Encriptados y Archivados)**

Herramienta de línea de comandos para **compresión y encriptación de archivos** con **procesamiento concurrente** mediante hilos POSIX.  
Desarrollado en **C** como parte del curso de **Sistemas Operativos – Parcial 3 (Universidad EAFIT).**

---

## 📘 Descripción General
GSEA es una utilidad de alto rendimiento diseñada para procesar grandes volúmenes de archivos utilizando **llamadas al sistema** directas (System Calls POSIX) y **concurrencia a nivel de hilos**. Permite comprimir y encriptar archivos o directorios completos de manera simultánea.

La herramienta cumple estrictamente con la filosofía de **no utilizar librerías externas** para la lógica central: todos los algoritmos de compresión, encriptación y gestión de hilos han sido implementados desde cero.

---

## ⚙️ Características Implementadas

### 1. Concurrencia y Rendimiento
- **Procesamiento Paralelo:** Utiliza `pthread` para procesar múltiples archivos simultáneamente.
- **Thread Pool:** Implementación de procesamiento por lotes para maximizar el uso de CPU sin saturar los recursos del sistema.
- **System Calls:** Uso exclusivo de `open`, `read`, `write`, `opendir`, `readdir` para máxima eficiencia en I/O, evitando el overhead de `stdio.h`.

### 2. Algoritmos Propios
#### Compresión: Run-Length Encoding (RLE)
- **Justificación:** Se eligió RLE por su extrema eficiencia computacional y bajo consumo de memoria. En un entorno de sistemas operativos donde el cuello de botella suele ser la E/S de disco, RLE permite comprimir flujos de datos repetitivos (logs, imágenes simples, datos de sensores) casi a la velocidad del bus de memoria, maximizando el throughput del sistema concurrente.

#### Encriptación: Cifrado Vigenère
- **Justificación:** Se implementó Vigenère como demostración académica de criptografía simétrica polialfabética. Al operar mediante aritmética modular simple sobre bytes, evita la complejidad computacional de algoritmos de bloque como AES, permitiendo que la herramienta mantenga una latencia ultra baja incluso bajo carga pesada, cumpliendo el requisito de confidencialidad básica.

---

## 🚀 Instrucciones de Uso

### Compilación
El proyecto incluye un `Makefile` completo.
```bash
make
```

### Ejecución
La sintaxis general es:
```bash
./build/gsea [OPERACIONES] [ALGORITMOS] -i [ENTRADA] -o [SALIDA] -k [CLAVE]
```

#### Argumentos Principales
- **Operaciones:**
  - `-c`: Comprimir
  - `-d`: Descomprimir
  - `-e`: Encriptar
  - `-u`: Desencriptar
  - `-ce`: Comprimir y luego Encriptar (Pipeline)
  - `-du`: Desencriptar y luego Descomprimir

- **Algoritmos:**
  - `--comp-alg rle`: Algoritmo de compresión RLE.
  - `--enc-alg vigenere`: Algoritmo de encriptación Vigenère.

- **Archivos:**
  - `-i [path]`: Archivo o directorio de entrada.
  - `-o [path]`: Archivo o directorio de salida.
  - `-k [clave]`: Clave secreta (texto) para encriptación.

### Ejemplos

**1. Comprimir y Encriptar un solo archivo:**
```bash
./build/gsea -ce --comp-alg rle --enc-alg vigenere -i secreto.txt -o secreto.bin -k miclave
```

**2. Procesar todo un directorio concurrentemente:**
```bash
./build/gsea -ce --comp-alg rle --enc-alg vigenere -i logs/ -o logs_seguros/ -k admin123
```

**3. Recuperar (Desencriptar y Descomprimir):**
```bash
./build/gsea -du --comp-alg rle --enc-alg vigenere -i secreto.bin -o recuperado.txt -k miclave
```

---

## 🧩 Estructura del Proyecto
```
src/
├── main.c              # Punto de entrada
├── args_parser.c       # Parsing de argumentos CLI
├── file_manager.c      # I/O con System Calls (open/read/write)
├── concurrency.c       # Lógica de hilos (pthreads)
├── compression/
│   └── rle.c           # Implementación de RLE
├── encryption/
│   └── vigenere.c      # Implementación de Vigenère
└── utils/
    └── error_handler.c # Sistema de logging y errores
```
