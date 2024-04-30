#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NUM_PLAYERS 2
#define HAND_SIZE 7
#define DECK_SIZE 108

// Define colors and values for Uno cards
typedef enum {
    RED, YELLOW, GREEN, BLUE
} Color;

typedef struct {
    Color color;
    int value;
} Card;

// Define player structure
typedef struct {
    Card hand[HAND_SIZE];
    int hand_size;
} Player;

// Function to initialize the Uno deck
void init_deck(Card deck[]) {
    int index = 0;

    for (Color c = RED; c <= BLUE; c++) {
        for (int value = 0; value <= 9; value++) {
            deck[index].color = c;
            deck[index].value = value;
            index++;
        }
    }

    // Add special cards (skip, reverse, draw 2, draw 4)
    char special_card[4] = ("skip", "reverse", "draw2", "draw4");
    for (Color c = RED; c <= Blue; c++){
        for(int i = 0; i <= 3; i++){
            deck[index].color = c;
            deck[index].value = special_card[i];
        }
    }

    // Implement this part based on Uno rules
}

// Function to shuffle the Uno deck
void shuffle_deck(Card deck[]) {
    srand(time(NULL));
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

// Function to deal cards to players
void deal_cards(Card deck[], Player players[]) {
    int card_index = 0;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = deck[card_index];
            card_index++;
        }
        players[i].hand_size = HAND_SIZE;
    }
}

// Function to deal first move
/*
void firstmove_die(){
    for(int i = 0; i < 1; i++){
        int die = rand() % (6 + 1 -0) + 0;
    }
}
*/
int main() {
    Card deck[DECK_SIZE];
    Player players[NUM_PLAYERS];

    // Initialize deck and shuffle
    init_deck(deck);
    shuffle_deck(deck);

    // Deal cards to players
    deal_cards(deck, players);

    // Game loop
    bool game_over = false;
    int current_player = 0;

    while (!game_over) {
        // Player's turn logic
        // Implement game rules and turn logic here

        // Check for Uno condition and end of game
        // Set game_over flag to true when the game ends
    }

    // Determine the winner and display the result
    // Close network connection

    return 0;
}