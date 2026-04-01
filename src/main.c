#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "definiciones.h"
#include "analizadorsintactico.h"
#include "TS.h"

/*
 *
 * Para simplificar la lectura todas las funciones empiezan por dos letras que describen a que .c pertenecen
 * De este modo:
 *          SE => Sistema de entrada
 *          AL => Analizador lexico
 *          AS => Analizador sintactico
 *          TS => Tabla de simbolos
 *
 *
 * El main es muy simple, se carga el documento indicado por parámetro (o en caso de que no haya parámetros
 * se usa regression.d por defecto). Luego se inicializa el sistema de entrada y la tabla de símbolos
 * y finalmente se imprime primero la TS inicial, luego se hace el "análisis" sintáctico que bien sabemos que en realidad
 * lo único que hace es imprimir los comp. léxicos que va leyendo según los recibe del ana. léxico.
 * y, finalmente, imprime lo que hay en la TS al final, que debería de ser lo mismo que la inicial + todos los
 * identificadores que se van leyendo a lo largo del análisis.
 *
 * Y eso es todo por el main. Continiacuión en el analizador sintáctico.
 *
 */

 extern FILE *yyin;   /* variable global de Flex */


int main(int argc, char *argv[])
{
    //Si no tiene argumentos usa regression.d por defecto, si no: Usa el primer argumento como fichero a cargar
    const char *fichero = (argc > 1) ? argv[1] : "regression.d";

    if (!fichero) {
        printf("No se ha podido cargar ningun fichero");
        exit(EXIT_FAILURE);
    }

    /* - Inciar TS y Sistema de Entrada (SE) - */
    yyin = fopen(fichero, "r");
    if (!yyin) {
        fprintf(stderr, "No se pudo abrir el fichero: %s\n", fichero);
        exit(EXIT_FAILURE);
    }
    TS_init();

    printf("\n\n===================================================");
    printf("\n============== TS ANTES DEL ANALISIS ==============\n");
    TS_imprimir();

    /* ── ANALISIS ── */
    printf("\n\n===================================================");
    printf("  \n================= INICIO ANALISIS =================\n\n");

    AS_imprimir_lexemas();

    printf("\n================== FIN  ANALISIS ==================\n");
    printf("===================================================\n");
    /* - FIN ANALISIS - */

    printf("\n\n===================================================");
    printf("\n============= TS DESPUES DEL ANALISIS =============\n");
    TS_imprimir();

    /* - Limpieza - */
    TS_destruir();
    fclose(yyin);

    return 0;
}
