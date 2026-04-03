# DCompiler: Compilador del lenguaje D en C
Proyecto desarrollado para la asignatura de Compiladores e intérpretes del grado de Ingeniería Informática en la Universidad de Santiago de Compostela (USC). El proyecto está desarrollado por Iago Feijóo Rey.

## Descripción del Proyecto
Implementación de un compilador para el lenguaje de programación **D**, escrito en C. El compilador está actualmente en fase de **análisis léxico**, siendo capaz de tokenizar código fuente D e identificar todos sus componentes léxicos: palabras reservadas, identificadores, literales (enteros, reales y strings), operadores y símbolos.

El analizador léxico está implementado con **Flex**, que se encarga tanto de la lectura del fichero fuente como del reconocimiento de tokens mediante expresiones regulares. Soporta los tres tipos de comentarios del lenguaje D: de línea (`//`), de bloque (`/* */`) y **anidados** (`/+ +/`). La tabla de símbolos usa un ABB para la gestión de identificadores y palabras reservadas.

## Contenido del Repositorio
* `src/`:
    - `main.c`: Punto de entrada. Carga el fichero, inicializa los módulos y ejecuta el análisis.
    - `compilador.l`: Definición del analizador léxico en Flex. Contiene los patrones de tokens y las acciones asociadas.
    - `lex_yy.c`: Código C generado automáticamente por Flex a partir de `compilador.l`. No debe editarse a mano.
    - `analizadorsintactico.c/.h`: Módulo que consume tokens del analizador léxico e imprime los resultados.
    - `TS.c/.h`: Tabla de símbolos implementada con un ABB.
    - `ABB.c/.h`: Árbol Binario de Búsqueda usado internamente por la TS.
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
Flex (compilador.l / lex_yy.c)
    │  yylex()  ←  gestiona el buffer de entrada internamente
    │  ├── blancos y comentarios (//, /* */, /+ +/)  → se ignoran
    │  ├── identificadores / palabras reservadas      → consulta/inserta en TS
    │  ├── literales enteros (decimal, hex, binario)  → LIT_ENTERO
    │  ├── literales reales (punto y/o exponente)     → LIT_REAL
    │  └── literales string                           → LIT_STRING
    │
    ├──► Tabla de Símbolos (ABB)
    │
    ▼
Analizador Sintáctico
    │  AS_imprimir_lexemas()
    ▼
Salida: <tipo, "lexema">
```

## Requisitos del Sistema

* GCC (C99 o superior)
* Flex
* Sistema Linux/macOS (o cualquier entorno con `make`)

## Uso

```bash
# Generar el analizador léxico con Flex
flex -o lex_yy.c compilador.l

# Compilar
gcc -o compilador main.c analizadorsintactico.c lex_yy.c TS.c ABB.c errores.c

# O usando el Makefile
make

# Ejecutar con el fichero de regresión por defecto
./compilador

# Ejecutar con un fichero específico
./compilador ruta/al/fichero.d
```

La salida muestra la Tabla de Símbolos antes y después del análisis, seguida de todos los tokens reconocidos en formato `<codigo, "lexema">`.


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
<a href="https://github.com/westes/flex" target="_blank" rel="noreferrer">
<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/flex/flex-original.svg" alt="flex" width="40" height="40"/></a>
<a href="https://code.visualstudio.com/" target="_blank" rel="noreferrer">
<img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/vscode/vscode-original.svg" alt="vscode" width="40" height="40"/></a>
</p>
