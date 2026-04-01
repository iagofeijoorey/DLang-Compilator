%{
#include <string.h>
#include "definiciones.h"
#include "errores.h"
#include "TS.h"

ComponeneteLexico _ultimo_token;

static ComponeneteLexico hacer_token(int tipo, const char *lexema) {
    ComponeneteLexico t;
    t.tipo = tipo;
    strncpy(t.lexema, lexema, TAM_LEXEMA - 1);
    t.lexema[TAM_LEXEMA - 1] = '\0';
    return t;
}
%}

%option noyywrap
%option yylineno
%option nounput

DIGITO      [0-9]
LETRA       [a-zA-Z_]
ALNUM       [a-zA-Z0-9_]
HEX         [0-9a-fA-F]
BIN         [01]
ENTERO      {DIGITO}({DIGITO}|_)*
REAL        {ENTERO}"."{DIGITO}({DIGITO}|_)*([eE][+-]?{DIGITO}+)?
REAL_EXP    {ENTERO}[eE][+-]?{DIGITO}+
HEXA        "0"[xX]{HEX}({HEX}|_)*
BINARIO     "0"[bB]{BIN}({BIN}|_)*

%%

[ \t\r\n]+              { }

"//"[^\n]*              { }

"/*"([^*]|\*+[^*/])*\*+"/"  { }

"/+"                    {
                            int depth = 1;
                            int c1, c2;
                            c1 = input();
                            while (c1 != EOF) {
                                if (c1 == '/' && (c2 = input()) == '+') {
                                    depth++;
                                    c1 = input();
                                } else if (c1 == '+' && (c2 = input()) == '/') {
                                    depth--;
                                    if (depth == 0) break;
                                    c1 = input();
                                } else if (c1 == '/' || c1 == '+') {
                                    c1 = c2;
                                } else {
                                    c1 = input();
                                }
                            }
                        }

{LETRA}{ALNUM}*         {
                            int codigo = TS_buscar(yytext);
                            if (codigo == -1) {
                                TS_insertar(yytext, ID);
                                codigo = ID;
                            }
                            _ultimo_token = hacer_token(codigo, yytext);
                            return codigo;
                        }

{HEXA}                  { _ultimo_token = hacer_token(LIT_ENTERO, yytext); return LIT_ENTERO; }
{BINARIO}               { _ultimo_token = hacer_token(LIT_ENTERO, yytext); return LIT_ENTERO; }
{REAL}                  { _ultimo_token = hacer_token(LIT_REAL,   yytext); return LIT_REAL;   }
{REAL_EXP}              { _ultimo_token = hacer_token(LIT_REAL,   yytext); return LIT_REAL;   }
{ENTERO}                { _ultimo_token = hacer_token(LIT_ENTERO, yytext); return LIT_ENTERO; }

\"([^\"\\]|\\.)*\"      { _ultimo_token = hacer_token(LIT_STRING, yytext); return LIT_STRING; }
\"[^\"\n]*\n            {
                            ERR_notificar(ERR_STRING_ABIERTO, yylineno, NULL);
                            _ultimo_token = hacer_token(LIT_STRING, yytext);
                            return LIT_STRING;
                        }

"++"                    { _ultimo_token = hacer_token(INC,         yytext); return INC;         }
"+="                    { _ultimo_token = hacer_token(MAS_ASIG,    yytext); return MAS_ASIG;     }
"=="                    { _ultimo_token = hacer_token(IGUAL,       yytext); return IGUAL;        }
"<="                    { _ultimo_token = hacer_token(MENOR_IGUAL, yytext); return MENOR_IGUAL;  }

"+"     { _ultimo_token = hacer_token(MAS,          yytext); return MAS;          }
"-"     { _ultimo_token = hacer_token(MENOS,        yytext); return MENOS;        }
"*"     { _ultimo_token = hacer_token(POR,          yytext); return POR;          }
"/"     { _ultimo_token = hacer_token(DIV,          yytext); return DIV;          }
"%"     { _ultimo_token = hacer_token(MOD,          yytext); return MOD;          }
"="     { _ultimo_token = hacer_token(ASIG,         yytext); return ASIG;         }
"<"     { _ultimo_token = hacer_token(MENOR,        yytext); return MENOR;        }
">"     { _ultimo_token = hacer_token(MAYOR,        yytext); return MAYOR;        }
"!"     { _ultimo_token = hacer_token(NOT,          yytext); return NOT;          }
";"     { _ultimo_token = hacer_token(PUNTO_COMA,   yytext); return PUNTO_COMA;   }
","     { _ultimo_token = hacer_token(COMA,         yytext); return COMA;         }
"."     { _ultimo_token = hacer_token(PUNTO,        yytext); return PUNTO;        }
":"     { _ultimo_token = hacer_token(DOS_PUNTOS,   yytext); return DOS_PUNTOS;   }
"?"     { _ultimo_token = hacer_token(INTERROGA,    yytext); return INTERROGA;    }
"("     { _ultimo_token = hacer_token(ABRE_PAR,     yytext); return ABRE_PAR;     }
")"     { _ultimo_token = hacer_token(CIERRA_PAR,   yytext); return CIERRA_PAR;   }
"{"     { _ultimo_token = hacer_token(ABRE_LLAVE,   yytext); return ABRE_LLAVE;   }
"}"     { _ultimo_token = hacer_token(CIERRA_LLAVE, yytext); return CIERRA_LLAVE; }
"["     { _ultimo_token = hacer_token(ABRE_CORCH,   yytext); return ABRE_CORCH;   }
"]"     { _ultimo_token = hacer_token(CIERRA_CORCH, yytext); return CIERRA_CORCH; }
"~"     { _ultimo_token = hacer_token(TILDE,        yytext); return TILDE;        }

.       {
            fprintf(stderr, "ERROR: caracter inesperado '%s' en linea %d\n", yytext, yylineno);
            exit(1);
        }

%%
