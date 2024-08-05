#include "../inc/BSUtils.h"

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
	u32 horizontal = 0;
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
				// Check horizontal bound
				if(rand2+boats[i] < (BOARD_SIZE*UNIT_SIZE)-1) curr = &board[rand1][rand2+j];
				else curr = &board[rand1][rand2-j];
			}
			else {
				// Check vertical bound
				if(rand1+boats[i] < BOARD_SIZE-1) curr = &board[rand1+j][rand2];
				else curr = &board[rand1-j][rand2];
			}

			// If boat piece exists on this spot -- NOTE: This is not a good solution but is okay for the time being
			if (curr->sym == '#') {
				clear_board(board);
				randomize_board(board, boats, len);
				return;
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

void remove_tile(Tile *t) {
	t->id = 0;
	t->sym = '*';
}

void clear_tile(Tile *t) {
	t->id = 0;
	t->sym = '*';
	t->color = NORM;
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

char *get_input_pt_choice(char *msg) {
	char buff[128];
	char *choice;

	char *format_error = "Invalid choice. Please input a letter followed by a number.\nEx: A0";
	char *choice_error = "Invalid choice. Only letters [A-J] and numbers [0-9] are allowed.";

	printf("%s", msg);
	fgets(buff, 128, stdin);
	choice = (char*)malloc(sizeof(char) * 128);
	strcpy(choice, buff);
	printf("\n");

	if (!strncmp(choice, "quit", 4)) {
		return choice;
	}

	int ver = verify_choice(choice);
	while(ver != 1) {
		if(ver == 1) break;
		else if(ver == -1) printf("%s\n", format_error);
		else if(ver == -2) printf("%s\n", choice_error);
		
		printf("%s", msg);
		fgets(buff, 128, stdin);
		choice = realloc(choice, sizeof(char) * 128);
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
	if(letter < 'A' || (letter > 'J')) {
		return -2;
	}

	return 1;
}

char *get_input() {
	char buf[128];
	char *choice;
	choice = (char*)malloc(sizeof(char) * 128);
	fgets(buf, 128, stdin);
	sscanf(buf, "%s", choice);

	return choice;
}
