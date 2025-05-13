#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::vector;
using std::string;
using std::cerr;
using std::cout;
using std::unique_ptr;
using std::make_unique;

#define BUFFER_SIZE 1024

const int port = 8080;
const char ip[] = "127.0.0.1";
//const char * flags = {"NORMAL_DATA", "SERVER_ACK", "SEND_URGENT_REQUEST", "SERVER_URGENT_ACK", "SERVER_NO_URGENT_DATA", "SERVER_UNKNOWN_COMMAND"};
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
    char buffer[BUFFER_SIZE] = {0};

    while(true){
        if(s->client_fd == -1){    
            s->client_fd = accept(s->server_fd, (struct sockaddr *)&new_client, &client_len);
            if(s->client_fd < 0){
                cerr << "Failed to find client\n";
            }
            cout << "connected to client fd: " << s->client_fd << "\n";
        }

        int read_bytes = recv(s->client_fd, &buffer, sizeof(buffer), 0); //?MSG_PEEK
        // Cheak so that the server actually recived some code
        if(read_bytes < 0){
            cerr << "Error read msg: " << read_bytes << "\n";
            return 0;
        }
        cout << buffer << "\n";
        if(!strcmp(buffer, "NORMAL_DATA:Hello")){
            memset(&buffer, sizeof(buffer), 0);
            strcpy(buffer, "SERVER_ACK:Hello");
            send(s->client_fd, &buffer, sizeof(char) * sizeof(buffer), 0);
        }

    }
    close(s->server_fd);
}

/*
!Extra code which was fun to write, but not up to specs
    char *p;
        p = strtok(buffer, ":");
        if(!strcmp(buffer, "NORMAL_DATA")){
            char *msg = strtok(NULL, ":");
            cout << msg << "\n";
            char flag[] = {"SERVER_ACK:"};
            memset(&buffer, sizeof(buffer), 0);
            strcpy(buffer, strcat(flag, msg));
            send(s->client_fd, &buffer, sizeof(buffer), 0);
        }
*/