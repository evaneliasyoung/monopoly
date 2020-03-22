
/**
 * @file mono.java
 * @brief A Monopoly simulator.
 *
 * @author Evan Elias Young
 * @date 2020-03-21
 * @date 2020-03-21
 * @copyright Copyright 2020 Evan Elias Young. All rights reserved.
 */

import java.util.*;

class IntObj {
  public int value;
}

public class mono {
  // The number of games to simulate.
  private static final long GAMES = 10000;
  // The number of moves per player per game.
  private static final int MOVES = 40;
  // The number of spaces on the board.
  private static final int SPACES = 40;

  private static final Random rng = new Random();

  // Where the chance cards send the player.
  private static ArrayList<Integer> chance_cards = new ArrayList<Integer>(
      Arrays.asList(-1, -1, -1, -1, -1, -1, -5, -5, -3, 0, 5, 10, 11, 24, 39));

  // Where the community chest cards send the player.
  private static ArrayList<Integer> chest_cards = new ArrayList<Integer>(
      Arrays.asList(0, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1));

  // The properties and the number of turn lands.
  private static long turn_lands[] = new long[SPACES];

  // The properties and the number of turn ends.
  private static long turn_ends[] = new long[SPACES];

  /**
   * Rolls two dice and stores the sum.
   *
   * @param d1 The first die.
   * @param d2 The second die.
   * @param mv The sum.
   */
  public static void roll_dice(IntObj d1, IntObj d2, IntObj mv) {
    d1.value = rng.nextInt(6) + 1;
    d2.value = rng.nextInt(6) + 1;
    mv.value = d1.value + d2.value;
  }

  /**
   * Runs one game of monopoly.
   */
  public static void run_game() {
    // The player's position
    int pl = 0;
    // The number of turns the player has been in jail.
    int jail = 0;
    // The number of doubles the player has rolled in a row.
    int dbl = 0;
    // The first die roll.
    IntObj d1 = new IntObj();
    // The second die roll.
    IntObj d2 = new IntObj();
    // The move sum.
    IntObj mv = new IntObj();
    // The destination of the chance or chest card.
    int ch = 0;

    // TODO: These functions.
    // Shuffle the chance cards.
    // array_shuffle(chance_cards, CARDS);
    // Shuffle the community chest cards.
    // array_shuffle(chest_cards, CARDS);
    for (int i = 0; i < MOVES; ++i) {
      // Roll the dice.
      roll_dice(d1, d2, mv);

      if (jail == 0 || d1.value == d2.value) {
        // No turns left in jail, or rolled doubles.
        pl = (pl + mv.value) % SPACES;
        turn_lands[pl]++;
        if (pl == 7 || pl == 22 || pl == 36) {
          // Landed on a chance spot.
          ch = chance_cards.get(0);
          Collections.rotate(chance_cards, -1);
          if (ch != -1) {
            // -1 means a card does nothing.
            if (ch == -3) {
              // Move the player back 3 spaces.
              pl -= 3;
            } else if (ch == -5) {
              // Move the player to the next railroad.
              pl = (((int) (Math.round((float) (pl) / 10)) * 10) + 5) % SPACES;
            } else {
              // Move the player to the destination.
              pl = ch;
              if (ch == 10) {
                // The player moved to jail.
                jail = 2;
              }
            }
          }
        }
        if (pl == 2 || pl == 17 || pl == 33) {
          // Landed on a chest spot.
          ch = chest_cards.get(0);
          Collections.rotate(chest_cards, -1);
          if (ch != -1) {
            // -1 means a card does nothing.
            pl = ch;
            if (ch == 10) {
              // The player moved to jail.
              jail = 2;
            }
          }
        }
        if (pl == 30) {
          // The player moved to jail.
          pl = 10;
          jail = 2;
        }
      } else {
        // Still in jail.
        turn_lands[pl]++;
        jail--;
      }

      if (d1.value == d2.value) {
        // Rolled doubles.
        if (++dbl == 3) {
          // Rolled three doubles in a row.
          pl = 10;
          jail = 2;
        }
      } else {
        // Didn't roll doubles.
        dbl = 0;
      }
      turn_ends[pl]++;
    }
  }

  /**
   * The main point of entry for Monopoly.
   *
   * @param args The arguments.
   */
  public static void main(String[] args) {
    System.out.println("MONO-JAVA");
    System.out.println(String.format("RUNNING %s GAMES", GAMES));
    System.out.println(String.format("PLAYING %s MOVES", MOVES));

    // Run the main loop for the simulations.
    for (int i = 0; i < GAMES; ++i) {
      run_game();
    }

    System.out.println("DONE!");
    System.out.println("LANDS");
    for (int i = 0; i < SPACES; i++) {
      System.out.println(turn_lands[i]);
    }
    System.out.println("ENDS");
    for (int i = 0; i < SPACES; i++) {
      System.out.println(turn_ends[i]);
    }
  }
}
