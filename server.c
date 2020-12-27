//<server.c>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define LBUF 100
#define LEN sizeof(struct sockaddr_un)
char bufer[LBUF];
main(){
    struct sockaddr_un server, client;
    int n, sock, s_len;
    int c_len = LEN;
    if((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0){
        printf("Server: impossible to create socket.\n");
        exit(1);
    }
    unlink("/home/newstorage/5893_pam/lab9/lab9.server");
    bzero(&server, LEN);
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "/home/newstorage/5893_pam/lab9/lab9.server");
    s_len = sizeof(server.sun_family) + strlen(server.sun_path);
    if(bind(sock, (struct sockaddr *)&server, s_len) < 0){
        printf(strerror(errno));
        printf("Server: error in binding.\n");
        exit(1);
    } 
    for(;;){
        n = recvfrom(sock, bufer, LBUF, 0, (struct sockaddr *)&client, &c_len);
        if(n < 0){
            printf("Server: error in recieving.\n");
            continue;
        }
        printf("From %s: %s\n", client.sun_path, bufer);
    }
}
