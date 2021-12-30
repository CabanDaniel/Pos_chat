//
// Created by Matúš Baláži on 28. 12. 2021.
//

#include "server.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <langinfo.h>

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    printf("\033[32;1mSERVER: Zapnutie prebehlo uspesne.\033[0m\n");

    listen(sockfd, 5);

    //nadviazeme spojenie s jednym klientskym socketom
    cli_len = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        return 3;
    }

    int bolExit = 0;
    while (bolExit == 0) {
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        //citanie buffra co prisiel na socket
        if (n < 0) {
            perror("Error reading from socket");
            return 4;
        }
        char *typSpravy;
        typSpravy = strtok(buffer, " ");

        if (strcmp(typSpravy, "registracia") == 0) {
            int res = spracovanieRegistracie(newsockfd, n);

        } else if (strcmp(typSpravy, "prihlasenie") == 0) {
            int res = spracovaniePrihlasenia(newsockfd, n);

        } else if (strcmp(typSpravy, "chatovanie") == 0) {
            int res = spracovanieChatovania(newsockfd, n);

        } else if (strcmp(typSpravy, "exit") == 0) {
            bolExit = 1;

        }
    }
    close(newsockfd);
    close(sockfd);

    return 0;
}
