#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define MAX_CLIENTS 2

// Uno game logic functions and structures (to be implemented)

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind server socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Accept incoming connections
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        client_sockets[i] = new_socket;
        printf("Client %d connected\n", i+1);
    }
    
    // Close all client sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        close(client_sockets[i]);
    }
    return 0;
}
