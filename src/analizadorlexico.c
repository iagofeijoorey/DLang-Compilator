#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "ctype.h"
#include "analizadorlexico.h"
#include "sistemaDeEntrada.h"
#include "errores.h"
#include "TS.h"



/*
 *
 * El analizador léxico es el que se encarga de implementar los autómatas que se van a usar para reconocer
 * los componentes léxicos del código leido. Hay un aútomata general que se encarga de reconocer el primer caracter
 * de cada componente léxico y, a partir de ahí, deriva la función a otro autómata específico en caso de se pueda
 * y para el resto emplea un switch que detecta todas las excepciones.
 *
 * Hay 4 autómatas específicos:
 *        - El de salto, que se encarga de saltarse todos los espacios en blanco y comentarios,
 *          independientemente del tipo de comentario que sea. Este se ejecuta siempre al comienzo de cada
 *          lectura, ya que si no hay comenrarios o espacios no pasa nada, y si los hay se los salta.
 *
 *        - El automata de identificadores, que se encarga de reconcer tanto palabras reservadas como
 *          identificadores, ya que ambos empiezan por una letra o un guión bajo y luego pueden
 *          contener letras, dígitos o guiones bajos.
 *
 *        - El automata de números, que se encarga de reconocer tanto enteros como reales, incluyendo números
 *          hexadecimales y binarios.
 *
 *        - El automata de strings, que se encarga de reconocer cadenas de texto entre comillas dobles.
 *
 *
 */

static int linea   = 1;
static int columna = 1;

static char leer() //Tenemos una funcion para leer porque no solo llama a sig_caracter, sino que tmb actualiza bien la linea
{
    char c = SE_sig_caracter();
    if      (c == '\n') { linea++; columna = 1; }
    else if (c != '\0')   columna++;
    return c;
}

static void devolver() {  //Tenemos una funcion para devolver porque no solo llama a retroceder, sino que tmb actualiza bien la linea
    SE_retroceder();
    if (columna > 1) columna--;
}

/* - Automata: saltar blancos y comentarios (de linea, de blqoue y anidados) - */
static void automata_saltar()
{
    char c;
    empezar_bucle:
    while ((c = leer()) != '\0') {
        if (isspace(c)) continue;
        if (c == '/') {
            char c2 = leer();

            /*
             *
             * Comentario de linea, que son los que siguen con una barra "/" a la barra que ya leimos
             * Estos comentarios se cierran cuando se lee un salto de linea \n (o bien fin de fichero \0)
             *
             */
            if (c2 == '/') {
                while ((c = leer()) != '\0' && c != '\n');
                goto empezar_bucle;
            }

            /*
             *
             * Comentario de bloque, que son los que siguen con una "*" a la barra que ya leimos
             * Estos comentarios se cierran cuando se lee un * / los dos juntos
             *
             */
            if (c2 == '*') {
                while ((c = leer()) != '\0')
                    if (c == '*' && leer() == '/') goto empezar_bucle;
                return;
            }

            /*
             *
             * Comentario anidado, que son los que siguen con un mas "+" a la barra que ya leimos
             * Estos cementarios son especiales ya que aceptan jerarquía de niveles, es decir, al igual que con los
             * paréntesis que para poder dar como terminado el paréntesis deben haberse cerrado tantos como se han
             * abierto, aquí pasa lo mismo.
             *
             */
            if (c2 == '+') {
                int depth = 1;
                while ((c = leer()) != '\0' && depth > 0) {
                    if (c == '/'){
                        char x = leer();
                        if (x == '+') depth++;
                        else devolver();
                    }
                    else if (c == '+') {
                        char x = leer();
                        if (x == '/') depth--;
                        else devolver();
                        devolver(); //devolver para que no se coma un simbolo de más
                    }
                }
                goto empezar_bucle;
            }

            devolver();
            devolver();
            return;
        }
        devolver();
        return;
    }
}

/* ── Automata: identificador o palabra reservada ─────────── */
static ComponeneteLexico automata_identificador(char primer)
{
    ComponeneteLexico t = { ID, {0} };
    char  buf[TAM_LEXEMA]; int len = 0;
    buf[len++] = primer;

    char c;
    while ((c = leer()) != '\0' && (isalnum(c) || c == '_'))
        buf[len++] = c;
    devolver();
    buf[len] = '\0';

    int codigo = TS_buscar(buf);
    if (codigo == -1) {
        TS_insertar(buf, ID);
        codigo = ID;
    }

    t.tipo = codigo;
    strncpy(t.lexema, buf, TAM_LEXEMA - 1);
    return t;
}

/* - Automata: numero entero o real - */
static ComponeneteLexico automata_numero(char primer)
{
    ComponeneteLexico t = { LIT_ENTERO, {0} };
    char  buf[TAM_LEXEMA]; int len = 0;
    bool  es_real = false;
    buf[len++] = primer;

    /* Hex o binario */
    if (primer == '0') {
        char c2 = leer();
        if (c2 == 'x' || c2 == 'X' || c2 == 'b' || c2 == 'B') {
            buf[len++] = c2;
            char c;
            while ((c = leer()) != '\0' && (isxdigit(c) || c == '_'))
                if (c != '_') buf[len++] = c;
            devolver();
            buf[len] = '\0';
            strncpy(t.lexema, buf, TAM_LEXEMA - 1);
            return t;
        }
        devolver();
    }

    char c;
    while ((c = leer()) != '\0' && (isdigit(c) || c == '_'))
        if (c != '_') buf[len++] = c;

    /* Parte decimal */
    if (c == '.') {
        char c2 = leer();
        if (isdigit(c2)) {
            es_real = true; buf[len++] = '.'; buf[len++] = c2;
            while ((c = leer()) != '\0' && (isdigit(c) || c == '_'))
                if (c != '_') buf[len++] = c;
        } else { devolver(); devolver(); goto fin; }
    }

    /* Exponente */
    if (c == 'e' || c == 'E') {
        es_real = true; buf[len++] = c;
        c = leer();
        if (c == '+' || c == '-') { buf[len++] = c; c = leer(); }
        if (!isdigit(c)) {
            ERR_notificar(ERR_LEXEMA_LARGO, linea, NULL);
        }
        buf[len++] = c;
        while ((c = leer()) != '\0' && isdigit(c)) buf[len++] = c;
        devolver(); goto fin;
    }
    devolver();

fin:
    buf[len] = '\0';
    strncpy(t.lexema, buf, TAM_LEXEMA - 1);
    t.tipo = es_real ? LIT_REAL : LIT_ENTERO;
    return t;
}

/* - Automata: string - */
static ComponeneteLexico automata_string()
{
    ComponeneteLexico t = { LIT_STRING, {0} };      //Definimos un comp_lex
    char  buf[TAM_LEXEMA]; int len = 0;
    buf[len++] = '"';

    char c;
    while ((c = leer()) != '\0') {      //Leemos mientras no se acaba el fichero caracte a caracter
        if (c == '"') {                 //Si se cierra el string, terminamos, asi que salimos
            buf[len++] = '"';
            break;
        }
        if (c == '\n') {                //Si hay un salto de linea -> Error porque no se cerró el string
            fprintf(stderr, "ERROR: string sin cerrar en linea %d\n", linea);
            /*exit(1); */
        }
        if (c == '\\') {                //Si se cambia de linea tratamos el caso
            buf[len++] = c;
            buf[len++] = leer();
            continue;
        }
        buf[len++] = c;
        if (len >= TAM_LEXEMA - 2) {
            ERR_notificar(ERR_LEXEMA_LARGO, linea, NULL);
        }
    }
    buf[len] = '\0';
    strncpy(t.lexema, buf, TAM_LEXEMA - 1);
    return t;
}

/* ── Funcion principal: devuelve el siguiente componente léxico ──────── */
ComponeneteLexico AL_sig_comp_lexico()
{
    automata_saltar();          //Primero saltamos los espacios o comentarios, si es que fuera necesario.
    SE_marcar_inicio();         // le pedimos al sistema de entrada que actualice el puntero inicio a la posi de delantero

    ComponeneteLexico comp_lex = { ERROR_LEX, {0} }; //Definimos un comp_lex
    char  c = leer();                                               //Y leemos el primer caracter

    if (c == '\0') {                //Manejo de la excepción de que sea el fin de fichero (EOF = \0)
        comp_lex.tipo = EOF;
        strcpy(comp_lex.lexema, "NULL");
        return comp_lex;
    }

    if (isalpha(c) || c == '_') return automata_identificador(c);       //Si empieza por alfanumerico -> identificador
    if (isdigit(c)) return automata_numero(c);                          // Si empieza por dígito -> numero
    if (c == '"') return automata_string();                             // Si empieza por comillas -> string

    /*
     * Este switch tan largo tiene como propósito detectar que comp. léxico estamos leyendo si, por algun motivo, el caracter que hemos leido
     * no es ni un espacio, ni un comentario, ni un identificador, ni un numero, ni un string, es decir, no ha entrado en ninguno de los ifs de arriba.
     * En ese caso salta al switch donde se comprueba que puede ser.
     *
     * Cabe destacar que creamos la variable c2 para los casos donde el segundo caracter sea significativo, esto es, para los == -= += y todas estas
     * variantes de operadores.
     */
    char c2;
    switch (c) {
        case '+': c2=leer(); //comento el primero, los demás van más o menos igual               Si es un más comprobamos el segundo caracter (porque puede ser doble)
            if      (c2=='+') { comp_lex.tipo=INC;       strcpy(comp_lex.lexema,"++"); }              //Si el sigiente es un mas pues es un "++"
            else if (c2=='=') { comp_lex.tipo=MAS_ASIG;  strcpy(comp_lex.lexema,"+="); }              //Si es un "=" pues es un "+="
            else { devolver(); comp_lex.tipo=MAS;         strcpy(comp_lex.lexema,"+"); } break;       //Si no pues solo un +
        case '*': c2=leer();
             { devolver(); comp_lex.tipo=POR;   strcpy(comp_lex.lexema,"*"); } break;
        case '/': c2=leer();
             { devolver(); comp_lex.tipo=DIV;   strcpy(comp_lex.lexema,"/"); } break;
        case '%': c2=leer();{ devolver(); comp_lex.tipo=MOD;   strcpy(comp_lex.lexema,"%"); } break;
        case '=': c2=leer();
            if      (c2=='=') { comp_lex.tipo=IGUAL;  strcpy(comp_lex.lexema,"=="); }
            else { devolver(); comp_lex.tipo=ASIG;     strcpy(comp_lex.lexema,"="); } break;
        case '<': c2=leer();
            if (c2=='=') { comp_lex.tipo=MENOR_IGUAL; strcpy(comp_lex.lexema,"<="); }
            else { devolver(); comp_lex.tipo=MENOR;    strcpy(comp_lex.lexema,"<"); } break;
        case '-': comp_lex.tipo=MENOS;          strcpy(comp_lex.lexema,"-"); break;
        case '!':  comp_lex.tipo=NOT;           strcpy(comp_lex.lexema,"!"); break;
        case '>': comp_lex.tipo=MAYOR;          strcpy(comp_lex.lexema,">"); break;
        case ';': comp_lex.tipo=PUNTO_COMA;     strcpy(comp_lex.lexema,";");  break;                    //Todos estos no tienen posibilidad de ser dobles
        case ',': comp_lex.tipo=COMA;           strcpy(comp_lex.lexema,",");  break;                    //Así que directamente asignamos el tipo y el lexema
        case '.': comp_lex.tipo=PUNTO;          strcpy(comp_lex.lexema,".");  break;
        case ':': comp_lex.tipo=DOS_PUNTOS;     strcpy(comp_lex.lexema,":");  break;
        case '?': comp_lex.tipo=INTERROGA;      strcpy(comp_lex.lexema,"?");  break;
        case '(': comp_lex.tipo=ABRE_PAR;       strcpy(comp_lex.lexema,"(");  break;
        case ')': comp_lex.tipo=CIERRA_PAR;     strcpy(comp_lex.lexema,")");  break;
        case '{': comp_lex.tipo=ABRE_LLAVE;     strcpy(comp_lex.lexema,"{");  break;
        case '}': comp_lex.tipo=CIERRA_LLAVE;   strcpy(comp_lex.lexema,"}");  break;
        case '[': comp_lex.tipo=ABRE_CORCH;     strcpy(comp_lex.lexema,"[");  break;
        case ']': comp_lex.tipo=CIERRA_CORCH;   strcpy(comp_lex.lexema,"]");  break;
        case '~': comp_lex.tipo=TILDE;          strcpy(comp_lex.lexema,"~");  break;

        default:
            comp_lex.lexema[0] = c; comp_lex.lexema[1] = '\0';
            fprintf(stderr, "ERROR: caracter inesperado '%c' en linea %d\n", c, linea);
            exit(1);
    }
    return comp_lex;
}