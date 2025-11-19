# Guía de Pruebas - GSEA (Gestión Segura y Eficiente de Archivos)

Este documento describe paso a paso cómo compilar y probar la herramienta GSEA para verificar su funcionalidad completa (compresión, encriptación, concurrencia y recuperación de datos).

## 1. Compilación
Antes de ejecutar, asegúrate de tener el entorno limpio y compilar el proyecto.

```bash
cd Parcial3
make clean
make
```
El ejecutable se generará en `build/gsea` (o `build/gsea.exe`).

---

## 2. Preparación del Entorno de Pruebas
Crea las carpetas y archivos necesarios para simular un escenario real.

```bash
# Crear carpetas de trabajo
mkdir entrada
mkdir salida
mkdir recuperados

# Crear archivos de prueba con diferentes tipos de contenido
echo "Mensaje secreto y confidencial del proyecto SO" > entrada/mensaje.txt
echo "AAAAAAAAAABBBBBBBBBBCCCCCCCCCC" > entrada/repetitivo.txt
```

---

## 3. Prueba 1: Archivo Único (Encriptar y Comprimir)
Verificar que el programa procesa un solo archivo correctamente.

### Ejecución
```bash
./build/gsea -ce --comp-alg rle --enc-alg vigenere -i "./entrada/mensaje.txt" -o "./salida/mensaje.enc" -k "CLAVE" -v
```
* `-ce`: Comprimir + Encriptar
* `-i`: Archivo de entrada
* `-o`: Archivo de salida (con extensión personalizada si se desea)
* `-k`: Clave "CLAVE"

### Verificación
Verifica que el archivo de salida existe y NO es legible:
```bash
cat salida/mensaje.enc
# Debería mostrar caracteres extraños (encriptado)
```

---

## 4. Prueba 2: Directorio Completo con Concurrencia
Verificar que el programa procesa múltiples archivos en paralelo usando hilos.

### Preparación (Generar carga)
Generar 20 archivos automáticamente:
```bash
for i in {1..20}; do echo "Contenido del archivo $i" > entrada/file_$i.txt; done
```

### Ejecución
```bash
./build/gsea -ce --comp-alg rle --enc-alg vigenere -i "./entrada" -o "./salida" -k "CLAVE" -v
```

### Verificación
Observa la salida en la terminal. Deberías ver mensajes como `[Thread 0]`, `[Thread 1]`, etc., indicando que diferentes hilos están trabajando simultáneamente.
Revisa la carpeta `salida` para confirmar que todos los archivos (`file_1.txt`, etc.) fueron creados.

---

## 5. Prueba 3: Recuperación de Datos (Desencriptar y Descomprimir)
La prueba definitiva: tomar los archivos encriptados y volverlos a su estado original.

### Ejecución
```bash
./build/gsea -ud --comp-alg rle --enc-alg vigenere -i "./salida" -o "./recuperados" -k "CLAVE" -v
```
* `-ud`: Desencriptar + Descomprimir (Orden inverso lógico)
* `-i`: Carpeta con archivos encriptados
* `-o`: Carpeta donde guardar los archivos limpios
* `-k`: **DEBE** ser la misma clave ("CLAVE")

### Verificación de Integridad
Compara un archivo original con el recuperado. Deben ser idénticos.

```bash
# Original
cat entrada/mensaje.txt

# Recuperado
cat recuperados/mensaje.txt
```

Si el texto es legible y coincide, la prueba es **EXITOSA**.

---

## Resumen de Comandos

| Acción | Comando |
|--------|---------|
| **Compilar** | `make` |
| **Procesar (Backup)** | `./build/gsea -ce --comp-alg rle --enc-alg vigenere -i IN -o OUT -k KEY` |
| **Recuperar (Restore)** | `./build/gsea -ud --comp-alg rle --enc-alg vigenere -i IN -o OUT -k KEY` |

