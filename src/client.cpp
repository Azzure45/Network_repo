#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::cerr;
using std::cout;

int main(void){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd <= 0){
        cerr << "Failed to create socket\n";
        return 1;
    }
    int optval = 1;
    if(setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0 ){
        cerr << "Failed to change options\n";
        return 1;
    }
    struct sockaddr_in client_addr;
    memset(&client_fd, 0, sizeof(client_fd));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "172.16.217.188", &client_addr.sin_addr);

    int status = bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    //do some error check

    if(listen(client_fd, 5) < 0){
        cerr << "failed to listen for server\n";
        return 1;
    }
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    int server_fd = accept(client_fd, (struct sockaddr *)&server_addr, &server_addr_len);

    close(client_fd);
}