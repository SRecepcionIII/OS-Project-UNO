#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8888
#define MAX_CLIENTS 2
#define DECK_SIZE 108
#define HAND_SIZE 7
#define MOVE_SIZE 32

// Structure to hold player information
struct PlayerInfo {
    int socket;
    char hand[HAND_SIZE][MOVE_SIZE];
    char *top_card;
};

// Uno game logic functions and structures (to be implemented)
char *Color[] = {"Red", "Yellow", "Green", "Blue"};
char *Value[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Skip", "Reverse", "+2", "Wild"};

void shuffle_deck(char deck[][MOVE_SIZE], int size) {
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MOVE_SIZE];
        strcpy(temp, deck[i]);
        strcpy(deck[i], deck[j]);
        strcpy(deck[j], temp);
    }
}

void deal_cards(char deck[][MOVE_SIZE], char player_hands[MAX_CLIENTS][HAND_SIZE][MOVE_SIZE]) {
    for (int i = 0; i < HAND_SIZE * MAX_CLIENTS; i++) {
        strcpy(player_hands[i / HAND_SIZE][i % HAND_SIZE], deck[i]);
    }
}

// Function to handle player's turn in a separate thread
void *handle_player_turn(void *player_info_ptr) {
    struct PlayerInfo *player_info = (struct PlayerInfo *)player_info_ptr;
    int client_socket = player_info->socket;
    char (*hand)[MOVE_SIZE] = player_info->hand;
    char *top_card = player_info->top_card;

    char buffer[1024] = {0};
    sprintf(buffer, "Your hand: ");
    for (int i = 0; i < HAND_SIZE; i++) {
        strcat(buffer, hand[i]);
        strcat(buffer, " ");
    }
    send(client_socket, buffer, strlen(buffer), 0);

    // Send the top card to the client for reference
    send(client_socket, top_card, strlen(top_card), 0);

    char move[MOVE_SIZE];
    recv(client_socket, move, sizeof(move), 0);
    printf("Player move: %s\n", move);

    // Check if the move is valid
    if (is_valid_move(move, top_card)) {
        // Update game state if the move is valid
        execute_move(move, hand);
        // Acknowledge the move
        send(client_socket, "Valid move", strlen("Valid move"), 0);
    } else {
        // Inform the player of an invalid move
        send(client_socket, "Invalid move", strlen("Invalid move"), 0);
        // Optionally, you can prompt the player to make another move
    }

    // Close the client socket and terminate the thread
    close(client_socket);
    free(player_info);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
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

    // Uno game setup
    char deck[DECK_SIZE][MOVE_SIZE];
    char player_hands[MAX_CLIENTS][HAND_SIZE][MOVE_SIZE];

    // Initialize and shuffle deck
    shuffle_deck(deck, DECK_SIZE);

    // Deal cards to players
    deal_cards(deck, player_hands);

    // Accept incoming connections and handle player turns
    pthread_t threads[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Create player information struct
        struct PlayerInfo *player_info = (struct PlayerInfo *)malloc(sizeof(struct PlayerInfo));
        player_info->socket = new_socket;
        memcpy(player_info->hand, player_hands[i], sizeof(player_info->hand));
        player_info->top_card = deck[0]; // Assuming the top card of the deck is the initial top card

        // Create thread to handle player's turn
        if (pthread_create(&threads[i], NULL, handle_player_turn, (void *)player_info) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Close server socket
    close(server_fd);

    return 0;
}