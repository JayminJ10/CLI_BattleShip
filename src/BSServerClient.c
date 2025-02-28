#include "../inc/BSNet.h"

void *empty = NULL;
void *ret = (void*)1;
int ssock, csock, conn;

void *server(void *unused) {
    ssock = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(ssock, (struct sockaddr*) &addr, sizeof(addr))) {
        perror("bind error:");
        return empty;
    }

    socklen_t addr_len = sizeof(addr);
    getsockname(ssock, (struct sockaddr*) &addr, &addr_len);
    printf("server is on port %d\n", (int) ntohs(addr.sin_port));

    if (listen(ssock, 1)) {
        perror("listen error:");
        return empty;
    }

    // accept incoming connection
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    csock = accept(ssock, (struct sockaddr*) &caddr, &caddr_len);
}

void *client(int port) {
    conn = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // connect to local machine at specified port
    char addrstr[NI_MAXHOST + NI_MAXSERV + 1];
    snprintf(addrstr, sizeof(addrstr), "127.0.0.1:%d", port);

    // parse into address
    inet_pton(AF_INET, addrstr, &addr.sin_addr);

    // connect to server
    if (connect(conn, (struct sockaddr*) &addr, sizeof(addr))) {
        perror("connect error:");
        return empty;
    }

    return ret;

//TODO: do threads
}

void server_game_loop(Tile **board) {
	int lose = 0;
    int boats_left = BOAT_PCS;
    server(NULL);
	while (!lose) {
		// Print board
		printf("PLAYER:\n");
		print_board(board);

        printf("YOUR MOVE\n");

        // Get input
		char letter; 
        u8 letter_val = 0;
		u8 number = 0;
        packet_t received_packet;

		char *choice = get_input_pt_choice("Enter your choice: ");
        if (!strncmp(choice, "quit", 4)) {
            packet_t ff = {.type = PACKET_QUIT};
            send(csock, &ff, sizeof(ff), 0);
            close(csock);
            close(ssock);
            exit(1);
        }
		sscanf(choice, "%c %hu", &letter, &number);
        letter_val = letter - 65;

        // Send first:
        // Send our choice to the client and await a hit response to update our board before continuing.
        packet_t move = {.type = PACKET_COORDINATE, .packet.data = board[(number)][(letter_val)]};
        send(csock, &move, sizeof(move), 0);
        packet_t checked;
        recv(csock, &checked, sizeof(checked), 0);
        switch(checked.type) {
            case PACKET_HIT:
            {
                update_board(board, move.packet.data, checked.hit, SENT);
                break;
            }
            case PACKET_GAMEOVER:
            {
                printf("You Win!\n");
                close(csock);
                close(ssock);
                exit(1);
            }
        }

        printf("WAITING FOR P2\n");

        // Wait fot client
        recv(csock, &received_packet, sizeof(received_packet), 0);

        switch(received_packet.type) {
            case PACKET_COORDINATE:
            {
                // Update board first
                unsigned short hit = 0;
                u8 x = received_packet.packet.data.indi;
                u8 y = received_packet.packet.data.indj;
                if (board[x][y].sym == '#') {
                    hit = 1;
                }
                packet_t check = {.type = PACKET_HIT, .hit = hit};
                if (check.hit == 1) {
                    boats_left -= 1;
                    if (boats_left == 0) {
                        packet_t gameover = {.type = PACKET_GAMEOVER};
                        send(csock, &gameover, sizeof(gameover), 0);
                        printf("Game Over: P2 Wins!\n");
                        close(csock);
                        close(ssock);
                        exit(1);
                    }
                }
                send(csock, &check, sizeof(check), 0);
                update_board(board, received_packet.packet.data, hit, RECEIVED);
                break;
            }
            case PACKET_QUIT:
            {
                close(csock);
                close(ssock);
                return;
            }
            default:
            {
                printf("Unrecognized packet type!\n");
                exit(1);
            }
        }
	}
	return;
}

void client_game_loop(int port, Tile **board) {
    int lose = 0;
    int boats_left = BOAT_PCS;
    if (client(port) == empty) {
        printf("Could not connect!\n");
        exit(1);
    }
    // Print board
    printf("PLAYER:\n");
    print_board(board);
    while (!lose) {
        printf("WAITING FOR P1\n");

        // Wait for server
        int bytes = 0;
        packet_t received_packet;
        packet_t to_send;
        // For input
		char letter; 
        u8 letter_val = 0;
		u8 number = 0;

        recv(conn, &received_packet, sizeof(received_packet), 0);

        switch(received_packet.type) {
            case PACKET_COORDINATE:
            {
                // Update board first
                u8 hit = 0;
                u8 x = received_packet.packet.data.indi;
                u8 y = received_packet.packet.data.indj;
                if (board[x][y].sym == '#') {
                    hit = 1;
                }
                packet_t check = {.type = PACKET_HIT, .hit = hit};
                if (check.hit == 1) {
                    boats_left -= 1;
                    if (boats_left == 0) {
                        packet_t gameover = {.type = PACKET_GAMEOVER};
                        send(conn, &gameover, sizeof(gameover), 0);
                        printf("Game Over: P1 Wins!\n");
                        close(conn);
                        exit(1);
                    }
                }
                send(conn, &check, sizeof(check), 0);
                update_board(board, received_packet.packet.data, hit, RECEIVED);
                break;
            }
            case PACKET_QUIT:
            {
                close(conn);
                return;
            }
            default:
            {
                printf("Unrecognized packet type!\n");
                exit(1);
            }
        }

        printf("YOUR MOVE\n");

		char *choice = get_input_pt_choice("Enter your choice: ");
        if (!strncmp(choice, "quit", 4)) {
            packet_t ff = {.type = PACKET_QUIT};
            send(conn, &ff, sizeof(ff), 0);
            close(conn);
            exit(1);
        }
		sscanf(choice, "%c %hu", &letter, &number);
        letter_val = letter - 65;

        // Send our choice to the server and await a hit response to update our board before continuing.
        packet_t move = {.type = PACKET_COORDINATE, .packet.data = board[(number)][(letter_val)]};
        send(conn, &move, sizeof(move), 0);
        packet_t checked;
        recv(conn, &checked, sizeof(checked), 0);
        switch(checked.type) {
            case PACKET_HIT:
            {
                update_board(board, move.packet.data, checked.hit, SENT);
                break;
            }
            case PACKET_GAMEOVER:
            {
                printf("You Win!\n");
                close(conn);
                exit(1);
            }
        }
	}
	return;
}

void update_board(Tile **board, Tile data, u8 hit, int type) {
    u8 x, y;
    x = data.indi, y = data.indj;
    Tile *updating = &board[x][y];

    // If there's a boat on our end
    if (type == SENT) {
        if (hit) {
            // Don't overwrite a boat piece with an X, just change the color to mimic a peg
            if (updating->sym == '#') {
                updating->color = RED;
            }
            else {
            // Otherwise place a red 'X' to mark a hit    
                updating->sym = 'X';
                updating->color = RED;
                updating->hit = hit;
            }
        }
        else {
            // Don't overwrite a boat piece
            if (updating->sym == '#') {
                updating->color = GREY;
            }
            else {
            // Otherwise place a white 'O' to mark a miss    
                updating->sym = 'O';
                updating->color = GREY;
                updating->hit = hit;
            }
        }
    }

    if (type == RECEIVED) {
        if (hit) {
            // If we had a previous hit, don't remove the marker, just remove the boat piece and replace it with an 'X'
            if(!strncmp(updating->color, "RED", 3)) {
                updating->sym = 'X';
            }
            // Or if the boat had a miss marked, just remove the boat piece and replace it with an 'O'
            else if (!strncmp(updating->color, "GREY", 4)) {
                updating->sym = 'O';
            }
            // Otherwise, remove the boat piece altogether
            else {
                clear_tile(updating);
            }
        }
        else {
            // This with a default piece is the same as "do nothing"
            clear_tile(updating);
        }
    }
    // Print out the updated board
    print_board(board);
}
