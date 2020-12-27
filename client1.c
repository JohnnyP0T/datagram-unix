//<client1.c>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#define LBUF 100
#define LEN sizeof(struct sockaddr_un)

char bufer[LBUF];
main(int argc, char *argv[]){
    if(argc != 2){
        printf("Need file name.\n");
        exit(1);
    }
    char *estr;
    FILE *fil;
    if((fil = fopen(argv[1], "r")) == NULL){
        printf("Cannot open the file.\n");
        exit(1);
    }
    struct sockaddr_un server, client;
    int n, sock, s_len, c_len, msgien;
    if((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0){
        printf("Client: impossible to create socket.\n");
        exit(1);
    }
    bzero(&client, LEN);
    client.sun_family = AF_UNIX;
    strcpy(client.sun_path, "/home/newstorage/5893_pam/lab9/client1Pot");
    //mktemp(client.sun_path);
    c_len = sizeof(client.sun_family) + strlen(client.sun_path);

    if(bind(sock, (struct sockaddr *)&client, c_len) < 0){
        printf("Client: error in binding\n");
        exit(1);
    }

    bzero(&server, LEN);
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "/home/newstorage/5893_pam/lab9/lab9.server");
    s_len = sizeof(server.sun_family) + strlen(server.sun_path);

    while(1){
        estr = fgets(bufer, sizeof(bufer), fil);
        if(estr != NULL){
            if(sendto(sock, bufer, LBUF, 0, (struct sockaddr *)&server, s_len) != LBUF){
                printf("Client: error in sending.\n");
                exit(1);
            }
        }
	else exit(1); //условие выхода из бесконечного цикла
    }
    fclose(fil);
    close(sock);
    unlink(client.sun_path);
    exit(0);
}
