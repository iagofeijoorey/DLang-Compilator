#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "TS.h"

/* - Nodo de la tabla (lista enlazada por hash) - */
typedef struct Nodo {
    char       lexema[TAM_LEXEMA];
    TipoComponeneteLexico  tipo;
    struct Nodo *sig;
} Nodo;

/* - Hash table simple - */
#define NUM_ENTRADAS_HASH 128

static Nodo *tabla[NUM_ENTRADAS_HASH];
static bool  iniciada = false;

static unsigned int hash(const char *s)
{
    unsigned int h = 0;
    while (*s) h = h * 31 + (unsigned char)*s++;
    return h % NUM_ENTRADAS_HASH;
}

/* - API publica - */

void TS_insertar(const char *lexema, TipoComponeneteLexico tipo)
{
    unsigned int h = hash(lexema);
    for (Nodo *n = tabla[h]; n; n = n->sig)
        if (strcmp(n->lexema, lexema) == 0) return;  /* ya existe */

    Nodo *n = malloc(sizeof *n);
    strncpy(n->lexema, lexema, TAM_LEXEMA - 1);
    n->lexema[TAM_LEXEMA - 1] = '\0';
    n->tipo = tipo;
    n->sig  = tabla[h];
    tabla[h] = n;
}

int TS_buscar(const char *lexema)
{
    unsigned int h = hash(lexema);
    for (Nodo *n = tabla[h]; n; n = n->sig)
        if (strcmp(n->lexema, lexema) == 0) return n->tipo;
    return -1;
}

void TS_init()      //Funcion de precarga de la tabla de simbolos hardcodeando
{
    if (iniciada) return;
    memset(tabla, 0, sizeof tabla);
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

void TS_destruir() //Funcion de limpieza
{
    for (int i = 0; i < NUM_ENTRADAS_HASH; i++) {
        Nodo *n = tabla[i];
        while (n) {
            Nodo *sig = n->sig;
            free(n);
            n = sig;
        }
        tabla[i] = NULL;
    }
    iniciada = false;
}

void TS_imprimir() //funcionnpara imprimr lo que haya en la TS
{
    printf("===================================================\n");
    for (int i = 0; i < NUM_ENTRADAS_HASH; i++)
        for (Nodo *n = tabla[i]; n; n = n->sig)
            printf("  %-24s -> %d\n", n->lexema, n->tipo);
    printf("===================================================\n");
}