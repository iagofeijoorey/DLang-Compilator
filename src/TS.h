#ifndef TS_H
#define TS_H

/* ============================================================
 *  TS.h  -  Tabla de Simbolos
 *
 *  Tabla plana: guarda pares (lexema -> codigo).
 *  - Las palabras reservadas se precargan al inicio.
 *  - Los identificadores se insertan la primera vez que se leen.
 *  - Consultar un lexema devuelve su codigo, o -1 si no existe.
 * ============================================================ */

#include "definiciones.h"

void TS_init    ();               /* Precarga las palabras reservadas   */
void TS_destruir();               /* Libera toda la memoria             */

/* Inserta lexema con su codigo. Si ya existe no hace nada.  */
void TS_insertar(const char *lexema, TipoComponeneteLexico tipo);

/* Devuelve el codigo del lexema, o -1 si no esta en la TS.  */
int  TS_buscar  (const char *lexema);

/* Imprime toda la tabla (para depuracion).                  */
void TS_imprimir();

#endif /* TS_H */