#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "BSVars.h"

#define BOARD_SIZE  10
#define UNIT_SIZE   1
#define EASY_BOATS	5
#define MED_BOATS	4
#define HARD_BOATS	3

#define NORM  		"\x1B[0m"
#define RED  		"\x1B[31m"
#define GRN  		"\x1B[32m"
#define YEL  		"\x1B[33m"
#define BLUE  		"\x1B[34m"
#define MAG  		"\x1B[35m"
#define CYAN  		"\x1B[36m"
#define WHT  		"\x1B[37m"

typedef unsigned int uint;

char *get_input(char *msg);
int verify_choice(char *inp);
void print_board(Tile **brd);
void game_loop();
void game_over();
void clear_board(Tile **board);
void randomize_board(Tile **board, int boats[], int len);
Tile **make_board();
