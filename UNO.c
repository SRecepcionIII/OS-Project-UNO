#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define NUM_PLAYERS 2

int HAND_SIZE = 7;
int DECK_SIZE = 40;

char *Color[4] = {"Red", "Yellow" , "Green", "Blue"};
char *Value[16] = {"0","1","2","3","4","5","6","7","8","9","Cancel","Skip","Reverse","Wild","+2","+4"};
char Card[4][16];
char Deck[64];
char PlayerCard[7];
//char Discard[];

// Function to initialize the Uno deck
void init_deck() {
    for (int y = 0; y <= 3; y++) {
        for (int i = 0; i <= 15; i++) {
            //printf("Color: %s \n Value: %s \n\n", Color[y], Value[i]);
            Card[y][i] = {{Color[y]}, {Color[i]}};
            printf("Color: %s \n Value: %s \n\n", Card[y], Card[i]);
        }
    }
}
/*
// Function to shuffle the Uno deck
void shuffle_deck(Deck[]) {
    srand(time(NULL));
    for (int i = 0; i < DECK_SIZE; i++) {
        int j = rand() % DECK_SIZE;
        Card temp = Deck[i];
        Deck[i] = Deck[j];
        Deck[j] = temp;
    }
}

// Function to deal cards to players
void deal_cards(Card deck[], Player players[]) {
    int card_index = 1;
    for (int i =  0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < HAND_SIZE; j++) {
            players[i].hand[j] = deck[card_index];
            card_index++;
        }
        players[i].hand_size = HAND_SIZE;
    }
}

// Function to check if a player has Uno
bool has_uno(Player player) {
    return player.hand_size == 1;
}

// Function to display player's hand
void display_hand(Player player) {
    printf("Your hand: ");
    for (int i = 0; i < player.hand_size; i++) {
        printf("(%d,%d) ", player.hand[i].color, player.hand[i].value);
    }
    printf("\n");
}

// Function to check if a player's move is valid
bool is_valid_move(Card top_card, Card player_card) {
    return (top_card.color == player_card.color || top_card.value == player_card.value);
}

// Function to execute player's move
void execute_move(Card *top_card, Player *player, int move_index) {
    *top_card = player->hand[move_index];
    for (int i = move_index; i < player->hand_size - 1; i++) {
        player->hand[i] = player->hand[i + 1];
    }
    player->hand_size--;
}

// Function to handle player's turn
void player_turn(Player *player, Card *top_card) {
    printf("It's your turn!\n");
    display_hand(*player);
    printf("Enter the index of the card you want to play: ");
    int move_index;
    scanf("%d", &move_index);

    if (move_index < 0 || move_index >= player->hand_size) {
        printf("Invalid index. Please try again.\n");
        player_turn(player, top_card);
        return;
    }

    Card selected_card = player->hand[move_index];
    if (!is_valid_move(*top_card, selected_card)) {
        printf("Invalid move. Please try again.\n");
        player_turn(player, top_card);
        return;
    }

    execute_move(top_card, player, move_index);
}

void current_top(int top_color, int top_num){
    char convColor;
    
    switch (top_color)
    {
    case 0:
        convColor = "RED";
        break;
    case 1:
        convColor = "Yellow";
        break;
    case 2:
        convColor = "RED";
        break;
    case 3:
        convColor = "RED";
        break;
    default:
        break;
    }

    printf("Top card: (%s,%d)\n", convColor, top_num);
}
*/

/*
void testrun(){
    int n = 0;
    while (n != 64)
    {
        printf("Card %d \n", Deck[n]);
        n++;
    }
}
*/

int main() {


    // Initialize deck and shuffle
    init_deck();

    //testrun();

    /*
    shuffle_deck(deck);

    // Deal cards to players
    deal_cards(deck, players);

    // Initialize top card
    Card top_card = deck[0];

    // Game loop
    bool game_over = false;
    int current_player = 0;

    while (!game_over) {
        Player *current = &players[current_player];
     
        // Display current top card
        current_top(top_card.color, top.card.value);
        


        // Player's turn logic
        player_turn(current, &top_card);

        // Check for Uno condition
        if (has_uno(*current)) {
            printf("Player %d has UNO!\n", current_player + 1);
        }

        // Check for game over condition
        if (current->hand_size == 0) {
            printf("Player %d wins!\n", current_player + 1);
            game_over = true;
        }

        // Move to the next player
        current_player = (current_player + 1) % NUM_PLAYERS;
    }*/


    return 0;
}
