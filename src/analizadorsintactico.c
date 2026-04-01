#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"
#include "analizadorsintactico.h"

/*
 *
 * La función principal de esta clase es AS_imprimir_lexemas(), que se invoca desde el main.
 *
 * Esta clase se encarga de solicitar componentes léxicos al analizador léxico generado por Flex
 * (compilador.l / lex_yy.c) de manera continua hasta encontrar el EOF.
 *
 * Cada llamada a yylex() reconoce el siguiente token de la entrada y actualiza la variable global
 * _ultimo_token, que contiene el tipo y el lexema del componente léxico leído. La función interna
 * sig_comp_lexico() encapsula este proceso y rellena el estado local comp_lex, colocando un token
 * de tipo EOF cuando la entrada se agota.
 *
 * Por cada componente léxico recibido, AS_imprimir_lexemas() lo imprime por pantalla con el formato
 * <tipo, "lexema"> y solicita el siguiente. El bucle termina cuando se detecta EOF.
 *
 * Y eso es todo por el analizador sintáctico. Continuación en el analizador léxico (compilador.l).
 *
 */


extern int yylex(void);
extern ComponeneteLexico _ultimo_token;

static ComponeneteLexico comp_lex;

static void sig_comp_lexico(void)
{
    if (yylex() == 0) {
        ComponeneteLexico eof = { .tipo = EOF, .lexema = "NULL" };
        comp_lex = eof;
        return;
    }
    comp_lex = _ultimo_token;
}

void AS_imprimir_lexemas(void)
{
    sig_comp_lexico();
    while (comp_lex.tipo != EOF) {
        printf("<%i, \"%s\">\n", comp_lex.tipo, comp_lex.lexema);
        sig_comp_lexico();
    }
}
