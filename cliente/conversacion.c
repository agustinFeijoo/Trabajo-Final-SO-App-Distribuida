#include "conversacion.h"

char *modoEnvio(int newSockFd, char *buffer)
{
    while (1)
    {

        fgets(buffer, SIZE_OF_BUFFER, stdin);

        if (strcmp(buffer, "T\n") == 0)
        {
            write(newSockFd,buffer,SIZE_OF_BUFFER);
            if (transferirArchivo(newSockFd)==0)
            {
                printf("Archivo transferido exitosamente");
                break;
            }
        }
        else
        {
            if (strcmp(buffer, "M\n") == 0)
            {
                printf("Modo envio \n");
                printf("> ");
                fgets(buffer, SIZE_OF_BUFFER, stdin);
                if (write(newSockFd, buffer, SIZE_OF_BUFFER) < 0)
                    error("Error en write()");
                break;
            }
            else
            {
                if (strcmp(buffer, "F\n") == 0){
                    write(newSockFd,buffer,SIZE_OF_BUFFER);
                    break;
                }
                else
                    printf("Lo sentimos,tu comando fue errado..Ingrese nuevamente 'T','M' o 'F'");
            }
        }
        //else vuelve a presionar el comando porque le erro
    }
    return buffer;
}

char *modoRecibir(int newSockFd, char *buffer)
{

    bzero(buffer, SIZE_OF_BUFFER);
    if (read(newSockFd, buffer, SIZE_OF_BUFFER) < 0)
        error("error en read()");
    //Veo si se trata de enviar un archivo

    if (strcmp(buffer, "T\n") == 0)
    { //entro en modo recibir archivo
        if (recibirArchivo(newSockFd) == 0){
            bzero(buffer,SIZE_OF_BUFFER);
            buffer = "El archivo se recibió con exito";
        }
    }
 

    return buffer;
}

int recibirArchivo(int sockfd)
{
    FILE *fp;

    int bytesReceived = 0, fSize, tamanoRestante;
    char fname[100];
    char recvBuff[1024];
    read(sockfd, fname, 256);
    //strcat(fname,"AK");
    printf("File Name: %s\n", fname);
    newlineRemover(fname);
    printf("Receiving file...");
    fp = fopen(fname, "ab");
    if (NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }
    read(sockfd, &fSize, sizeof(fSize)); // Tomo el tamaño del archivo
    long double sz = 1;
    /* Receive data in chunks of 256 bytes */
    tamanoRestante = fSize;
    //  while ((bytesReceived = read(sockfd, recvBuff, 1024)) > 0) //Entra una vez y se queda esperando
    while (tamanoRestante > 0)
    {
        bytesReceived = read(sockfd, recvBuff, 1024);
        tamanoRestante = tamanoRestante - bytesReceived;
        sz++;
        gotoxy(0, 4);
        printf("Received: %llf Mb", (sz / 1024));
        //  fflush(stdout);
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1, bytesReceived, fp);
        // printf("%s \n", recvBuff);

        if (bytesReceived < 0)
        {
            printf("\n Read Error \n");
        }
    }
    printf("\nFile OK....Completed\n");
    fclose(fp);
    return 0; //Se recibió el archivo con exito
}

int transferirArchivo(int newSockFd)
{
    char fname[255];
    int readedBytes = 0, fSize;
    printf("\nEscriba el nombre del archivo");
    fgets(fname, 255, stdin);
    write(newSockFd, fname, 255); //sending the name of the file
    newlineRemover(fname);

    FILE *fp = fopen(fname, "rb");
    if (fp == NULL)
    {
        perror("File open error");
    }
    else
    {
        // obtain file size:
        fseek(fp, 0, SEEK_END);
        fSize = ftell(fp);
        rewind(fp);

        write(newSockFd, &fSize, sizeof(fSize)); //Envio la cantidad de bytes del archivo
        /* Read data from file and send it */
        while (1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[1024] = {0};
            int nread = fread(buff, 1, 1024, fp);

            //printf("Bytes read %d \n", nread);

            /* If read was success, send data. */
            if (nread > 0)
            {
                printf("Sending \n");
                write(newSockFd, buff, nread);
            }
            if (nread < 1024)
            {
                if (feof(fp))
                {
                    printf("End of file\n");

                    //write(newSockFd,"fin de archivo",sizeof("fin de archivo"));//le envio que se termina el arch?
                }
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }
        }
    }

    return 0;
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}
void newlineRemover(char *array)
{
    int i, lenght;
    lenght = strlen(array);
    for (i = 0; i < lenght; i++)
    {
        if (array[i] == '\n')
            array[i] = '\0';
    }
}
void error(const char *error)
{
    perror(error);
    exit(1);
}
