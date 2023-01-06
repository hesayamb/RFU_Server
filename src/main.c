#include "connectivity/Socket.h"

int main() {

    http_t* http = CreateHttp();
    if(http != NULL)
    {
        StartServer(http);
    }
    return 0;
}