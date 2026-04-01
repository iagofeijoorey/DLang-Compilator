#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "TS.h"
#include "ABB.h"

static ABB tabla;
static bool iniciada = false;

/*
 * Antes, en mi versión previa, usé una tabla hash para la TS, pero decidí cambiar a un ABB que tenía en la entrega pero que no usaba
 * Para la anterior entrega había implementado una tabla hash y también habia cogido de una asignatura anterior un ABB, pero usaba la primera
 * Sin embargo, para esta versión he quitado la tabla hash y usado solo la ABB porque no tenía ningun sentido tener ambas cosas ahi sin usar una de ellas
 *
 * TS.c implementa la Tabla de Símbolos del compilador. Internamente usa un ABB donde cada nodo almacena un par (lexema -> código de tipo). 
 *
 * Al inicializarse con TS_init() se precargan todas las palabras reservadas del lenguaje. A partir de ahí, cada vez que el analizador léxico lee un identificador
 * llama a TS_buscar() para saber si es una palabra reservada o un ID ya visto, y llama a TS_insertar() si es un ID nuevo.
 *
 * TS_imprimir() recorre el árbol en inorden (orden alfabético) e imprime cada entrada, lo que permite ver el estado de la tabla antes y después del análisis.
 *
 * TS_destruir() libera toda la memoria dinámica asociada
 *
 */

void TS_init()
{
    if (iniciada) return;
    ABB_init(&tabla);
    iniciada = true;

    /* Palabras reservadas precargadas */
    TS_insertar("import",  IMPORT);
    TS_insertar("void",    VOID);
    TS_insertar("int",     INT);
    TS_insertar("double",  DOUBLE);
    TS_insertar("while",   WHILE);
    TS_insertar("foreach", FOREACH);
    TS_insertar("return",  RETURN);
    TS_insertar("cast",    CAST);
    TS_insertar("enforce", ENFORCE);
}

void TS_insertar(const char *lexema, TipoComponeneteLexico tipo)
{
    /* Si ya existe no hace nada (mismo comportamiento que antes) */
    if (ABB_buscar(&tabla, lexema) != NULL) return;

    TipoComponeneteLexico *dato = malloc(sizeof *dato);
    *dato = tipo;
    ABB_insertar(&tabla, lexema, dato);
}

int TS_buscar(const char *lexema)
{
    TipoComponeneteLexico *dato = ABB_buscar(&tabla, lexema);
    return dato ? *dato : -1;
}


static void imprimir_nodo(const char *clave, void *dato, void *ctx)
{
    (void)ctx;
    printf("  %-24s -> %d\n", clave, *(TipoComponeneteLexico *)dato);
}

void TS_imprimir()
{
    printf("===================================================\n");
    ABB_inorden(&tabla, imprimir_nodo, NULL);
    printf("===================================================\n");
}


static void liberar_dato(const char *clave, void *dato, void *ctx)
{
    (void)clave; (void)ctx;
    free(dato);
}

void TS_destruir()
{
    ABB_inorden(&tabla, liberar_dato, NULL);
    ABB_destruir(&tabla);
    iniciada = false;
}