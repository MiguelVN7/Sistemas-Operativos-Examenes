# Casos de Uso - GSEA

## Tabla de Contenidos

1. [Casos de Uso Principales](#casos-de-uso-principales)
2. [Escenarios de Usuario](#escenarios-de-usuario)
3. [Flujos de Trabajo](#flujos-de-trabajo)
4. [Ejemplos Prácticos](#ejemplos-prácticos)

## Casos de Uso Principales

### CU-01: Comprimir un Archivo

**Actor**: Usuario

**Precondiciones**:
- El archivo de entrada existe
- El usuario tiene permisos de lectura sobre el archivo
- Hay espacio suficiente en disco para el archivo comprimido

**Flujo Principal**:
1. Usuario invoca GSEA con opción de compresión
2. Sistema valida argumentos
3. Sistema lee archivo de entrada
4. Sistema aplica algoritmo de compresión seleccionado
5. Sistema escribe archivo comprimido
6. Sistema reporta estadísticas

**Postcondiciones**:
- Archivo comprimido generado exitosamente
- Archivo original permanece intacto

**Flujos Alternativos**:
- 2a. Argumentos inválidos → Sistema muestra error y ayuda
- 3a. Archivo no encontrado → Sistema muestra error
- 4a. Error de compresión → Sistema muestra error detallado

---

### CU-02: Encriptar un Archivo

**Actor**: Usuario

**Precondiciones**:
- El archivo de entrada existe
- Usuario proporciona clave de encriptación
- Usuario tiene permisos de lectura

**Flujo Principal**:
1. Usuario invoca GSEA con opción de encriptación y clave
2. Sistema valida clave y argumentos
3. Sistema lee archivo de entrada
4. Sistema aplica algoritmo de encriptación seleccionado
5. Sistema escribe archivo encriptado
6. Sistema confirma operación exitosa

**Postcondiciones**:
- Archivo encriptado generado
- Archivo original permanece intacto

**Flujos Alternativos**:
- 2a. Clave inválida → Sistema muestra error
- 2b. Clave muy corta → Sistema solicita clave más fuerte
- 4a. Error de encriptación → Sistema muestra error

---

### CU-03: Comprimir y Encriptar

**Actor**: Usuario

**Precondiciones**:
- El archivo de entrada existe
- Usuario proporciona clave de encriptación
- Algoritmos válidos seleccionados

**Flujo Principal**:
1. Usuario invoca GSEA con opciones de compresión y encriptación
2. Sistema valida todos los argumentos
3. Sistema lee archivo de entrada
4. Sistema comprime datos
5. Sistema encripta datos comprimidos
6. Sistema escribe archivo final
7. Sistema reporta estadísticas

**Postcondiciones**:
- Archivo comprimido y encriptado generado
- Reducción de tamaño lograda
- Datos protegidos con encriptación

---

### CU-04: Descomprimir un Archivo

**Actor**: Usuario

**Precondiciones**:
- Archivo comprimido existe
- Formato de compresión válido
- Espacio suficiente en disco

**Flujo Principal**:
1. Usuario invoca GSEA con opción de descompresión
2. Sistema valida formato del archivo
3. Sistema lee archivo comprimido
4. Sistema descomprime datos
5. Sistema escribe archivo original
6. Sistema confirma operación

**Postcondiciones**:
- Archivo original restaurado
- Integridad de datos verificada

---

### CU-05: Desencriptar un Archivo

**Actor**: Usuario

**Precondiciones**:
- Archivo encriptado existe
- Usuario proporciona clave correcta
- Formato de encriptación válido

**Flujo Principal**:
1. Usuario invoca GSEA con opción de desencriptación y clave
2. Sistema valida clave
3. Sistema lee archivo encriptado
4. Sistema desencripta datos
5. Sistema escribe archivo desencriptado
6. Sistema confirma operación

**Postcondiciones**:
- Archivo desencriptado generado
- Datos accesibles

**Flujos Alternativos**:
- 2a. Clave incorrecta → Sistema muestra error
- 4a. Datos corruptos → Sistema reporta error de integridad

---

### CU-06: Procesamiento Paralelo

**Actor**: Usuario

**Precondiciones**:
- Archivo grande (>1MB)
- Sistema con múltiples núcleos
- Suficiente memoria RAM

**Flujo Principal**:
1. Usuario especifica número de hilos
2. Sistema divide archivo en chunks
3. Sistema crea hilos de trabajo
4. Cada hilo procesa su chunk
5. Sistema sincroniza resultados
6. Sistema combina chunks procesados
7. Sistema escribe resultado final

**Postcondiciones**:
- Procesamiento completado más rápido
- Recursos del sistema utilizados eficientemente

---

## Escenarios de Usuario

### Escenario 1: Backup de Documentos

**Contexto**: Juan necesita hacer backup de sus documentos importantes.

**Objetivo**: Comprimir y encriptar carpeta de documentos.

**Pasos**:
```bash
# Comprimir documentos con RLE
./build/gsea -c rle -o documentos_backup.rle documentos/

# Encriptar el archivo comprimido
./build/gsea -e aes -k "MiClaveSegura123" -o documentos_backup.enc documentos_backup.rle

# O hacerlo todo en un paso
./build/gsea -c rle -e aes -k "MiClaveSegura123" -o documentos_backup.dat documentos/
```

**Resultado**: Documentos protegidos y almacenados de forma eficiente.

---

### Escenario 2: Transferencia Segura

**Contexto**: María necesita enviar archivos confidenciales por internet.

**Objetivo**: Comprimir y encriptar antes de enviar.

**Pasos**:
```bash
# Comprimir con LZW y encriptar con Vigenère
./build/gsea -c lzw -e vigenere -k "SECRETKEY" -o report_secured.dat report.pdf

# Enviar archivo
scp report_secured.dat usuario@servidor:/destino/

# En el destino, desencriptar y descomprimir
./build/gsea -D -d -c lzw -e vigenere -k "SECRETKEY" -o report_original.pdf report_secured.dat
```

**Resultado**: Transferencia segura completada.

---

### Escenario 3: Archivos de Log

**Contexto**: Sistema genera logs con mucha repetición.

**Objetivo**: Comprimir logs antiguos para ahorrar espacio.

**Pasos**:
```bash
# Comprimir con RLE (ideal para logs repetitivos)
./build/gsea -c rle -o app_logs_2024.rle app_logs_2024.txt

# Verificar reducción de tamaño
ls -lh app_logs_2024.txt app_logs_2024.rle
```

**Resultado**: Espacio en disco ahorrado significativamente.

---

### Escenario 4: Procesamiento de Archivos Grandes

**Contexto**: Archivo multimedia de 500MB necesita ser procesado.

**Objetivo**: Utilizar múltiples hilos para acelerar procesamiento.

**Pasos**:
```bash
# Usar 8 hilos para comprimir archivo grande
./build/gsea -c huffman -t 8 -o video_compressed.huff video_original.raw

# Monitorear uso de CPU
htop
```

**Resultado**: Procesamiento completado más rápido usando múltiples núcleos.

---

### Escenario 5: Análisis de Efectividad

**Contexto**: Usuario quiere probar diferentes algoritmos.

**Objetivo**: Comparar ratios de compresión.

**Pasos**:
```bash
# Probar RLE
./build/gsea -c rle -o test.rle test.txt
ls -lh test.txt test.rle

# Probar Huffman
./build/gsea -c huffman -o test.huff test.txt
ls -lh test.txt test.huff

# Probar LZW
./build/gsea -c lzw -o test.lzw test.txt
ls -lh test.txt test.lzw

# Comparar resultados
echo "Original: $(stat -c%s test.txt) bytes"
echo "RLE: $(stat -c%s test.rle) bytes"
echo "Huffman: $(stat -c%s test.huff) bytes"
echo "LZW: $(stat -c%s test.lzw) bytes"
```

**Resultado**: Usuario identifica el mejor algoritmo para su caso de uso.

---

## Flujos de Trabajo

### Workflow 1: Desarrollo y Testing

```
1. Modificar código fuente
2. make clean
3. make debug
4. make test
5. Revisar output y logs
6. Corregir si es necesario
7. make rebuild
```

### Workflow 2: Deployment

```
1. make clean
2. make all
3. make test (verificar)
4. sudo make install
5. gsea --version (verificar instalación)
```

### Workflow 3: Backup Automatizado

```bash
#!/bin/bash
# backup_script.sh

DATE=$(date +%Y%m%d)
SOURCE="/home/user/documents"
DEST="/backups/documents_$DATE.enc"
KEY="MySecureBackupKey2024"

# Backup con compresión y encriptación
gsea -c lzw -e aes -k "$KEY" -o "$DEST" "$SOURCE"

# Verificar éxito
if [ $? -eq 0 ]; then
    echo "Backup successful: $DEST"
    # Opcional: subir a cloud
    # aws s3 cp "$DEST" s3://my-backup-bucket/
else
    echo "Backup failed!"
    exit 1
fi
```

---

## Ejemplos Prácticos

### Ejemplo 1: Cifrado Simple

```bash
# Crear archivo de prueba
echo "Este es un mensaje secreto" > mensaje.txt

# Encriptar con Vigenère
./build/gsea -e vigenere -k "CLAVE" -o mensaje.enc mensaje.txt

# Desencriptar
./build/gsea -D -e vigenere -k "CLAVE" -o mensaje_decrypt.txt mensaje.enc

# Verificar
diff mensaje.txt mensaje_decrypt.txt
```

### Ejemplo 2: Compresión de Imágenes (conceptual)

```bash
# Convertir imagen a formato raw
convert imagen.png imagen.raw

# Comprimir con RLE (asumiendo datos repetitivos)
./build/gsea -c rle -o imagen.rle imagen.raw

# Descomprimir
./build/gsea -d -c rle -o imagen_restored.raw imagen.rle

# Reconvertir a PNG
convert imagen_restored.raw imagen_restored.png
```

### Ejemplo 3: Pipeline de Procesamiento

```bash
# Pipeline completo con verbose output
./build/gsea \
    -c huffman \
    -e aes \
    -k "ComplexKey123!" \
    -t 4 \
    -v \
    -o final_output.dat \
    large_input_file.bin
```

---

## Métricas de Éxito

### Para Compresión

- **Ratio de compresión**: Tamaño_Comprimido / Tamaño_Original
- **Tiempo de procesamiento**: Segundos para completar
- **Uso de memoria**: Memoria pico durante operación
- **Throughput**: MB/s procesados

### Para Encriptación

- **Seguridad**: Fortaleza del algoritmo
- **Tiempo de procesamiento**: Overhead de encriptación
- **Integridad**: Datos recuperables correctamente
- **Compatibilidad**: Funciona con diferentes tipos de archivo

---

**Versión**: 1.0
**Fecha**: Octubre 2025
