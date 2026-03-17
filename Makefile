CC      = gcc
CFLAGS  = -Wall
TARGET  = compilador
SRCS    = main.c sistemaDeEntrada.c analizadorlexico.c analizadorsintactico.c TS.c ABB.c errores.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
