#ifndef ABB_H
#define ABB_H

#include <stdbool.h>

typedef struct NodoABB NodoABB;

typedef struct {
    NodoABB *raiz;
    int      num_nodos;
} ABB;

void  ABB_init    (ABB *a);
bool  ABB_insertar(ABB *a, const char *clave, void *dato);
void *ABB_buscar  (const ABB *a, const char *clave);
bool  ABB_eliminar(ABB *a, const char *clave);
void  ABB_inorden (const ABB *a, void (*fn)(const char *, void *, void *), void *ctx);
void  ABB_destruir(ABB *a);
int   ABB_size    (const ABB *a);

#endif