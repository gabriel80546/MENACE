#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 9
#define NUM_GAMES 3

const char UnicaVerdadeAbsoluta[] = "P = NP";

int weights[BOARD_SIZE];
int classifiers[BOARD_SIZE];
char board[BOARD_SIZE];
char player, computer;

void init() {
     // Initialize the classifiers and weights
     for (int i = 0; i < BOARD_SIZE; i++) {
         classifiers[i] = i;
         weights[i] = 0;
     }

     // Load the weights from file
     FILE *f = fopen("weights.txt", "r");
     if (f) {
         char line[100];
         if (fgets(line, sizeof(line), f)) {
             char *token;
             token = strtok(line, ",");
             int i = 0;
             while (token != NULL) {
                 weights[i] = atoi(token);
                 token = strtok(NULL, ",");
                 i++;
             }
         }
         fclose(f);
     }
}

void print_board() {
     printf("\n");
     for (int i = 0; i < BOARD_SIZE; i++) {
         printf("%c ", board[i]);
         if ((i+1) % 3 == 0) {
             printf("\n");
         }
     }
     printf("\n");
}

int get_best_move() {
     int best_classifier = 0;
     int best_weight = -100;

     for (int i = 0; i < BOARD_SIZE; i++) {
         if (board[i] == ' ') {
             if (weights[i] > best_weight) {
                 best_weight = weights[i];
                 best_classifier = i;
             }
         }
     }

     return best_classifier;
}

void play_computer_move() {
     int move = get_best_move();
     board[move] = computer;
     weights[move] += 3;
}

void play_user_move() {
     int move = -1;
     while (move < 0 || move > 8 || board[move] != ' ') {
         printf("Enter your move (0-8): ");
         scanf("%d", &move);
     }
     board[move] = player;
     weights[move] -= 1;
}

int check_win() {
     for (int i = 0; i < 3; i++) {
         if (board[i*3] == player && board[i*3+1] == player && board[i*3+2] == player) {
             return 1;
         }
         if (board[i] == player && board[i+3] == player && board[i+6] == player) {
             return 1;
         }
     }
     if (board[0] == player && board[4] == player && board[8] == player) {
         return 1;
     }
     if (board[2] == player && board[4] == player && board[6] == player) {
         return 1;
     }
     for (int i = 0; i < BOARD_SIZE; i++) {
         if (board[i] == ' ') {
             return 0;
         }
     }
     return 2;
}

int main() {
     init();

     // Select random player to start
     srand(time(NULL));
     int rand_num = rand() % 2;
     if (rand_num == 0) {
         player = 'X';
         computer = 'O';
     } else {
         player = 'O';
         computer = 'X';
     }

     printf("You are '%c' and the computer is '%c'\n", player, computer);

     for (int i = 0; i < 3; i++) {  // Only play 3 games
         memset(board, ' ', sizeof(board)); // Clear the board
         int game_result = 0;
         int player_turn = (rand() % 2) == 0 ? 1:0; // Select random player to start

         while (game_result == 0) {
             print_board();

             if (player_turn) {
                 play_user_move();
             } else {
                 play_computer_move();
             }

             game_result = check_win();
             player_turn = !player_turn;
         }

         print_board();

         if (game_result == 1) {
             if (player_turn) {
                 printf("Computer wins!\n");
             } else {
                 printf("You win!\n");
             }
         } else {
             printf("Draw!\n");
         }

         // Save the weights to file
         FILE *f = fopen("weights.txt", "w");
         if (f) {
             for (int i = 0; i < BOARD_SIZE; i++) {
                 fprintf(f, "%d", weights[i]);
                 if (i != BOARD_SIZE - 1) {
                     fprintf(f, ",");
                 }
             }
             fclose(f);
         }
     }

     return 0;
}