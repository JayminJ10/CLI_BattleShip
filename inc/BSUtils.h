#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "BSTypes.h"

#define BOARD_SIZE  10
#define UNIT_SIZE   1
#define EASY_BOATS	5
#define MED_BOATS	4
#define HARD_BOATS	3

// Colors for CLI output design
#define NORM  		"\x1B[0m"
#define RED  		"\x1B[31m" // hit
#define GRN  		"\x1B[32m" // boat_3
#define YEL  		"\x1B[33m" // boat_5
#define BLUE  		"\x1B[34m" // boat_2
#define MAG  		"\x1B[35m" // boat_4
#define CYAN  		"\x1B[36m"
#define WHT  		"\x1B[37m"
#define GREY        "\033[0;37m" // miss

// Game related
char *get_input_pt_choice(char *msg);
char *get_input();
int verify_choice(char *inp);
void print_board(Tile **brd);
void game_loop(Tile **board);
void game_over();
void clear_board(Tile **board);
void update_board(Tile **board, Tile data, u8 hit, int type);
void remove_tile(Tile *t);
void clear_tile(Tile *t);
void randomize_board(Tile **board, int boats[], int len);
Tile **make_board();
