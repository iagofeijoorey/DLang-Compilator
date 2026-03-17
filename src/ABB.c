#define _POSIX_C_SOURCE 200809L
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "ABB.h"

struct NodoABB {
    char    *clave;
    void    *dato;
    NodoABB *izq, *der;
};

static NodoABB *nuevo_nodo(const char *clave, void *dato)
{
    NodoABB *n = malloc(sizeof *n);
    n->clave = strdup(clave);
    n->dato  = dato;
    n->izq   = n->der = NULL;
    return n;
}

static NodoABB *insertar_r(NodoABB *n, const char *clave, void *dato, bool *ok)
{
    if (!n) {
        *ok = true;
        return nuevo_nodo(clave, dato);
    }
    int cmp = strcmp(clave, n->clave);
    if (cmp < 0) {
        n->izq = insertar_r(n->izq, clave, dato, ok);
    }
    else if (cmp > 0) {
        n->der = insertar_r(n->der, clave, dato, ok);
    }
    return n;
}

static NodoABB *min_nodo(NodoABB *n) { while (n->izq) n = n->izq; return n; }

static NodoABB *eliminar_r(NodoABB *n, const char *clave, bool *ok)
{
    if (!n) return NULL;
    int cmp = strcmp(clave, n->clave);
    if      (cmp < 0) n->izq = eliminar_r(n->izq, clave, ok);
    else if (cmp > 0) n->der = eliminar_r(n->der, clave, ok);
    else {
        *ok = true;
        if (!n->izq) { NodoABB *r = n->der; free(n->clave); free(n); return r; }
        if (!n->der) { NodoABB *r = n->izq; free(n->clave); free(n); return r; }
        NodoABB *s = min_nodo(n->der);
        free(n->clave); n->clave = strdup(s->clave); n->dato = s->dato;
        bool aux = false;
        n->der = eliminar_r(n->der, s->clave, &aux);
    }
    return n;
}

static void inorden_r(const NodoABB *n, void (*fn)(const char *, void *, void *), void *ctx)
{
    if (!n) return;
    inorden_r(n->izq, fn, ctx);
    fn(n->clave, n->dato, ctx);
    inorden_r(n->der, fn, ctx);
}

static void destruir_r(NodoABB *n)
{
    if (!n) return;
    destruir_r(n->izq);
    destruir_r(n->der);
    free(n->clave);
    free(n);
}

void  ABB_init    (ABB *a)                          { a->raiz = NULL; a->num_nodos = 0; }
int   ABB_size    (const ABB *a)                    { return a->num_nodos; }
void  ABB_inorden (const ABB *a, void (*fn)(const char *, void *, void *), void *ctx) { inorden_r(a->raiz, fn, ctx); }
void  ABB_destruir(ABB *a)                          { destruir_r(a->raiz); ABB_init(a); }

bool ABB_insertar(ABB *a, const char *clave, void *dato)
{
    bool ok = false;
    a->raiz = insertar_r(a->raiz, clave, dato, &ok);
    if (ok) a->num_nodos++;
    return ok;
}

void *ABB_buscar(const ABB *a, const char *clave)
{
    NodoABB *n = a->raiz;
    while (n) {
        int cmp = strcmp(clave, n->clave);
        if      (cmp < 0) n = n->izq;
        else if (cmp > 0) n = n->der;
        else              return n->dato;
    }
    return NULL;
}

bool ABB_eliminar(ABB *a, const char *clave)
{
    bool ok = false;
    a->raiz = eliminar_r(a->raiz, clave, &ok);
    if (ok) a->num_nodos--;
    return ok;
}