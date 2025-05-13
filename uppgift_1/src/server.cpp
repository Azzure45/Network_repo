#include <iostream>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::cerr;
using std::cout;
using std::unique_ptr;
using std::make_unique;

const int port = 8080;
const char ip[] = "127.0.0.1";
typedef struct Server_t{
    int client_fd;  // client file descriptor
    int server_fd;  // server file descriptor

    Server_t(void){ // constructor
        this->client_fd=-1;
        this->server_fd=-1;
    }
}Server_t;

typedef unique_ptr<Server_t> Server_p;

int main(void){
    Server_p s = make_unique<Server_t>();
    s->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(s->server_fd <= 0){
        cerr << "Failed to create server socket\n";
        exit(1);
    }
    
    int opt = 1;
    if(setsockopt(s->server_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0){
        cerr << "Failed to use setsockopt\n";
        exit(2);
    }

    struct sockaddr_in s_addr;
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family=AF_INET;
    s_addr.sin_port=htons(port);
    inet_pton(AF_INET, ip, &s_addr.sin_addr);

    if(bind(s->server_fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0){
        cerr << "Failed to bind socket\n";
        exit(3);
    }

    if(listen(s->server_fd, 1) < 0){
        cerr << "Failed to listen\n";
        exit(4);
    }
    cout << "Listening on port: " << port << "...\n";
    struct sockaddr_in new_client;
    socklen_t client_len = sizeof(new_client);

    while(true){    
        s->client_fd = accept(s->server_fd, (struct sockaddr *)&new_client, &client_len);
        if(s->client_fd < 0){
            cerr << "Failed to find client\n";
            //continue; //?
        }
    }
}