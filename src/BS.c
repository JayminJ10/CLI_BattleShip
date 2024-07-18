#include "../inc/BSUtils.h"

Tile **board;
Tile **cpu_board;

int blist_e[5] = {2, 3, 3, 4, 5};
int blist_m[4] = {2, 3, 4, 5};
int blist_h[3] = {2, 3, 4};

void main(int argc, char **argv) {
    srand(time(NULL));
	if(argc != 2){
		printf("Usage: './BattleShip *difficulty*' - ([E]asy, [M]edium, [H]ard)\n");
		exit(1);
	}
	else if(!strcmp(argv[1], "E")) {
		board = make_board();
		cpu_board = make_board();
		clear_board(board);
		clear_board(cpu_board);
		randomize_board(board, blist_e, EASY_BOATS);
		randomize_board(cpu_board, blist_e, EASY_BOATS);
	}
	else if(!strcmp(argv[1], "M")) {
		board = make_board();
		cpu_board = make_board();
		clear_board(board);
		clear_board(cpu_board);
		randomize_board(board, blist_m, MED_BOATS);
		randomize_board(cpu_board, blist_m, MED_BOATS);
	}
	else if(!strcmp(argv[1], "H")) {
		board = make_board();
		cpu_board = make_board();
		clear_board(board);
		clear_board(cpu_board);
		randomize_board(board, blist_h, HARD_BOATS);
		randomize_board(cpu_board, blist_h, HARD_BOATS);
	}
	else {
		printf("Usage: './BattleShip *difficulty*' - ([E]asy, [M]edium, [H]ard)\n");
		exit(1);
	}

	game_loop(board, cpu_board);
}

void game_loop(Tile **pboard, Tile **cpu_board) {
	int lose = 0;
	while (!lose) {
		// Print board
		printf("PLAYER:\n");
		print_board(pboard);
		// Get input
		char *choice = get_input("Enter your choice: ");
		break;
	}
	return;
}