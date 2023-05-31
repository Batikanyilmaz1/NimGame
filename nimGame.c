#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Define constants for the board size
#define MAX_ROWS 4
#define MAX_MATCHSTICKS 7

// Define the Board struct with an array of integers for rows
struct Board {
    int rows[MAX_ROWS];
};

// Define the Player struct with a character array for the player name
struct Player {
    char name[20];
};

// Initialize the board by setting each row's value to 2 times the index plus 1
void init_board(struct Board* board) {
    int i, j;
    for (i = 0; i < MAX_ROWS; i++) {
        board->rows[i] = 2 * i + 1;
    }
}

// Display the contents of the board by iterating over each row and printing the row number and the letter "I" for each element in the row
void display_board(struct Board* board) {
    int i, j;
    for (i = 0; i < MAX_ROWS; i++) {
        printf("Row %d: ", i + 1); // Print the row number
        for (j = 0; j < board->rows[i]; j++) {
            printf("I "); // Print the letter "I" for each matchstick in the row
        }
        printf("\n"); // Move to the next line after printing all the matchsticks in the row
    }
}
// Prompts the user to enter a valid row number and returns it.
// A valid row number is between 1 and MAX_ROWS (inclusive), and the corresponding row on the board must contain at least one matchstick.
int get_valid_row(struct Board* board) {
    int row;
    do {
        printf("Enter row number (1-%d): ", MAX_ROWS);
        scanf("%d", &row);
    } while (row < 1 || row > MAX_ROWS || board->rows[row - 1] == 0);
    return row;
}

// Prompts the user to enter a valid number of matchsticks to remove from a given row on the board.
// The row number is passed as an argument, and the corresponding row on the board must contain at least one matchstick.
// The function returns the number of matchsticks entered by the user.
int get_valid_sticks(struct Board* board, int row) {
    int sticks;
    do {
        printf("Enter number of sticks to remove (1-%d): ", board->rows[row - 1]);
        scanf("%d", &sticks);
    } while (sticks < 1 || sticks > board->rows[row - 1]);
    return sticks;
}

// Removes a given number of matchsticks from a specified row on the board.
// The row number and number of matchsticks to remove are passed as arguments.
void remove_sticks(struct Board* board, int row, int sticks) {
    board->rows[row - 1] -= sticks;
}

// Checks if the game is over by summing up the number of matchsticks in all rows of the board.
// If the sum is zero, the game is over and the function returns 1. Otherwise, the game is not over and the function returns 0.
int game_over(struct Board* board) {
    int i, sum = 0;
    for (i = 0; i < MAX_ROWS; i++) {
        sum += board->rows[i];
    }
    return sum == 0;
}


int computer_get_row(struct Board* board) {
    srand(time(NULL)); // Initialize random number generator with the current time
    int row;
    do {
        row = rand() % MAX_ROWS + 1; // Generate a random row number between 1 and MAX_ROWS (inclusive)
    } while (board->rows[row - 1] == 0); // Loop until a non-empty row is selected
    return row; // Return the selected row number
}



int computer_get_sticks(struct Board* board) {
    srand(time(NULL)); // Initialize random number generator with the current time
    int row;
    do {
        row = rand() % MAX_ROWS + 1; // Generate a random row number between 1 and MAX_ROWS (inclusive)
    } while (board->rows[row - 1] == 0); // Loop until a non-empty row is selected
    
    int nim_sum = 0;
    int sticks = 0;


    // Calculate the Nim-sum
    for (int i = 0; i < MAX_ROWS; i++) {
        nim_sum ^= board->rows[i];
    }

    // If the nim-sum is zero, remove one stick from the current row
    if (nim_sum == 0) {
        sticks = 1;
    }
    // Otherwise, change the nim-sum to zero
    else {
        for (int i = 0; i < MAX_ROWS; i++) {
            if (board->rows[i] > 0) {
                int new_nim_sum = nim_sum ^ board->rows[i] ^ (board->rows[i] - 1);
                if (new_nim_sum == 0) {
                    sticks = board->rows[i] - 1;
                    break;
                }
            }
        }
    }

    // Make sure the number of sticks is valid for the selected row
    if (sticks > board->rows[row - 1]) {
        sticks = board->rows[row - 1];
    }

    // Ensure that the number of sticks chosen is at least 1
    if (sticks < 1) {
        sticks = 1;
    }

    return sticks;
}

// This function plays the game by taking in a board and two players, and then alternating turns between the players until the game is over.
void play_game(struct Board* board, struct Player* p1, struct Player* p2) {
    // Set the current player to player 1 to begin with
    struct Player* current_player = p1;

    int row, sticks;

    // Keep looping until the game is over
    while (!game_over(board)) {

    // Print out the current player's turn and display the current state of the board
    printf("\n%s's turn:\n", current_player->name);
    display_board(board);

    // Get a valid row and number of sticks to remove from that row
    row = get_valid_row(board);
    sticks = get_valid_sticks(board, row);

    // Remove the sticks from the board
    remove_sticks(board, row, sticks);

    // Switch to the other player for the next turn
        if (current_player == p1) {
        current_player = p2;
        } else {
        current_player = p1;
     }
}

    // Print out the winner of the game
    printf("\n%s wins!\n", current_player->name);
}


// This function allows the user to play a game against the computer.
void play_computer_game() {
    // Initialize the board and the player and computer structs
    struct Board board;
    struct Player player, computer;

    // Ask the user for their name and set the computer's name to "Computer"
    printf("Enter your name: ");
    scanf("%s", player.name);
    strcpy(computer.name, "Computer");

    // Initialize the board and seed the random number generator
    init_board(&board);
    srand(time(NULL));

    // Set the current player to the user to start with
    struct Player* current_player = &player;

    int row, sticks;

    // Keep looping until the game is over
    while (!game_over(&board)) {

        // Print out the current player's turn and display the current state of the board
        printf("\n%s's turn:\n", current_player->name);
        display_board(&board);

        // Get the row and number of sticks to remove from the board based on whether the current player is the user or the computer
        if (current_player == &player) {
            row = get_valid_row(&board);
            sticks = get_valid_sticks(&board, row);
        } else {
            row = computer_get_row(&board);
            sticks = computer_get_sticks(&board);
            printf("Computer removes %d stick(s) from row %d\n", sticks, row);
        }

        // Remove the sticks from the board
        remove_sticks(&board, row, sticks);

        // Switch to the other player for the next turn
        if (current_player == &player) {
            current_player = &computer;
        } else {
         current_player = &player;
        }
    }


    // Display the final state of the board and print out the winner
    display_board(&board);
    if (current_player == &player) {
        printf("\nCongratulations, you win!\n");
    } else {
        printf("\nSorry, the computer wins!\n");
    }
}

// This function allows two players to play a game against each other
void play_two_player_game() {
    struct Board board;
    struct Player p1, p2;
    // Ask the user for the names of the two players
    printf("Enter Player 1's name: ");
    scanf("%s", p1.name);
    printf("Enter Player 2's name: ");
    scanf("%s", p2.name);
    init_board(&board); // Initialize the board
    play_game(&board, &p1, &p2); // Call the play_game function with the initialized board and player structs
}

// This function displays the menu options for the user
void display_menu() {
    printf("1. Play two player game\n");
    printf("2. Play against computer\n");
    printf("3. Quit\n");
}

// main function
int main() {
    int choice;
    do {
        display_menu(); // Call the display_menu function
        printf("Enter your choice (1-3): "); // Ask the user for the choice
        scanf("%d", &choice);
        switch (choice) { // Switch case to call proper function according to users choice
            case 1:
                play_two_player_game();
                break;
            case 2:
                play_computer_game();
                break;
            case 3:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
    } while (choice != 3);
    return 0;
}

