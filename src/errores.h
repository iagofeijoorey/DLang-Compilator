#ifndef ERRORES_H
#define ERRORES_H

#define ERR_LEXEMA_LARGO   0
#define ERR_FICHERO        1
#define ERR_STRING_ABIERTO 2

void ERR_notificar(int tipo, int linea, const char *descripcion);

#endif /* ERRORES_H */