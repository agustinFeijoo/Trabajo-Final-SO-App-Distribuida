#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
//Definidas por mi
#include "conversacion.h"

#define SIZE_OF_BUFFER 1024
 
char buffer[SIZE_OF_BUFFER];


int main()
{
    int sockfd, port, nPalabras = 0, cMensajeFichero = 1, nCaracteresFichero = 0, words = 0;
    char  c, fichero[100], ch,host[100];
    socklen_t addrlen;
    int modificandoArchivo=0;
    struct hostent *he;
    struct sockaddr_in serv_addr;
    FILE *f;

    printf("Ingrese el puerto ");
    scanf("%d", &port);
    fgets(buffer, SIZE_OF_BUFFER, stdin);
    printf("Ingrese el host,ej:127.0.0.1\n");
    scanf("%s",&host);
    he = gethostbyname2(host, AF_INET);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // porque no da err de parseo?
        error((char *)"Error al conectar");

    printf("Para salir del chat escribir 'F'\n");
    printf("Para enviar un archivo escribir 'T' \n");
    printf("Para enviar un mensaje 'M'");
    fgets(buffer,SIZE_OF_BUFFER,stdin);//limpio el buffer
    while (strncmp(buffer, "F\n",1) != 0)
    {
        printf("\n Cliente (ingrese el modo 'T','F','M','C'):");
        //Modo envio también hace la llamada a transferirArchivo(...)
        strcpy(buffer,modoEnvio(sockfd,buffer,modificandoArchivo));
        if(strncmp(buffer, "F\n",1) == 0)
            break;

        printf("\nServer:");
        printf("%s",modoRecibir(sockfd,buffer));
        //Modo recibir también hace la llamada a recibirArchivo(...)
        if(strncmp(buffer, "C\n",1) == 0){
             modificandoArchivo=1;
            
        }
            
    }
    close(sockfd);
}
