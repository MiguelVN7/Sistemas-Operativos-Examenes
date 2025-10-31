# Documento Técnico - GSEA

## Tabla de Contenidos

1. [Introducción](#introducción)
2. [Arquitectura del Sistema](#arquitectura-del-sistema)
3. [Módulos del Sistema](#módulos-del-sistema)
4. [Algoritmos Implementados](#algoritmos-implementados)
5. [Gestión de Concurrencia](#gestión-de-concurrencia)
6. [Gestión de Recursos](#gestión-de-recursos)
7. [Manejo de Errores](#manejo-de-errores)
8. [Pruebas y Validación](#pruebas-y-validación)

## Introducción

GSEA es un sistema de compresión y encriptación de archivos diseñado con una arquitectura modular y escalable.

## Arquitectura del Sistema

### Diseño Modular

El sistema está organizado en los siguientes módulos:

1. **Core Modules**: Funcionalidad básica del sistema
   - args_parser: Procesamiento de argumentos
   - file_manager: Gestión de archivos
   - concurrency: Manejo de hilos

2. **Compression Modules**: Algoritmos de compresión
   - huffman: Codificación Huffman
   - rle: Run-Length Encoding
   - lzw: Lempel-Ziv-Welch

3. **Encryption Modules**: Algoritmos de encriptación
   - vigenere: Cifrado Vigenère
   - des: Data Encryption Standard
   - aes_simple: Advanced Encryption Standard

4. **Utilities**: Funciones de soporte
   - error_handler: Sistema de errores centralizado
   - memory_manager: Gestión de memoria

### Flujo de Datos

```
[Usuario] → [args_parser] → [file_manager]
                                   ↓
                            [concurrency]
                                   ↓
                           [compression] (opcional)
                                   ↓
                           [encryption] (opcional)
                                   ↓
                           [file_manager] → [Salida]
```

## Módulos del Sistema

### args_parser

**Responsabilidad**: Validar y procesar argumentos de línea de comandos.

**Funciones principales**:
- Parseo de opciones
- Validación de parámetros
- Configuración del sistema

### file_manager

**Responsabilidad**: Operaciones de I/O de archivos.

**Funciones principales**:
- Lectura de archivos
- Escritura de archivos
- Validación de permisos
- Manejo de errores de I/O

### concurrency

**Responsabilidad**: Gestión de hilos y sincronización.

**Funciones principales**:
- Creación de hilos
- Sincronización con mutex
- División de trabajo
- Agregación de resultados

## Algoritmos Implementados

### Compresión

#### 1. Run-Length Encoding (RLE)

**Estado**: ✅ Completamente implementado

**Descripción**: Comprime secuencias de datos repetidos.

**Formato de salida**: [byte][count][byte][count]...

**Casos de uso ideal**:
- Datos con alta repetición
- Imágenes monocromáticas
- Archivos de log con patrones

#### 2. Huffman Coding

**Estado**: 🚧 En desarrollo

**Descripción**: Compresión basada en frecuencia de símbolos.

**Características**:
- Códigos de longitud variable
- Compresión sin pérdida
- Óptimo para distribuciones no uniformes

#### 3. Lempel-Ziv-Welch (LZW)

**Estado**: 🚧 En desarrollo

**Descripción**: Compresión basada en diccionario.

**Características**:
- Construcción dinámica de diccionario
- No requiere tabla de símbolos previa
- Eficiente para texto y datos estructurados

### Encriptación

#### 1. Cifrado Vigenère

**Estado**: ✅ Completamente implementado

**Descripción**: Cifrado polialfabético clásico.

**Características**:
- Clave repetitiva
- Resistente a análisis de frecuencia simple
- Procesa solo caracteres alfabéticos

**Ejemplo**:
```
Texto: ATTACKATDAWN
Clave: LEMON
Cifrado: LXFOPVEFRNHR
```

#### 2. DES (Data Encryption Standard)

**Estado**: 🚧 En desarrollo

**Descripción**: Cifrado de bloques de 64 bits.

**Características**:
- Bloques de 64 bits
- Clave de 56 bits
- 16 rondas Feistel

#### 3. AES (Advanced Encryption Standard)

**Estado**: 🚧 En desarrollo

**Descripción**: Cifrado de bloques moderno.

**Características**:
- Bloques de 128 bits
- Clave de 128/192/256 bits
- SubBytes, ShiftRows, MixColumns, AddRoundKey

## Gestión de Concurrencia

### Estrategia de Hilos

1. **División de archivos**: Archivos grandes se dividen en chunks
2. **Procesamiento paralelo**: Cada chunk se procesa en un hilo
3. **Sincronización**: Uso de mutex para recursos compartidos
4. **Agregación**: Los resultados se combinan en orden

### Sincronización

```c
pthread_mutex_t mutex;
pthread_mutex_lock(&mutex);
// Critical section
pthread_mutex_unlock(&mutex);
```

## Gestión de Recursos

### Memoria

- Sistema de tracking en modo DEBUG
- Detección automática de fugas
- Liberación ordenada de recursos

### Archivos

- Apertura/cierre automático
- Validación de permisos
- Manejo de errores de I/O

## Manejo de Errores

### Niveles de Severidad

1. **INFO**: Información general
2. **WARNING**: Advertencias no críticas
3. **ERROR**: Errores recuperables
4. **CRITICAL**: Errores fatales

### Logging

Cada error incluye:
- Timestamp
- Severidad
- Archivo y línea
- Código de error
- Mensaje descriptivo

## Pruebas y Validación

### Suite de Pruebas

1. **test_compression.c**: Pruebas de compresión
   - RLE básico
   - RLE sin repetición
   - Huffman placeholder
   - LZW placeholder

2. **test_encryption.c**: Pruebas de encriptación
   - Vigenère básico
   - Vigenère con mayúsculas/minúsculas
   - DES placeholder
   - AES placeholder

3. **test_file_manager.c**: Pruebas de utilidades
   - Error handler
   - Memory manager
   - Códigos de error

### Ejecución de Pruebas

```bash
make test
```

### Validación de Memoria

```bash
make DEBUG=1
./build/test_compression
# Verificar reporte de fugas al final
```

## Conclusiones

GSEA proporciona una arquitectura sólida y modular para compresión y encriptación de archivos, con soporte para procesamiento concurrente y gestión robusta de recursos.

---

**Versión**: 1.0
**Fecha**: Octubre 2025
