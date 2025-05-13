#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using std::cerr;
using std::cout;

int main(void){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd <= 0){
        cerr << "Failed to create socket\n";
        return 1;
    }
    // int optval = 1;
    // if(setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0 ){
    //     cerr << "Failed to change options\n";
    //     close(client_fd);
    //     return 1;
    // }
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    // client_addr.sin_addr = "172.16.217.188";
    inet_pton(AF_INET, "172.16.217.188", &client_addr.sin_addr);

    std::cout << "Connecting to server..." << std::endl;
    if (connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return 1;
    }

    // int server_fd = accept(client_fd, (struct sockaddr *)&server_addr, &server_addr_len);
    char buffer[BUFFER_SIZE] = "AnSa\n";
    // int valread = read(sock, buffer, BUFFER_SIZE);
    send(client_fd, buffer, sizeof(char) * sizeof(buffer), 0);
    while(true){
        cout << "yipiy\n";
        ;;
    }

    close(client_fd);
}