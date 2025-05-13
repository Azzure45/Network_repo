#include <iostream>
#include <memory>
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
using std::cin;
using std::unique_ptr;
using std::make_unique;

typedef struct Client_t{
    int client_fd;                      // client file descriptor
    char buffer[BUFFER_SIZE] = {0};     // message buffer
    size_t buff_len;                    // buffer lenght

    Client_t(void) : 
    client_fd(-1),
    buff_len(sizeof(buffer)) // constructor
    {
    }

    void send_msg(const char * txt, int flag){ // handels sending messages to server
        memset(&buffer, sizeof(buffer), 0);
        strcpy(buffer, txt);
        if(flag != MSG_OOB){ send(client_fd, &buffer, buff_len, flag); }
        else{send(client_fd, &buffer, 1, flag);}
    }

    void read_msg(void){
        memset(&buffer, sizeof(buffer), 0);
        read(client_fd, &buffer, buff_len);
        cout << buffer << "\n";

    }
}Client_t;

typedef unique_ptr<Client_t> Client_p;

int main(void){
    Client_p c = make_unique<Client_t>();
    c->client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(c->client_fd <= 0){
        cerr << "Failed to create socket\n";
        return 1;
    }

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &client_addr.sin_addr);

    cout << "Connecting to server..." << std::endl;
    if (connect(c->client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(c->client_fd);
        return 1;
    }

    cout << "Connected successfully\n";
    // send(client_fd, buffer, sizeof(char) * sizeof(buffer), 0);
    while(true){
        int ipt = 0;
        cout << "Write a integer from 1-4 for that respective scenario\n";
        cin >> ipt;
        switch (ipt){
        case 1:
            c->send_msg("NORMAL_DATA:Hello", 0);
            c->read_msg();
            break;
        case 2:
            c->send_msg("SEND_URGENT_REQUEST", 0);
            c->send_msg("U", MSG_OOB);
            c->read_msg();
            
            c->send_msg("trailing", 0);
            c->read_msg();
            break;
        case 3:
            c->send_msg("SEND_URGENT_REQUEST", 0);
            c->send_msg("NORMAL_AFTER_REQUEST", 0);
            c->read_msg();
            break;    
        case 4:
            c->send_msg("TEST_GARBAGE", 0);
            c->read_msg();
        default:
            cout << "Please follow the instructions on the screen\n";
        }
        // sleep(1);
    }

    close(c->client_fd);
}