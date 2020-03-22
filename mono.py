#!/usr/bin/env python3
"""
@file      mono-py.py
@brief     A Monopoly game simulator.

@author    Evan Elias Young
@date      2020-01-09
@date      2020-03-21
@copyright Copyright 2020 Evan Elias Young. All rights reserved.
"""

import secrets
import random
from copy import deepcopy
from typing import List, Dict

# The number of games to simulate.
GAMES: int = 10000
# The number of moves per player per game.
MOVES: int = 40
# The number of spaces on the board.
SPACES: int = 40
# The number of chance and chest cards.
CARDS: int = 15
# Where the chance cards send the player.
chance_cards: List[int] = [
    -1, -1, -1, -1, -1, -1, -5, -5, -3, 0, 5, 10, 11, 24, 39
]
# Where the community chest cards send the player.
chest_cards: List[int] = [
    0, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1, -1, -1
]
# The properties and the number of turn lands.
lands: Dict[str, int] = {
    'GO': 0,
    'Mediterranean Avenue': 0,
    'Community Chest 1': 0,
    'Baltic Avenue': 0,
    'Income Tax': 0,
    'Reading Railroad': 0,
    'Oriental Avenue': 0,
    'Chance 1': 0,
    'Vermont Avenue': 0,
    'Connecticut Avenue': 0,
    'Jail': 0,
    'St. Charles Place': 0,
    'Electric Company': 0,
    'States Avenue': 0,
    'Virginia Avenue': 0,
    'Pennsylvania Railroad': 0,
    'St. James Place': 0,
    'Community Chest 2': 0,
    'Tennessee Avenue': 0,
    'New York Avenue': 0,
    'Free Parking': 0,
    'Kentucky Avenue': 0,
    'Chance 2': 0,
    'Indiana Avenue': 0,
    'Illinois Avenue': 0,
    'B&O Railroad': 0,
    'Atlantic Avenue': 0,
    'Ventnor Avenue': 0,
    'Water Works': 0,
    'Marvin Gardens': 0,
    'Go To Jail': 0,
    'Pacific Avenue': 0,
    'North Caroline Avenue': 0,
    'Community Chest 3': 0,
    'Pennsylvania Avenue': 0,
    'Short Line': 0,
    'Chance 3': 0,
    'Park Place': 0,
    'Luxury Tax': 0,
    'Boardwalk': 0
}
# The properties and the number of turn ends.
ends: Dict[str, int] = deepcopy(lands)
# The list of space names.
space_list = list(lands.keys())

for k in range(GAMES):
    # The player's position
    pl: int = 0
    # The number of turns the player has been in jail.
    jail: int = 0
    # The number of doubles the player has rolled in a row.
    dbl: int = 0
    # The first die roll.
    d1: int
    # The second die roll.
    d2: int
    # The move sum.
    mv: int

    # Shuffle the chance cards.
    random.shuffle(chance_cards)
    # Shuffle the community chest cards.
    random.shuffle(chest_cards)
    for i in range(MOVES):
        d1, d2 = [secrets.randbelow(6) + 1, secrets.randbelow(6) + 1]
        mv = d1 + d2

        if jail == 0 or d1 == d2:
            pl = (pl + mv) % SPACES
            lands[space_list[pl]] += 1
            if pl == 7 or pl == 22 or pl == 36:
                ch = chance_cards.pop(0)
                chance_cards.append(ch)
                if ch != -1:
                    if ch == -3:
                        pl -= 3
                    elif ch == -5:
                        pl = ((round(pl / 10) * 10) + 5) % SPACES
                    else:
                        pl = ch
                        if ch == 10:
                            jail = 2
            if pl == 2 or pl == 17 or pl == 33:
                ch = chest_cards.pop(0)
                chest_cards.append(ch)
                if ch != -1:
                    pl = ch
                    if ch == 10:
                        jail = 2
            if pl == 30:
                pl = 10
                jail = 2
        else:
            lands[space_list[pl]] += 1
            jail -= 1

        if d1 == d2:
            if dbl == 2:
                pl = 10
                jail = 2
            else:
                dbl += 1
        else:
            dbl = 0

        ends[space_list[pl]] += 1

print('MONO-PY')
print(f'RUNNING {GAMES} GAMES')
print(f'PLAYING {MOVES} MOVES')
print('DONE!')
print('Lands')
[print(lands[k]) for k in lands]
print('Ends')
[print(ends[k]) for k in ends]
