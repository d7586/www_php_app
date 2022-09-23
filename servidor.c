
/*Simbolos estándar */
#include <unistd.h>
/* sockets */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
/* strings / errors*/
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define SERV_PORT 4040 /* puerto de conexión*/
#define SERV_HOST_ADDR "192.168.0.xxx" /*Dirección IP con soporte IPV4*/
#define BUF_SIZE 100 /* Tamaño del Buffer para enviar datos (tx) como para recibir datos (rx)*/
#define BACKLOG 3 /* Conexiones en cola */

int main(int argc, char* argv[]) /* los argumentos de entrada no se utilizan */
{
    int sockfd, connfd ; /* descriptores de archivo de socket de escucha y socket de conexión*/
    unsigned int len; /* longitud de la dirección del cliente */
    struct sockaddr_in servaddr, client; /* estructura una para el servidor y otra para el cliente */
    int len_rx, len_tx = 0; /* longitud recibida y enviada en bytes*/
    char buff_tx[BUF_SIZE] = "Servidor activo";
    char buff_rx[BUF_SIZE]; /* buffers de recepción */

    /* Creación del socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Definimos el tipo de socket a utilizar*/
    if (sockfd == -1)
    {
        fprintf(stderr, "[SERVER-error]: La creación del socket falló %d: %s \n", errno,
                strerror( errno ));
        return -1;
    }
    else
    {
        printf("[SERVER]: Socket creado con éxito..\n");
    }

    /* estructura */
    memset(&servaddr, 0, sizeof(servaddr));

    /* Definir los campos IP, SERV_PORT, IPV4 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    servaddr.sin_port = htons(SERV_PORT);

    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        fprintf(stderr, "[SERVER-error]: Fallo en el socket enlazado %d: %s \n", errno,
                strerror( errno ));
        return -1;
    }
    else
    {
        printf("[SERVER]: Socket enlazado correctamente \n");
    }

    if ((listen(sockfd, BACKLOG)) != 0)
    {
        fprintf(stderr, "[SERVER-error]: la escucha del socket falló. %d: %s \n", errno,
                strerror( errno ));
        return -1;
    }
    else
    {
        printf("[SERVER]: Escucha en SERV_PORT %d \n\n", ntohs(servaddr.sin_port) );
    }
    len = sizeof(client); /* Inicio del tamaño de dirección del cliente */

    /* Acepta los dato que van entrando */
    while(1)
    {
        connfd = accept(sockfd, (struct sockaddr *)&client, &len);
        if (connfd < 0)
        {
            fprintf(stderr, "[SERVER-error]: conexión no aceptada. %d: %s \n", errno, strerror( errno ));
            return -1;
        }
        else
        {
            while (1) /* Lo primero que hará el servidor es detectar si hay datos del cliente*/
            {
                /* leer el mensaje del cliente, copiarlo en el búfer */
                len_rx = read(connfd, buff_rx, sizeof(buff_rx));
                if (len_rx == -1) {
                    fprintf(stderr, "[SERVER-error]: connfd no se puede leer. %d: %s \n", errno, strerror(errno));
                } else if (len_rx == 0) /* if length is 0 client socket closed, then exit */
                {
                    printf("[SERVER]: socket cliente ha cerrado \n\n");
                    close(connfd);
                    break;
                } else {
                    write(connfd, buff_tx, strlen(buff_tx));
                    printf("[SERVER]: %s \n", buff_rx);
                }
            }
        }
    }
}