#include "../inc/BSUtils.h"
#include <pthread.h>

Tile **board;
Tile **cpu_board;

int blist[5] = {2, 3, 3, 4, 5};

void main(int argc, char **argv) {
	//pthread_t serv;
    char buffer[128];
	char horj = '.';
	char choice[128];
	srand(time(NULL));

	printf("Host or Join? [H]/[J] ");
	fgets(buffer, 128, stdin);
	printf("\n");
	sscanf(buffer, "%c", &horj);

	if(/*!strcmp((char)tolower(horj), "h")*/ horj == 'h') {
		char buf[128];
		char cpu_play[128];
		char choice[128];

		// Ask for cpu play
		printf("Play with CPU? [y]/[n]\n");
		fgets(buf, 128, stdin);
		sscanf(buf, "%s", cpu_play);
		// If yes, generate cpu board
		if(!strncmp(cpu_play, "y", 1)) {
			printf("yep\n");
			cpu_board = make_board();
			clear_board(cpu_board);
			randomize_board(cpu_board, blist, EASY_BOATS);
		}

		// Make player board
// !!TODO - Get player input to set pieces.
		board = make_board();
		clear_board(board);
		randomize_board(board, blist, EASY_BOATS);

		/* Server related:*/
		// OPTIONAL: Start up server on a seperate thread
		//pthread_create(&serv, NULL, server, NULL);

		server(NULL);
	}
	else if(horj == 'j') {
		printf("Enter port to connect to: ");
		char *port = get_input();
		client(atoi(port));
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
		char *choice = get_input_pt_choice("Enter your choice: ");
		break;
	}
	return;
}