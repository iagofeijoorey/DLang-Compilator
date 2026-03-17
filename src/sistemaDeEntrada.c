#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "errores.h"
#include "sistemaDeEntrada.h"

//El bloque A del doble centinela va desde la posición 0 a la posición TAM_BUFFER-1
#define BASE_A    0
#define CENT_A    (TAM_BUFFER)

//El bloque B del doble centinela va desde la posición TAM_BUFFER+1 a la posición 2*TAM_BUFFER
#define BASE_B    (TAM_BUFFER + 1)
#define CENT_B    (2 * TAM_BUFFER + 1)
// El hecho de que cada bloque sea de un tamaño -1 respecto a TAM_BUFFER es porque el último caracter
// de cada bloque se reserva para colocar el centinela, que es un '\0' que indica el fin del bloque

//Por lo tanto, el tamaño total del buffer del doble centinela es el doble del tamaño del bloque + 2 posiciones
//El motivo de las 2 posiciones es para colocar los centinelas al final de cada bloque, que son
//los '\0' que indican el fin del bloque
#define TAM_TOTAL (2 * TAM_BUFFER + 2)

static FILE *fichero     = NULL;        //fichero

static char  buf[TAM_TOTAL];            //Doble centinela
/*
 *
 * En mi caso he decidido usar un solo buffer para la entrada y marcar el final de cada centinela con un \0 (EOF)
 * Creo que es una implementación algo mas sencilla que mantener dos bufferes
 *
 */

//Control de posiciones en el buffer del doble centinela
static int   inicio      = 0;
static int   delantero   = 0;

static bool  fin_fichero = false;       //Boolean para el control de fin de fichero

static void recargar_centinela(int bloque)
{
    int base = bloque ? BASE_B : BASE_A; //
    int cent = bloque ? CENT_B : CENT_A; //

    if (fin_fichero) { //Si es el fin del fichero, entonces rellena el resto del buffer con EOF
        memset(buf + base, '\0', TAM_BUFFER);
    }
    else { //en caso de que no sea el fin del fichero, entonces lee el bloque del fichero y coloca el centinela al final
        size_t n = fread(buf + base, 1, TAM_BUFFER, fichero);
        if (ferror(fichero)) {
            ERR_notificar(ERR_FICHERO, -1, NULL); //Contempla el error de fichero
        }
        if (n < (size_t)TAM_BUFFER) { //Si se ha leído menos de un bloque completo, entonces es el fin del fichero,
                                      //por lo que coloca el centinela al final del bloque y rellena el resto del bloque con EOF
            memset(buf + base + n, '\0', TAM_BUFFER - n);
            fin_fichero = true;
        }
    }
    buf[cent] = '\0';
}

void SE_inicializar(const char *ruta)   //Funcion que abre el fichero (una de las dos unicas que se usan en main) y que carga el centinela A
{                                       //Tmb contempla errores e inicia las variables de control (inicio y delantero)
    fichero = fopen(ruta, "rb");
    if (!fichero) {
        ERR_notificar(ERR_FICHERO, -1, ruta);
    }
    fin_fichero = false;
    inicio = delantero = 0;
    recargar_centinela(0);
}

void SE_cerrar_fichero()            //Funcion de limpieza
{
    if (fichero)
    {
        fclose(fichero);
        fichero = NULL;
    }
}

char SE_sig_caracter()          //La funcion mas importante junto con recargar_centinela. Lee el siguiente caracter del buffer y lo devuelve.
{                               //Tmb contempla el control de fin de cada centinela. Es decir: si el delantero está en algun centinela contempla el caso
    if (delantero == CENT_A) {  //Este es uno de esos casos: Si delantero está en Centinela A -> recarga el centinela B y coloca delantero en la base del bloque B
        if (inicio >= BASE_B) {
            ERR_notificar(ERR_LEXEMA_LARGO, -1, NULL); //Si el inicio del lexema está en el bloque B, entonces el lexema es demasiado largo, porque no cabría en un bloque
        }
        recargar_centinela(1);          //Recarga el centinela B
        delantero = BASE_B;                   //Coloca delantero en la base del bloque B
    } else if (delantero == CENT_B) {         //este hace lo msimo pero con el otro
        if (inicio <= CENT_A) {
            ERR_notificar(ERR_LEXEMA_LARGO, -1, NULL);
        }
        recargar_centinela(0);
        delantero = BASE_A;
    }

    char c = buf[delantero];
    if (c == '\0' && fin_fichero) return '\0';
    delantero++;
    return c;
}

void SE_retroceder()    //Es la función que se encarga de traer para atrás delantero. Trata las dos excepciones, que es si delantero está en la base.
{                       //Si eso pasa entonces lleva delantero a la posi anterior al centinela contrario
    if(delantero == BASE_B) delantero = CENT_A - 1;
    else if(delantero == BASE_A) delantero = CENT_B - 1;
    else delantero--;
}

void SE_marcar_inicio() { inicio = delantero; } //Actualiza al posi de inicio a la de delantero