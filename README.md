# DCompiler: Compilador del lenguaje D en C
Proyecto desarrollado para la asignatura deCompiladores e intérpretes del grado de Ingeniería Informática en la Universidad de Santiago de Compostela (USC). El proyecto está desarrollado por Iago Feijóo Rey.

## Descripción del Proyecto
Implementación de un compilador para el lenguaje de programación **D**, escrito en C. El compilador está actualmente en fase de **análisis léxico**, siendo capaz de tokenizar código fuente D e identificar todos sus componentes léxicos: palabras reservadas, identificadores, literales (enteros, reales y strings), operadores y símbolos.

El sistema implementa un **doble buffer centinela** para la lectura eficiente del fichero fuente, autómatas específicos para cada tipo de componente léxico, y una tabla de símbolos con hash para la gestión de identificadores y palabras reservadas. Soporta los tres tipos de comentarios del lenguaje D: de línea (`//`), de bloque (`/* */`) y **anidados** (`/+ +/`).

## Contenido del Repositorio
* `src/`:
    - `main.c`: Punto de entrada. Carga el fichero, inicializa los módulos y ejecuta el análisis.
    - `sistemaDeEntrada.c/.h`: Sistema de doble buffer centinela para la lectura del fichero fuente.
    - `analizadorlexico.c/.h`: Autómatas para el reconocimiento de todos los componentes léxicos.
    - `analizadorsintactico.c/.h`: Módulo que consume tokens del analizador léxico e imprime los resultados.
    - `TS.c/.h`: Tabla de símbolos implementada con tabla hash y listas enlazadas.
    - `ABB.c/.h`: Árbol Binario de Búsqueda (estructura auxiliar).
    - `errores.c/.h`: Gestión centralizada de errores.
    - `definiciones.h`: Constantes, códigos de tokens y definición de `ComponeneteLexico`.
* `regression.d`: Fichero de prueba con código D que ejercita los casos límite del analizador.
* `Makefile`: Fichero de compilación.
* `ReadME.txt`: Guía de uso básica.
* `README.md`: Este archivo.

## Arquitectura del Proyecto

```
fichero .d
    │
    ▼
Sistema de Entrada (doble buffer centinela)
    │  SE_sig_caracter() / SE_retroceder()
    ▼
Analizador Léxico (autómatas)
    │  AL_sig_comp_lexico()
    │  ├── automata_saltar()       → blancos y comentarios (//, /* */, /+ +/)
    │  ├── automata_identificador() → IDs y palabras reservadas
    │  ├── automata_numero()       → enteros, reales, hex, binarios
    │  └── automata_string()       → literales string
    │
    ├──► Tabla de Símbolos (hash table)
    │
    ▼
Analizador Sintáctico
    │  AS_imprimir_lexemas()
    ▼
Salida: <tipo, "lexema">
```

## Requisitos del Sistema

* GCC (C99 o superior)
* Sistema Linux/macOS (o cualquier entorno con `make`)

## Uso

```bash
# Compilar
make

# Ejecutar con el fichero de regresión por defecto
./compilador

# Ejecutar con un fichero específico
./compilador ruta/al/fichero.d
```

La salida muestra la Tabla de Símbolos antes y después del análisis, seguida de todos los tokens reconocidos en formato `<codigo, "lexema">`.

## Estado del Proyecto

| Fase                  | Estado        |
|-----------------------|---------------|
| Sistema de Entrada    | ✅ Completado |
| Análisis Léxico       | ✅ Completado |
| Análisis Sintáctico   | 🔄 En progreso |
| Análisis Semántico    | ⏳ Pendiente  |
| Generación de Código  | ⏳ Pendiente  |

## Contacto

Para cualquier duda o sugerencia, puedes ponerte en contacto con el desarrollador:

**Iago Feijóo Rey**: iagofeijoorey@gmail.com
* [LinkedIn](https://www.linkedin.com/in/iagofeijoorey/)

## Languages and Tools used

<p align="left">
<a href="https://dlang.org/" target="_blank" rel="noreferrer">
<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/d/d-original.svg" alt="dlang" width="40" height="40"/></a>
<a href="https://www.gnu.org/software/gcc/" target="_blank" rel="noreferrer">
<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/gcc/gcc-original.svg" alt="gcc" width="40" height="40"/></a>
<a href="https://code.visualstudio.com/" target="_blank" rel="noreferrer">
<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/vscode/vscode-original.svg" alt="vscode" width="40" height="40"/></a>
</p>
