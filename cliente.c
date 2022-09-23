#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Se conecta al socket del servidor, le envía un texto
predefinido, lee lo que el servidor le responde y desconecta
*/

#define SERVER_ADDRESS "192.168.0.xxx" /* IP servidor */
#define PORT 4040

/* Secuencias de prueba */
char buf_tx[] = "El cliente se está conectando";
char buf_rx[100]; /* recibe al buffer */

int main() /* Este cliente se conecta, envía un texto y se desconecta. */
{
    int sockfd;
    struct sockaddr_in servaddr;

    /* Creación del Socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("CLIENT: La creación del socket ha fallado...\n");
        return -1;
    }
    else
    {
        printf("CLIENT: La creación del Socket ha sido con éxito..\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));

    /* Asignación de IP, PUERTO */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr( SERVER_ADDRESS );
    servaddr.sin_port = htons(PORT);

    /* El socket del cliente va a tratar de conectarse al socket servidor */
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("La conexión al servidor ha fallado...\n");
        return -1;
    }
    printf("Conectado al servidor..\n");

    /* Envíar datos*/
    write(sockfd, buf_tx, sizeof(buf_tx));
    read(sockfd, buf_rx, sizeof(buf_rx));
    printf("CLIENT:Recibido: %s \n", buf_rx);

    /* close the socket */
    close(sockfd);
}