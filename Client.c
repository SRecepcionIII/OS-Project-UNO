#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define SERVER_ADDR "127.0.0.1"

// Uno game logic functions and structures (to be implemented)
#define HAND_SIZE 7

// Function to receive player's hand from server
void receive_hand(int sock, char hand[][32]) {
    char buffer[1024] = {0};
    read(sock, buffer, sizeof(buffer));
    printf("%s\n", buffer);
    char *token = strtok(buffer, " ");
    int i = 0;
    while (token != NULL && i < HAND_SIZE) {
        strcpy(hand[i], token);
        token = strtok(NULL, " ");
        i++;
    }
}

// Function to send player's move to server
void send_move(int sock, char *move) {
    send(sock, move, strlen(move), 0);
}

// Function to check if a move is valid
int is_valid_move(char *move, char *top_card) {
    char *move_color = strtok(move, " ");
    char *move_value = strtok(NULL, " ");

    char *top_color = strtok(top_card, " ");
    char *top_value = strtok(NULL, " ");

    // Check if the move is a Wild card or a Wild Draw Four card
    if (strcmp(move_color, "Wild") == 0 || strcmp(move_value, "Wild") == 0) {
        return 1;
    }

    // Check if the move matches the color or value of the top card
    if (strcmp(move_color, top_color) == 0 || strcmp(move_value, top_value) == 0) {
        return 1;
    }

    return 0;
}

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
    
    // Uno game setup
    char hand[HAND_SIZE][32];

    // Connect to server
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

    // Receive player's hand from server
    receive_hand(sock, hand);

    // Game loop
    while (1) {
        // Display player's hand
        printf("Your hand: ");
        for (int i = 0; i < HAND_SIZE; i++) {
            printf("%s ", hand[i]);
        }
        printf("\n");

        // Get player's move
        char move[32];
        printf("Enter your move: ");
        scanf("%s", move);

        // Send player's move to server
        send_move(sock, move);

        // Receive updated hand from server
        receive_hand(sock, hand);

        // Check for Uno
        if (declare_uno(hand, HAND_SIZE)) {
            printf("UNO!\n");
        }
        int i;
        int end_game = 0;
        for (i = 0; i < HAND_SIZE; i++) {
            if (strcmp(hand[i], "") != 0) {
                end_game = 1;
                break;
            }
        }
        if (!end_game) {
            printf("You win!\n");
            break;
        }
    }

    close(sock);
    return 0;
}
