#include <stdio.h>
#include <stdlib.h>
#include "analizadorsintactico.h"
#include "analizadorlexico.h"

/*
 *
 * En el main la función principal es la de "AS_imprimir_lexemas" que se encuentra aquí
 *
 * La función de esta clase es ir pidiendo al analizador léxico el siguiente componente léxico de manera constante
 * mientras no se encuentra el EOF (End of File). Como se puede observar solo tiene una función para inicializar
 * los valores de comp_lex_actual/siguiente (para evitar fallos de memoria) y la función principal cuyo único objetivo
 * es el nombrado anteriormente. Como es de esperar, además de pedir el sigueinte comp. léxico el código también
 * imprime lo que ha leido por pantalla con el formato <tipo, "lexema">.
 *
 * Y eso es todo por el analizador sintáctico. Continuación en el analizador léxico.
 *
 */

static ComponeneteLexico  comp_lex_actual;
static ComponeneteLexico  comp_lex_siguiente;


void AS_inicializar()
{
    comp_lex_actual    = AL_sig_comp_lexico();
    comp_lex_siguiente = AL_sig_comp_lexico();
}

void AS_imprimir_lexemas() {
    //Esta función simplemente mientras no sea el final de fichero imprime el componente lexico que ha leido y luego salta al siguiente
    while (comp_lex_actual.tipo != EOF) {
        printf("<%i, \"%s\"> \n", comp_lex_actual.tipo, comp_lex_actual.lexema);

        comp_lex_actual    = comp_lex_siguiente;
        comp_lex_siguiente = AL_sig_comp_lexico();
    }
}


