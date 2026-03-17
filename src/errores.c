#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "errores.h"

/*
 *
 * Errores.c es una clase super simple que se encarga de gestionar los fallo que se detecten
 * Toda clase que detecte un error llama a ERR_notificar() con el tipo de error, la línea +
 * del fuente y una descripción opcional. Esta función se encarga de imprimir el mensaje apropiado
 * y, si el error es fatal, terminar la ejecución con EXIT_FAILURE.
 *
 *  Errores fatales (terminan la ejecución):
 *      ERR_LEXEMA_LARGO    - el lexema supera TAM_LEXEMA
 *      ERR_FICHERO         - fallo al abrir o leer el fichero
 *      ERR_STRING_ABIERTO  - string sin cerrar al final de línea / fichero
 *
 */

void ERR_notificar(int tipo, int linea, const char *descripcion)
{
    switch (tipo) {

        case ERR_LEXEMA_LARGO:
            fprintf(stderr, "ERROR FATAL: lexema demasiado largo");
            if (linea > 0)  fprintf(stderr, " (linea %d)", linea);
            if (descripcion)    fprintf(stderr, " — %s", descripcion);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);

        case ERR_FICHERO:
            fprintf(stderr, "ERROR FATAL: error de fichero");
            if (descripcion)    fprintf(stderr, " — %s", descripcion);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);

        case ERR_STRING_ABIERTO:
            fprintf(stderr, "ERROR: string sin cerrar");
            if (linea > 0)  fprintf(stderr, " (linea %d)", linea);
            if (descripcion)    fprintf(stderr, " — %s", descripcion);
            fprintf(stderr, "\n");
            break;
    }
}