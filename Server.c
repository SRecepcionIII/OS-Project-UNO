#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define MAX_CLIENTS 2

// Uno game logic functions and structures (to be implemented)
#define DECK_SIZE 108
#define HAND_SIZE 7

char *Color[] = {"Red", "Yellow", "Green", "Blue"};
char *Value[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Skip", "Reverse", "+2", "Wild"};

void shuffle_deck(char deck[][32], int size) {
    srand(time(NULL)); // Seed the random number generator

    // Fisher-Yates shuffle algorithm
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Generate a random index between 0 and i
        // Swap deck[i] and deck[j]
        char temp[32];
        strcpy(temp, deck[i]);
        strcpy(deck[i], deck[j]);
        strcpy(deck[j], temp);
    }
}

void deal_cards(char deck[][32], char player_hands[MAX_CLIENTS][HAND_SIZE][32]) {
    // Implement dealing cards logic
    // For simplicity, we will just deal the first HAND_SIZE * MAX_CLIENTS cards
    for (int i = 0; i < HAND_SIZE * MAX_CLIENTS; i++) {
        strcpy(player_hands[i / HAND_SIZE][i % HAND_SIZE], deck[i]);
    }
}

void display_hand(char *hand[], int size) {
    printf("Your hand: ");
    for (int i = 0; i < size; i++) {
        printf("%s ", hand[i]);
    }
    printf("\n");
}

// Function to handle player's turn
void player_turn(int client_socket, char hand[][32]) {
    // Send player's hand to client
    char buffer[1024] = {0};
    sprintf(buffer, "Your hand: ");
    for (int i = 0; i < HAND_SIZE; i++) {
        strcat(buffer, hand[i]);
        strcat(buffer, " ");
    }
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive player's move
    char move[32];
    recv(client_socket, move, sizeof(move), 0);
    printf("Player move: %s\n", move);
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

// Function to execute player's move
void execute_move(char *move, char *hand[], int *hand_size) {
    // Remove the played card from hand
    for (int i = 0; i < *hand_size; i++) {
        if (strcmp(hand[i], move) == 0) {
            for (int j = i; j < *hand_size - 1; j++) {
                strcpy(hand[j], hand[j + 1]);
            }
            strcpy(hand[*hand_size - 1], "");
            break;
        }
    }
    (*hand_size)--;
}

// Function to handle player's draw card action
void draw_card(char *deck[], char *hand[], int *hand_size) {
    // Draw a card from the deck
    strcpy(hand[*hand_size], deck[0]);
    (*hand_size)++;
    // Shift the deck to remove the drawn card
    for (int i = 0; i < DECK_SIZE - 1; i++) {
        strcpy(deck[i], deck[i + 1]);
    }
    strcpy(deck[DECK_SIZE - 1], "");
}

// Function to handle player's Uno declaration
int declare_uno(char *hand[], int hand_size) {
    // Check if player has only one card left
    if (hand_size == 1) {
        return 1;
    }
    return 0;
}

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
    
    // Uno game setup
    char deck[DECK_SIZE][32];
    char player_hands[MAX_CLIENTS][HAND_SIZE][32];

    // Initialize and shuffle deck
    shuffle_deck(deck, DECK_SIZE);

    // Deal cards to players
    deal_cards(deck, player_hands);

    // Game loop
    int client_sockets[MAX_CLIENTS] = {0};
    for (int i = 0; i < MAX_CLIENTS; i++) {
        // Accept incoming connections
        if ((client_sockets[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Client %d connected\n", i+1);

        // Handle player's turn
        char player_hand[HAND_SIZE][32];
        deal_cards(NULL, player_hand);  // Placeholder for dealing cards
        player_turn(client_sockets[i], player_hand);
    }

    // Close all client sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        close(client_sockets[i]);
    }
    return 0;
}
