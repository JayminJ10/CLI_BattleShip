#include "BSLib.h"

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

	game_loop();
}

Tile **make_board() {
	Tile **brd = (Tile**)malloc(BOARD_SIZE*UNIT_SIZE * BOARD_SIZE * sizeof(Tile));
	for(int i=0; i<BOARD_SIZE; i++) {
		brd[i] = (Tile*)malloc(BOARD_SIZE*UNIT_SIZE * sizeof(Tile));
	}
	return brd;
}

void clear_board(Tile **board) {
	for (int i=0; i<BOARD_SIZE; i++) {
		for (int j=0; j<BOARD_SIZE*UNIT_SIZE; j++) {
			board[i][j].indi = i, board[i][j].indj = j;
			board[i][j].sym = '*';
			board[i][j].color = NORM;
			board[i][j].id = 1;
		}
	}
}

void randomize_board(Tile **board, int boats[], int len) {
	uint horizontal = 0;
	horizontal ^= rand() % 2;
	int rand1, rand2 = 0;
	for(int i=0; i<len; i++) {
		rand1 = (int)(rand() % BOARD_SIZE);
		rand2 = (int)(rand() % BOARD_SIZE);
		while(board[rand1][rand2].sym == '#') {
			rand1 = (int)(rand() % BOARD_SIZE);
			rand2 = (int)(rand() % BOARD_SIZE);
		}

		board[rand1][rand2].sym = '#';
		Tile *first = &board[rand1][rand2];
		for(int j=1; j<boats[i]; j++) {
			Tile *curr = &board[rand1][rand2];
			
			if(horizontal) {
				if(rand2+boats[i] < (BOARD_SIZE*UNIT_SIZE)-1) curr = &board[rand1][rand2+j];
				else curr = &board[rand1][rand2-j];
			}
			else {
				if(rand1+boats[i] < BOARD_SIZE-1) curr = &board[rand1+j][rand2];
				else curr = &board[rand1-j][rand2];
			}
			
			switch(boats[i]) {
				case 2:
					first->id = 2;
					first->color = BLUE;
					curr->id = 2;
					curr->sym = '#';
					curr->color = BLUE;
					break;
				case 3:
					first->id = 3;
					first->color = GRN;
					curr->id = 3;
					curr->sym = '#';
					curr->color = GRN;
					break;
				case 4:
					first->id = 4;
					first->color = MAG;
					curr->id = 4;
					curr->sym = '#';
					curr->color = MAG;
					break;
				case 5:
					first->id = 5;
					first->color = YEL;
					curr->id = 5;
					curr->sym = '#';
					curr->color = YEL;
					break;
			}

		}
		horizontal ^= 1;
	}
}

void print_board(Tile **brd) {
	printf(
		"    A  B  C  D  E  F  G  H  I  J\n"
		"   ------------------------------"
	      );

	for (int i=0; i<BOARD_SIZE; i++) {
		printf("%s", NORM);
		printf(" \n%d |", i);
		for (int j=0; j<BOARD_SIZE*UNIT_SIZE; j++) {
			if (j == 0) printf(" ");
			else printf("  ");
			printf("%s", brd[i][j].color);
			printf("%c", brd[i][j].sym);
		}
		printf(NORM);
		printf(" | \n");
	}


	printf("   ------------------------------\n");
	printf("\n");
}

void game_loop() {
	int lose = 0;
	while (!lose) {
		// Print board
		printf("PLAYER:\n");
		print_board(board);
		// Get input
		char *choice = get_input("Enter your choice: ");
		break;
	}
	return;
}

char *get_input(char *msg) {
	char buff[128];
	char choice[128];

	char *format_error = "Invalid choice. Please input a letter followed by a number.\nEx: A0";
	char *choice_error = "Invalid choice. Only letters [A-J OR a-j] and numbers [0-9] are allowed.";

	printf("%s", msg);
	fgets(buff, 128, stdin);
	strcpy(choice, buff);
	printf("\n");

	int ver = verify_choice(choice);
	while(ver != 1) {
		if(ver == 1) break;
		else if(ver == -1) printf("%s\n", format_error);
		else if(ver == -2) printf("%s\n", choice_error);
		
		printf("%s", msg);
		fgets(buff, 128, stdin);
		strcpy(choice, buff);
		printf("\n");	

		ver = verify_choice(choice);
	}

	return choice;
}

int verify_choice(char *inp) {
	if(strlen(inp) != 3) {
		return -1;
	}
	char letter = 'A';
	int num = 0;
	if(!isalpha(inp[0]) || !isdigit(inp[1])) return -1;
	sscanf(inp, "%c%i", &letter, &num);
	if(letter < 'A' || (letter > 'J' && letter < 'a') || letter > 'j') {
		return -2;
	}

	return 1;
}
