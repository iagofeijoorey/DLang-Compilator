//Guía de uso

- Para compilar el programa usar el siguiente comando:
gcc -o compilador main.c sistemaDeEntrada.c analizadorlexico.c analizadorsintactico.c TS.c ABB.c errores.c

Esto generará el ejecutable: compilador

Para ejecutarlo utilizar el siguiente comando:
./compilador "ruta_archivo"  (ej.: ./compilador regression.d)

Esto ejecutara el analizador para el archivo que se le indique. En caso de que no se le indique una ruta a un archivo como argumento (es decir, utilizando el comando sin argumentos: ./compilador) se buscará el archivo regression.d localizado en la misma carpeta que el ejecutable y se analizará este mismo si es que se encuentra dicho archivo.





//Explicación del programa

Este proyecto está compuesto por un main que comprueba los argumentos pasados por terminal (si es que se le han pasado, sino se trata el caso por defecto) y luego ejecuta el análisis del compilador. Esto se hace llamando a la función AD_imprimir_lexemas() del analizador sintáctico (analizadorsintactico.c/.h) 

El analizador sintáctico realmente solo se encarga de pedir componentes léxicos e imprimirlo por pantalla, no de hacer un análisis sintáctico real. Esto se hace invocando la función AL_sig_comp_lexico() del analizador léxico (analizadorlexico.c/.h). 

Esta clase se encarga de ejecutar un autómata que pide caracteres al Sistema de Entrada y dependiendo de que tipo de carácter sea llama a un autómata específico para el tipo de lexema que vamos a leer. Cada autómata sigue leyendo caracteres mientras no termina el autómata. Esto llama a la función SE_sig_caracter() del Sistema de entrada (SE) que es lo que le permite ir recibiendo los caracteres del fichero que se está analizando.

El sistema de entrada tiene precisamente esa función, leer el fichero. Cuenta con un sistema de doble centinela capaz de ir metiendo en el buffer (en mi caso es un único buffer, que trato como si estuviera dividido en dos) el contenido del fichero. Luego la función devuelve el carácter leído del buffer.