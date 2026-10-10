#comandos recorrentemente executados
EXEC = ./bin/tp
CC = gcc
HEADER = -I./header

#e importante declarar essas variaveis com os nomes das pastas para que se mudarmos o nome seja mais facil muda-las no arquivo
SRC = ./src
OBJ = ./obj
TP = ./

#condicional com os comandos respectivos do sistema operacional
ifeq ($(OS),Windows_NT)
    RM = del /Q
    EXEC := $(EXEC).exe
	OBJCLEAN = .\obj\
	BINCLEAN = .\bin\*.exe
else
	EXEC = ./bin/tp
    RM = rm -f
	OBJCLEAN = ./obj/*
	BINCLEAN = ./bin/*
endif

#target de compilacao (comando pra compilar no windows -> mingw32-make || comando pra compilar no linux -> make)
all:
#faco a compilacao dos arquivos do src (.c) e transformo-os em objetos
	$(CC) -c $(SRC)/NOME.c $(HEADER) -o $(OBJ)/NOME.o
	
	

#faco a compilacao do main tranformando-o em um executavel
	$(CC) $(TP)/main.c $(OBJ)/*.o $(HEADER) -o $(EXEC) -lm

#target de execucao (comando pra compilar no windows -> mingw32-make run || comando pra compilar no linux -> make run)
run:
	$(EXEC)

#target que limpeza (comando pra compilar no windows -> mingw32-make clear || comando pra compilar no linux -> make clear)
clear:
	$(RM) $(OBJCLEAN) $(BINCLEAN)
