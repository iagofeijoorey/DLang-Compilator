CC      = gcc
CFLAGS  = -Wall
TARGET  = compilador
SRCS    = main.c analizadorsintactico.c TS.c ABB.c errores.c lex.yy.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Genera el analizador léxico con Flex
lex.yy.c: compilador.l
	flex compilador.l

clean:
	rm -f $(TARGET) lex.yy.c