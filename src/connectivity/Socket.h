#ifndef SOCKET_H
#define SOCKET_H

#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// Usual socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>

typedef struct {

    int                 sockfd; // socket file descriptor
    
    //Host address
    struct sockaddr_in  host_addr;
    int                 host_addrlen;
     
    //Client address
    struct sockaddr_in  client_addr;
    int                 client_addrlen;

} http_t;


http_t* CreateHttp();
//err_code_t DestroyHttp(http_t*);

err_code_t StartServer(http_t*);

#endif