#ifndef _CONVERSACION_H_
#define _CONVERSACION_H_

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#define  SIZE_OF_BUFFER 255


void error(const char *error);
void newlineRemover(char *array);
char* modoEnvio(int,char*);
char* modoRecibir(int,char*);
int transferirArchivo(int);
int recibirArchivo(int);
void gotoxy(int x, int y);
#endif
