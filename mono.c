/**
 *  @file      main.c
 *  @brief     A Monopoly game simulator.
 *
 *  @author    Evan Elias Young
 *  @date      2020-03-20
 *  @date      2020-03-21
 *  @copyright Copyright 2020 Evan Elias Young. All rights reserved.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef GAMES
// The number of games to simulate.
#define GAMES 10000
#endif // GAMES

#ifndef MOVES
// The number of moves per player per game.
#define MOVES 40
#endif // MOVES

#ifndef SPACES
// The number of spaces on the board.
#define SPACES 40
#endif // SPACES

#ifndef CARDS
// The number of chance and chest cards.
#define CARDS 15
#endif // CARDS

// Where the chance cards send the player.
char chance_cards[CARDS] = {-1, -1, -1, -1, -1, -1, -5, -5, -3, 0, 5, 10, 11, 24, 39};

// Where the community chest cards send the player.
char chest_cards[CARDS] = {0, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1};

// The properties and the number of turn lands.
long turn_lands[SPACES] = {0};

// The properties and the number of turn ends.
long turn_ends[SPACES] = {0};

/**
 * @brief Swaps two elements in an array.
 *
 * @param array The array.
 * @param i The index of the first element.
 * @param j The index of the second element.
 */
void array_swap(char array[], const size_t i, const size_t j)
{
  array[i] = array[i] ^ array[j];
  array[j] = array[j] ^ array[i];
  array[i] = array[i] ^ array[j];
}

/**
 * @brief Shuffles an array.
 *
 * @param array The array.
 * @param n The number of items in the array.
 */
void array_shuffle(char array[], const size_t n)
{
  char t;
  if (n > 1)
  {
    for (size_t i = 0; i < n - 1; ++i)
    {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      array_swap(array, i, j);
    }
  }
}

/**
 * @brief Rolls one die.
 *
 * @return char The value rolled.
 */
char roll_die()
{
  return 1 + rand() % 6;
}

/**
 * @brief Rolls two die and stores the sum.
 *
 * @param d1 The first die.
 * @param d2 The second die.
 * @param mv The sum.
 */
void roll_dice(char *d1, char *d2, char *mv)
{
  *d1 = roll_die();
  *d2 = roll_die();
  *mv = *d1 + *d2;
}

/**
 * @brief Draws the next card from the pile.
 *
 * @param pile The pile to draw from.
 * @return char The drawn card.
 */
char draw_card(char pile[])
{
  char tmp = pile[0];
  for (size_t i = 0; i < CARDS - 1; ++i)
  {
    pile[i] = pile[i + 1];
  }
  pile[CARDS - 1] = tmp;

  return pile[CARDS - 1];
}

/**
 * @brief Runs one game of Monopoly.
 */
void run_game()
{
  // The player's position
  char pl = 0;
  // The number of turns the player has been in jail.
  char jail = 0;
  // The number of doubles the player has rolled in a row.
  char dbl = 0;
  // The first die roll.
  char d1 = 0;
  // The second die roll.
  char d2 = 0;
  // The move sum.
  char mv = 0;
  // The destination of the chance or chest card.
  char ch = 0;

  // Shuffle the chance cards.
  array_shuffle(chance_cards, CARDS);
  // Shuffle the community chest cards.
  array_shuffle(chest_cards, CARDS);
  for (size_t i = 0; i < MOVES; ++i)
  {
    // Roll the dice.
    roll_dice(&d1, &d2, &mv);

    if (jail == 0 || d1 == d2)
    { // No turns left in jail, or rolled doubles.
      pl = (pl + mv) % SPACES;
      turn_lands[pl]++;
      if (pl == 7 || pl == 22 || pl == 36)
      { // Landed on a chance spot.
        ch = draw_card(chance_cards);
        if (ch != -1)
        { // -1 means a card does nothing.
          if (ch == -3)
          { // Move the player back 3 spaces.
            pl -= 3;
          }
          else if (ch == -5)
          { // Move the player to the next railroad.
            pl = (((int)(round((float)(pl) / 10)) * 10) + 5) % SPACES;
          }
          else
          { // Move the player to the destination.
            pl = ch;
            if (ch == 10)
            { // The player moved to jail.
              jail = 2;
            }
          }
        }
      }
      if (pl == 2 || pl == 17 || pl == 33)
      { // Landed on a chest spot.
        ch = draw_card(chest_cards);
        if (ch != -1)
        { // -1 means a card does nothing.
          pl = ch;
          if (ch == 10)
          { // The player moved to jail.
            jail = 2;
          }
        }
      }
      if (pl == 30)
      { // The player moved to jail.
        pl = 10;
        jail = 2;
      }
    }
    else
    { // Still in jail.
      turn_lands[pl]++;
      jail--;
    }

    if (d1 == d2)
    { // Rolled doubles.
      if (++dbl == 3)
      { // Rolled three doubles in a row.
        pl = 10;
        jail = 2;
      }
    }
    else
    { // Didn't roll doubles.
      dbl = 0;
    }
    turn_ends[pl]++;
  }
}

/**
 * @brief The main point of entry for Monopoly.
 *
 * @param argc The number of arguments.
 * @param argv The values of arguments.
 * @return int The exit code.
 */
int main(int argc, char const *argv[])
{
  // Seed the random number generator.
  srand(time(NULL));

  printf("MONO-C\n");
  printf("RUNNING %d GAMES\n", GAMES);
  printf("PLAYING %d MOVES\n", MOVES);

  // Run the main loop for the simulations.
  for (size_t i = 0; i < GAMES; ++i)
  {
    run_game();
  }

  printf("DONE!\n");
  printf("LANDS\n");
  for (size_t i = 0; i < SPACES; ++i)
  {
    printf("%d\n", turn_lands[i]);
  }
  printf("ENDS\n");
  for (size_t i = 0; i < SPACES; ++i)
  {
    printf("%d\n", turn_ends[i]);
  }

  return 0;
}
