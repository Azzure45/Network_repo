#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

const int port = 8080;
const char ip[] = "127.0.0.1";

using std::cerr;
using std::cout;

int main(void){
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd <= 0){
        cerr << "Failed to create socket\n";
        return 1;
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &client_addr.sin_addr);

    cout << "Connecting to server..." << std::endl;
    if (connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return 1;
    }

    cout << "Connected successfully\n";

    char buffer[BUFFER_SIZE] = "AnSa\n";
    // send(client_fd, buffer, sizeof(char) * sizeof(buffer), 0);
    while(true){
        // send(client_fd, buffer, sizeof(char) * sizeof(buffer), 0);
        ;;
    }

    close(client_fd);
}