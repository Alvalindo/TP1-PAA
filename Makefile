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
	$(CC) -c $(SRC)/backtracking.c $(HEADER) -o $(OBJ)/backtracking.o
	$(CC) -c $(SRC)/labirinto.c $(HEADER) -o $(OBJ)/labirinto.o
	$(CC) -c $(SRC)/saida.c $(HEADER) -o $(OBJ)/saida.o
	$(CC) -c $(SRC)/caminho.c $(HEADER) -o $(OBJ)/caminho.o
	$(CC) -c $(SRC)/todos_os_caminhos.c $(HEADER) -o $(OBJ)/todos_os_caminhos.o
	$(CC) -c $(SRC)/gerador.c $(HEADER) -o $(OBJ)/gerador.o
	$(CC) -c $(SRC)/analise.c $(HEADER) -o $(OBJ)/analise.o
	$(CC) -c $(SRC)/utils.c $(HEADER) -o $(OBJ)/utils.o

#faco a compilacao do main tranformando-o em um executavel
	$(CC) $(TP)/main.c $(OBJ)/*.o $(HEADER) -o $(EXEC) -lm

#target de execucao (comando pra compilar no windows -> mingw32-make run || comando pra compilar no linux -> make run)
run:
	$(EXEC)

#target que limpeza (comando pra compilar no windows -> mingw32-make clear || comando pra compilar no linux -> make clear)
clear:
	$(RM) $(OBJCLEAN) $(BINCLEAN)
