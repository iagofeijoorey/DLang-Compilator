#ifndef DEFINICIONES_H
#define DEFINICIONES_H


/* - Tamaños - */
#define TAM_BUFFER    192   //El bloque de texto mas largo es el comentario anidado del main. Son 151 bytes
                            //Por lo que escogí la suma de multiplos de 2 que mas se acercaba pasando por encima
                            //128 + 64 = 192 (Podría haber escogido 160 pero preferí quedarme un poco más por si acaso)
#define TAM_LEXEMA    191

/* - Comp. lexicos de 1 caracter: su propio codigo ASCII - */
#define MAS             '+'
#define MENOS           '-'
#define POR             '*'
#define DIV             '/'
#define MOD             '%'
#define ASIG            '='
#define MENOR           '<'
#define MAYOR           '>'
#define NOT             '!'
#define PUNTO_COMA      ';'
#define COMA            ','
#define PUNTO           '.'
#define DOS_PUNTOS      ':'
#define INTERROGA       '?'
#define ABRE_PAR        '('
#define CIERRA_PAR      ')'
#define ABRE_LLAVE      '{'
#define CIERRA_LLAVE    '}'
#define ABRE_CORCH      '['
#define CIERRA_CORCH    ']'
#define TILDE           '~'

/* - Comp. lexicos de palabras reservadas */
#define ID              300
#define IMPORT          310
#define VOID            311
#define INT             312
#define DOUBLE          313
#define WHILE           314
#define FOREACH         315
#define RETURN          316
#define CAST            317
#define ENFORCE         318

/* - Literales - */
#define LIT_ENTERO      320
#define LIT_REAL        321
#define LIT_STRING      322

/* - Operadores compuestos - */
#define MAS_ASIG        330
#define IGUAL           331
#define MENOR_IGUAL     332
#define INC             333

/* - ERROR - */
#define ERROR_LEX       999




/*
 *
 * Este struct es el que define como es un componente lexico
 * Consta de un campo tipo, que es basicamente el int que identifica de que tipo de comp. lexico se trata y luego
 * un campo de lexema que es un string (char[]) que contiene el texto leido del comp, lexico
 *
 */
typedef int TipoComponeneteLexico;

typedef struct {
    TipoComponeneteLexico tipo;
    char      lexema[TAM_LEXEMA];
} ComponeneteLexico;



#endif /* DEFINICIONES_H */