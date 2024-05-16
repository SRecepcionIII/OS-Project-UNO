#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define SERVER_ADDR "127.0.0.1"

// Uno game logic functions and structures (to be implemented)

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Create client socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr)<=0) {
        perror("invalid address");
        return -1;
    }
    
    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return -1;
    }
    
    printf("Connected to server\n");
    
    close(sock);
    return 0;
}
