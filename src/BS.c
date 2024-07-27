#include "../inc/BSNet.h"
#include <pthread.h>

Tile **p1board;
Tile **p2board;

int blist[5] = {2, 3, 3, 4, 5};

void main(int argc, char **argv) {
	pthread_t serv;
    char buffer[128];
	char horj = '.';
	char choice[128];
	srand(time(NULL));

	printf("Host or Join? [H]/[J] ");
	fgets(buffer, 128, stdin);
	printf("\n");
	sscanf(buffer, "%c", &horj);

	char buf[128];
	char cpu_play[128];

	if(horj == 'h' || horj == 'H') {
		p1board = make_board();
		clear_board(p1board);
		randomize_board(p1board, blist, EASY_BOATS);

		server_game_loop(p1board);
	}
	else if(horj == 'j' || horj == 'J') {
		printf("Enter port to connect to: ");
		char *port = get_input();

		p2board = make_board();
		clear_board(p2board);
		randomize_board(p2board, blist, EASY_BOATS);

		client_game_loop(atoi(port), p2board);
	}
	else {
		perror("Invalid choice.\n");
		exit(1);
	}
}