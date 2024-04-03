#include "names.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//
// Determines whether or not to continue the game based on if there is exactly one player alive.
//
// lives[]: The array containing the number of lives each player has.
// players: The number of players in the game
//
bool game_continue(uint32_t lives[], int players) {
    int count = 0;
    for (int i = 0; i < players; i++) {
        if (lives[i] != 0) {
            count++;
        }
    }
    return count != 1;
}

//
// Returns a random number from 0-5
//
int roll_dice() {
    int r = random();
    return r % 6;
}

//
// Taken from asgn1.pdf, the assignment specification.
// Returns the index of the player to the left.
//
// pos: The index of the current player.
// players: The number of players in the game.
//
int left(int pos, int players) {
    return (pos + players - 1) % players;
}

//
// Taken from asgn1.pdf, the assignment specification.
// Returns the index of the player to the right.
//
// pos: The index of the current player.
// players: The number of players in the game.
//
int right(int pos, int players) {
    return (pos + 1) % players;
}

int main(void) {
    int num_vampires; // The number of players in the game.
    int random_seed; // The random seed for generating dice rolls.

    // User input to set num_vampires.
    printf("Number of players: ");
    scanf("%d", &num_vampires);

    // Error handling for num_vampires.
    if (num_vampires < 2 || num_vampires > 10) {
        fprintf(stderr, "Invalid number of players.\n");
        return 0;
    }

    // User input for random_seed.
    printf("Random seed: ");
    scanf("%d", &random_seed);

    // Error handling for random_seed.
    if (random_seed < 0 || random_seed > 2147483647 || getchar() != '\n') {
        fprintf(stderr, "Invalid random seed.\n");
        return 0;
    }

    srandom(random_seed); // Sets random seed.

    const char *vampires[num_vampires]; // Array of players' names.
    uint32_t lives[num_vampires]; // Array containing number of lives for each player.

    // Sets all players' lives to 3.
    for (int i = 0; i < num_vampires; i++) {
        vampires[i] = names[i];
        lives[i] = 3;
    }

    int round = 1; // Round number.
    int round_rolls[num_vampires]; // Array containing the rolls of each player for one round.

    // Main loop for the game. Continues until there is exactly one survivor.
    while (game_continue(lives, num_vampires)) {
        printf("Round %d\n", round); // Prints round number.

        // Loops through vampires[]
        for (int i = 0; i < num_vampires; i++) {
            if (lives[i] != 0) {
                int first = roll_dice(); // Roll for first dice.
                int second = roll_dice(); // Roll for second dice.
                round_rolls[i]
                    = first + second; // Store the roll's combined total into round_rolls[].
                printf(" - %s rolls %s...", vampires[i],
                    rolls[first][second]); // Prints the name of player and their roll.

                // Checks to see if the roll was a Midnight (2 sixes) and prints resurrected and sparkled players accordingly.
                if (round_rolls[i] == 10) {
                    int left_pos = left(i, num_vampires);
                    int right_pos = right(i, num_vampires);
                    if (lives[left_pos] == 0) {
                        printf(" resurrects %s", vampires[left_pos]);
                    } else {
                        printf(" sparkles %s", vampires[left_pos]);
                    }
                    if (lives[right_pos] == 0) {
                        printf(" resurrects %s", vampires[right_pos]);
                    } else {
                        printf(" sparkles %s", vampires[right_pos]);
                    }
                }
                printf("\n");
            }
        }

        int low_index; // The index of the lowest roll of the current round.

        // Sets low_index to the first alive player's roll.
        for (int i = 0; i < num_vampires; i++) {
            if (lives[i] != 0) {
                low_index = i;
                break;
            }
        }

        for (int i = 0; i < num_vampires; i++) {

            // If a player rolled 2 sixes in the current round, increment the lives of the players to the left and right of the player who rolled the 2 sixes.
            if (round_rolls[i] == 10) {
                lives[left(i, num_vampires)] += 1;
                lives[right(i, num_vampires)] += 1;
            }

            // Checks to see if the next alive player's roll is lower than the roll at the current low_index.
            if (lives[i] != 0) {
                if (round_rolls[i] < round_rolls[low_index]) {
                    low_index = i;
                }
            }
        }

        // Decrements the lives of the player with the lowest roll of the round.
        lives[low_index]--;
        printf("%s is forced to eat garlic!\n", vampires[low_index]);

        // If the player has run out of lives, announce their death. Else, print their remaining lives.
        if (lives[low_index] == 0) {
            printf("%s has died.\n", vampires[low_index]);
        } else {
            if (lives[low_index] != 1) {
                printf(
                    "%s has %" PRId32 " lives remaining.\n", vampires[low_index], lives[low_index]);
            } else {
                printf("%s has 1 life remaining.\n", vampires[low_index]);
            }
        }

        round++; // Increment round.
    }

    // Announce winner of the Garlic Game.
    for (int i = 0; i < num_vampires; i++) {
        if (lives[i] != 0) {
            printf("%s wins the Garlic Game!\n", vampires[i]);
            break;
        }
    }

    return 0;
}
