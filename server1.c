#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8888
#define MAX_CLIENTS 2
#define NUM_PLAYERS 2
#define HAND_SIZE 7
#define DECK_SIZE 40

char *Color[4] = {"Red", "Yellow", "Green", "Blue"};
char *Value[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "Cancel", "Skip", "Reverse", "Wild", "+2", "+4"};
char Deck[40][16];
char PlayerCard[7];

// Function to initialize the Uno deck
void init_deck() {
    int card_index = 0;
    for (int color_index = 0; color_index < 4; color_index++) {
        for (int value_index = 0; value_index < 16; value_index++) {
            sprintf(Deck[card_index], "%s %s", Color[color_index], Value[value_index]);
            card_index++;
        }
    }
}

// Function to shuffle the Uno deck
void shuffle_deck(char deck[][16]) {
    srand(time(NULL));
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        char temp[16];
        strcpy(temp, deck[i]);
        strcpy(deck[i], deck[j]);
        strcpy(deck[j], temp);
    }
}

// Function to deal cards to players
void deal_cards(char deck[][16], char players[][7][16]) {
    int card_index = 0;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < HAND_SIZE; j++) {
            strcpy(players[i][j], deck[card_index]);
            card_index++;
        }
    }
}

// Function to display player's hand
void display_hand(char player_hand[][16]) {
    printf("Your hand: ");
    for (int i = 0; i < HAND_SIZE; i++) {
        printf("%s ", player_hand[i]);
    }
    printf("\n");
}

// Function to check if a player's move is valid
bool is_valid_move(char *top_card, char *player_card) {
    char top_color[16];
    char top_value[16];
    char player_color[16];
    char player_value[16];
    
    sscanf(top_card, "%s %s", top_color, top_value);
    sscanf(player_card, "%s %s", player_color, player_value);
    
    return (strcmp(top_color, player_color) == 0 || strcmp(top_value, player_value) == 0);
}

// Function to execute player's move
void execute_move(char *top_card, char *player_card) {
    strcpy(top_card, player_card);
}

// Function to handle player's turn
void player_turn(char *top_card, char player_hand[][16]) {
    printf("It's your turn!\n");
    display_hand(player_hand);
    printf("Enter the index of the card you want to play: ");
    int move_index;
    scanf("%d", &move_index);

    if (move_index < 0 || move_index >= HAND_SIZE) {
        printf("Invalid index. Please try again.\n");
        player_turn(top_card, player_hand);
        return;
    }

    char selected_card[16];
    strcpy(selected_card, player_hand[move_index]);
    if (!is_valid_move(top_card, selected_card)) {
        printf("Invalid move. Please try again.\n");
        player_turn(top_card, player_hand);
        return;
    }

    execute_move(top_card, selected_card);
}

// Function to handle game over condition
bool is_game_over(char player_hand[][16]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        if (strcmp(player_hand[i], "") != 0) {
            return false;
        }
    }
    return true;
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
    if (listen(server_fd, MAX_CLIENTS) < 0) {
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

    // Initialize deck
    init_deck();

    // Shuffle deck
    shuffle_deck(Deck);

    // Deal cards to players
    char players[NUM_PLAYERS][HAND_SIZE][16];
    deal_cards(Deck, players);

    // Display hands
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Player %d's hand: ", i + 1);
        display_hand(players[i]);
    }

    // Main game loop
    int current_player = 0;
    while (!is_game_over(players[current_player])) {
        char top_card[16];
        strcpy(top_card, Deck[0]); // Initialize top card
        
        // Display current top card
        printf("Top card: %s\n", top_card);

        // Player's turn logic
        player_turn(top_card, players[current_player]);

        // Check for game over condition
        if (is_game_over(players[current_player])) {
            printf("Player %d wins!\n", current_player + 1);
            break;
        }

        // Move to the next player
        current_player = (current_player + 1) % NUM_PLAYERS;
    }

    // Close all client sockets
    for (int i = 0; i < MAX_CLIENTS; i++) {
        close(client_sockets[i]);
    }

    return 0;
}
