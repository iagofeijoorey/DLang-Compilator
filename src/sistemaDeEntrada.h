#ifndef SISTEMA_DE_ENTRADA_H
#define SISTEMA_DE_ENTRADA_H

#include "definiciones.h"

void SE_inicializar(const char *ruta);
void SE_cerrar_fichero();
char SE_sig_caracter();
void SE_retroceder();
void SE_marcar_inicio();

#endif