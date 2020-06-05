#ifndef TYPES_H
#define TYPES_H

#define NUMBER_OF_LINES 5
#define BROKEN_TILES_LENGTH 7
#define NUMBER_OF_PLAYERS 2
#define NUMBER_OF_COLOURS 5
#define NUMBER_OF_EACH_COLOUR 20
#define FACTORY_SIZE 4
#define NUM_FACTORIES 5

#define SAVE_FILE_LINES_LENGTH 55

enum TileType
{
    RED = 'R',
    YELLOW = 'Y',
    DARKBLUE = 'B',
    LIGTHBLUE = 'L',
    BLACK = 'U',
    ORANGE = 'O',
    FIRSTPLAYER = 'F',
    NOTILE = '.'
};

#endif