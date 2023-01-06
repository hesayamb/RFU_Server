#include "Socket.h"
#include "sys_config.h"

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];
char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html><h1> ti tolko posmotri</h1><br>"
                    "<h3>hAhAhA a ya zapustil server </h3><html>\r\n";

http_t* CreateHttp() {

    http_t* http = (http_t*)malloc(sizeof(http_t));

    //Create Socket
    http->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (http->sockfd == -1) {
        perror("webserver (socket)");
        free(http);
        return NULL;
    }

    //Host description
    http->host_addrlen = sizeof(http->host_addr);

    http->host_addr.sin_family = AF_INET;
    http->host_addr.sin_port   = htons(DEFAULT_PORT);
    http->host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //Client description
    http->client_addrlen = sizeof(http->client_addr);

    // Bind the socket to the address 
    if (bind(http->sockfd, (struct sockaddr*)&http->host_addr, http->host_addrlen) != 0) {
        perror("webserver (bind)");
        free(http);
        return NULL;
    }

    return http;
}

err_code_t StartServer(http_t* http) {

     // Listen for incoming connections
    if (listen(http->sockfd, SOMAXCONN) != 0) {
        perror("webserver (listen)");
        return ERR_CODE_ERROR;
    }
    printf("server listening for connections\n");

    for (;;) {
        // Accept incoming connections
        int newsockfd = accept(http->sockfd, (struct sockaddr *)&http->host_addr,
                               (socklen_t *)&http->host_addrlen);
        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted\n");

        // Get client address
        int sockn = getsockname(newsockfd, (struct sockaddr *)&http->client_addr,
                                (socklen_t *)&http->client_addrlen);
        if (sockn < 0) {
            perror("webserver (getsockname)");
            continue;
        }

        // Read from the socket
        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        if (valread < 0) {
            perror("webserver (read)");
            continue;
        }

        // Read the request
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n", inet_ntoa(http->client_addr.sin_addr),
               ntohs(http->client_addr.sin_port), method, version, uri);

        // Write to the socket
        int valwrite = write(newsockfd, resp, strlen(resp));
        if (valwrite < 0) {
            perror("webserver (write)");
            continue;
        }

        close(newsockfd);
    }

    return ERR_CODE_SUCCESS;
}