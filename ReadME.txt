//Guía de uso

Se puede usar el makefile incluido, usando el comando
make

o bien se puede compilar el programa usar los siguientes comandos en orden:
flex -o lex_yy.c compilador.l
gcc -o compilador main.c analizadorsintactico.c lex_yy.c TS.c ABB.c errores.c

Esto generará el ejecutable: compilador

Para ejecutarlo utilizar el siguiente comando:
./compilador "ruta_archivo"  (ej.: ./compilador regression.d)

Esto ejecutara el analizador para el archivo que se le indique. En caso de que no se le indique una ruta a un archivo como argumento (es decir, utilizando el comando sin argumentos: ./compilador) se buscará el archivo regression.d localizado en la misma carpeta que el ejecutable y se analizará este mismo si es que se encuentra dicho archivo.





// Explicación del programa
 
El main abre el fichero indicado por parámetro (o regression.d por defecto) y lo asigna a yyin, la variable global de Flex que apunta al flujo de entrada. Después inicializa la tabla de símbolos, que ya viene cargada con las palabras reservadas del lenguaje.
 
A continuación se llama a AS_imprimir_lexemas() del analizador sintáctico. Esta función pide tokens uno a uno llamando a yylex() (generado por Flex a partir de compilador.l) e imprime cada uno con el formato <tipo, "lexema"> hasta que se agota la entrada.
 
Flex se encarga de leer el fichero directamente desde yyin: gestiona su propio buffer interno, por lo que no hace falta ningún sistema de entrada separado. Cada vez que reconoce un token actualiza la variable global _ultimo_token y devuelve el código numérico del tipo.
 
Cuando se lee un identificador, el analizador léxico consulta la tabla de símbolos para saber si es una palabra reservada ya conocida o un identificador nuevo. En el segundo caso lo inserta con código ID.
 
Al terminar el análisis, el main imprime el estado final de la TS (palabras reservadas + todos los identificadores encontrados) y libera la memoria antes de cerrar el fichero.