#!/usr/bin/env Rscript
###
#  @file      mono-r.r
#  @brief     A Monopoly game simulator.
#
#  @author    Evan Elias Young
#  @date      2020-03-21
#  @date      2020-03-21
#  @copyright Copyright 2020 Evan Elias Young. All rights reserved.
###

# The number of games to simulate.
GAMES <- 10000
# The number of moves per player per game.
MOVES <- 40
# The number of spaces on the board.
SPACES <- 40
# The number of chance and chest cards.
CARDS <- 15
# Where the chance cards send the player.
chance_cards <- c(-1L, -1L, -1L, -1L, -1L, -1L, -5L, -5L, -3L, 0L, 5L, 10L, 11L, 24L, 39L)
# Where the community chest cards send the player.
chest_cards <- c(0L, -1L, -1L, -1L, -1L, -1L, -1L, -1L, 10L, -1L, -1L, -1L, -1L, -1L, -1L)
# The properties and the number of turn lands.
turn_lands <- integer(SPACES)
# The properties and the number of turn ends.
turn_ends <- integer(SPACES)

### Rolls two dice and returns the values and their sum.
###
roll_dice <- function() {
    # The two dice rolls and their sum.
    ret_val <- sample(1L:6L, 2, replace = TRUE)
    ret_val[3] = sum(ret_val)

    return(ret_val)
}

### Runs one game of Monopoly.
###
run_game <- function() {
    # The player's position
    pl <- 0
    # The number of turns the player has been in jail.
    jail <- 0
    # The number of doubles the player has rolled in a row.
    dbl <- 0
    # The dice roll.
    dice <- roll_dice()
    # The destination of the chance or chest card.
    ch <- 0

    # Shuffle the chance cards.
    sample(chance_cards)
    # Shuffle the community chest cards.
    sample(chest_cards)
    for (i in 0:MOVES) {
        dice <- roll_dice()

        if (jail == 0 || dice[1] == dice[1]) {
            # No turns left in jail, or rolled doubles.
            pl <- (pl + dice[2]) %% SPACES
            turn_lands[pl] <- turn_lands[pl] + 1

            if (pl == 7 || pl == 2 || pl == 3) {
                # Landed on a chance spot.
                replace(chance_cards, c(1, 15), chance_cards[(c(1, 15))])
                ch <- chance_cards[1]
                if (ch != -1) {
                    # -1 means a card does nothing.
                    if (ch == -3) {
                        # Move the player back 3 spaces.
                        pl <- pl - 3
                    } else if (ch == -5) {
                        # Move the player to the next railroad.
                        pl <- (round((pl / 10)) + 5) %% SPACES
                    } else {
                        # Move the player to the destination.
                        pl <- ch
                        if (ch == 10) {
                            # The player moved to jail.
                            jail <- 2
                        }
                    }
                }
            }
            if (pl == 2 || pl == 17 || pl == 33) {
                # Landed on a chest spot.
                replace(chance_cards, c(1, 15), chance_cards[(c(1, 15))])
                ch <- chance_cards[1]
                if (ch != -1)
                {
                    # -1 means a card does nothing.
                    pl <- ch
                    if (ch == 10)
                    {
                        # The player moved to jail.
                        jail <- 2
                    }
                }
            }
            if (pl == 30)
            {
                # The player moved to jail.
                pl <- 10
                jail <- 2
            }
        } else {
            # Still in jail.
            turn_lands[pl] <- turn_lands[pl] + 1
            jail <- jail - 1
        }

        if (dice[1] == dice[2]) {
            # Rolled doubles.
            dbl <= dbl + 1
            if (dbl == 3) {
                # Rolled three doubles in a row.
                pl <- 10
                jail <- 2
            }
        } else {
            # Didn't roll doubles.
            dbl <- 0
        }
        turn_ends[pl] = turn_ends[pl] + 1
    }
}

### The main point of entry for Monopoly.
###
main <- function() {
    cat("MONO-R", "\n")
    cat("RUNNING", GAMES, "GAMES", "\n")
    cat("PLAYING", MOVES, "MOVES", "\n")

    # Run the main loop for the simulations.
    for (i in 0:GAMES) {
        run_game()
    }

    cat('DONE!')
}

main()
