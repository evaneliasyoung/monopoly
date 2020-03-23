/**
 * @file      mono.js
 * @brief     A Monopoly game simulator.
 *
 * @author    Evan Elias Young
 * @date      2020-03-23
 * @date      2020-03-23
 * @copyright Copyright 2020 Evan Elias Young. All rights reserved.
 */

// The number of games to simulate.
const GAMES = 100000000

// The number of moves per player per game.
const MOVES = 40

// The number of spaces on the board.
const SPACES = 40

// The number of chance and chest cards.
const CARDS = 15

// Where the chance cards send the player.
let chance_cards = [-1, -1, -1, -1, -1, -1, -5, -5, -3, 0, 5, 10, 11, 24, 39]

// Where the community chest cards send the player.
let chest_cards = [0, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1]

// The properties and the number of turn lands.
let turn_lands = new Array(SPACES).fill(0)

// The properties and the number of turn ends.
let turn_ends = new Array(SPACES).fill(0)

/**
 * @brief Swaps two element in an array.
 * @param {list} array The arrray.
 * @param {int} i The index of the first element.
 * @param {int} j The index of the second element.
 */
function array_swap (array, i, j) {
  let tmp = array[i]
  array[i] = array[j]
  array[j] = tmp
  return array
}

/**
 * @brief Shuffles an array.
 * @param {list} array The array.
 */
function array_shuffle (array) {
  for (let i = 0; i < array.length; ++i) {
    let j = Math.floor(array.length * Math.random())
    array = array_swap(array, i, j)
  }
  return array
}

/**
 * @brief Rolls one die.
 */
function roll_die () {
  return Math.ceil(Math.random() * 6)
}

/**
 * @brief Draws the next card from the pile.
 * @param {list} pile The pile to draw from.
 */
function draw_card (pile) {
  let tmp = pile[0]
  for (let i = 0; i < CARDS - 1; ++i) {
    pile[i] = pile[i + 1]
  }
  pile[CARDS - 1] = tmp

  return pile
}

/**
 * @brief Runs one game of Monopoly.
 */
function run_game () {
  // The player's position
  let pl = 0
  // The number of turns the player has been in jail.
  let jail = 0
  // The number of doubles the player has rolled in a row.
  let dbl = 0
  // The first die roll.
  let d1 = 0
  // The second die roll.
  let d2 = 0
  // The move sum.
  let mv = 0
  // The destination of the chance or chest card.
  let ch = 0

  // Shuffle the chance cards.
  chance_cards = array_shuffle(chance_cards)
  // Shuffle the community chest cards.
  chest_cards = array_shuffle(chest_cards)
  for (let i = 0; i < MOVES; ++i) {
    // Roll the dice.
    d1 = roll_die()
    d2 = roll_die()
    mv = d1 + d2

    if (jail == 0 || d1 == d2) {
      // No turns left in jail, or rolled doubles.
      pl = (pl + mv) % SPACES
      turn_lands[pl]++
      if (pl == 7 || pl == 2 || pl == 36) {
        // Landed on a chance spot.
        chance_cards = draw_card(chance_cards)
        ch = chance_cards[0]
        if (ch != -1) {
          // -1 means a card does nothing.
          if (ch == -3) {
            // Move the player back 3 spaces.
            pl -= 3
          } else if (ch == -5) {
            // Move the player to the next railroad.
            pl = (parseInt(Math.round(pl / 10)) * 10 + 5) % SPACES
          } else {
            // Move the player to the destination.
            pl = ch
            if (ch == 10) {
              // The player moved to jail.
              jail = 2
            }
          }
        }
      }
      if (pl == 2 || pl == 17 || pl == 33) {
        // Landed on a chest spot.
        chest_cards = draw_card(chest_cards)
        ch = chest_cards[0]
        if (ch != -1) {
          // -1 means a card does nothing.
          pl = ch
          if (ch == 10) {
            // The player moved to jail.
            jail = 2
          }
        }
      }
      if (pl == 30) {
        // The player moved to jail.
        pl = 10
        jail = 2
      }
    } else {
      // Still in jail.
      turn_lands[pl]++
      jail--
    }

    if (d1 == d2) {
      // Rolled doubles.
      if (++dbl == 3) {
        // Rolled three doubles in a row.
        pl = 10
        jail = 2
      }
    } else {
      // Didn't roll doubles.
      dbl = 0
    }
    turn_ends[pl]++
  }
}

/**
 * @brief The main point of entry for Monopoly.
 */
function main () {
  console.log('MONO-JAVASCRIPT')
  console.log(`RUNNING ${GAMES} GAMES`)
  console.log(`PLAYING ${MOVES} MOVES`)

  // Run the main loop for the simulations.
  for (let i = 0; i < GAMES; i++) {
    run_game()
  }

  console.log('DONE!')
  console.log('LANDS')
  turn_lands.forEach(e => {
    console.log(e)
  })
  console.log('ENDS')
  turn_ends.forEach(e => {
    console.log(e)
  })
}

main()
