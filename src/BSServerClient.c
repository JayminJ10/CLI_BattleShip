#include "../inc/BSNet.h"

void *empty = NULL;
int ssock, csock, conn;

void *server(void *unused) {
    // create socket
    ssock = socket(PF_INET, SOCK_STREAM, 0);

    // bind to open port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(ssock, (struct sockaddr*) &addr, sizeof(addr))) {
        perror("bind error:");
        return empty;
    }

    // read port
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

//TODO: do threads
}

void server_game_loop(Tile **board) {
	int lose = 0;
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
        printf("%hu %hu\n", letter_val, number);

        // Send first:
        // Send our choice to the client and await a hit response to update our board before continuing.
        packet_t move = {.type = PACKET_COORDINATE, .packet.data = board[/*htons*/(number)][/*htons*/(letter_val)]};
        send(csock, &move, sizeof(move), 0);
        packet_t checked;
        recv(csock, &checked, sizeof(checked), 0);
        switch(checked.type) {
            case PACKET_HIT:
                printf("checking hit\n");
                update_board(board, move.packet.data, checked.hit, SENT);
        }

        printf("WAITING FOR P2\n");

        // Wait fot client
        recv(csock, &received_packet, sizeof(received_packet), 0);

        switch(received_packet.type) {
            case PACKET_COORDINATE:
                // Update board first
                u8 hit = 0;
                u8 x = received_packet.packet.data.indi;
                u8 y = received_packet.packet.data.indj;
                if (board[x][y].sym == '#') {
                    hit = 1;
                }
                packet_t check = {.type = PACKET_HIT, .hit = hit};
                send(csock, &check, sizeof(check), 0);
                update_board(board, received_packet.packet.data, hit, RECEIVED);
                break;
            case PACKET_QUIT:
                close(csock);
                close(ssock);
                return;
            default:
                printf("Unrecognized packet type!\n");
                exit(1);
        }
	}
	return;
}

void client_game_loop(int port, Tile **board) {
    int lose = 0;
    client(port);
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
                // Update board first
                u8 hit = 0;
                u8 x = received_packet.packet.data.indi;
                u8 y = received_packet.packet.data.indj;
                if (board[x][y].sym == '#') {
                    hit = 1;
                }
                packet_t check = {.type = PACKET_HIT, .hit = hit};
                send(conn, &check, sizeof(check), 0);
                update_board(board, received_packet.packet.data, hit, RECEIVED);
                break;
            case PACKET_QUIT:
                close(conn);
                return;
            default:
                printf("Unrecognized packet type!\n");
                exit(1);
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
        printf("%hu %hu\n", letter_val, number);

        // Send our choice to the server and await a hit response to update our board before continuing.
        packet_t move = {.type = PACKET_COORDINATE, .packet.data = board[/*htons*/(number)][/*htons*/(letter_val)]};
        send(conn, &move, sizeof(move), 0);
        packet_t checked;
        recv(conn, &checked, sizeof(checked), 0);
        switch(checked.type) {
            case PACKET_HIT:
                printf("checking hit\n");
                update_board(board, move.packet.data, checked.hit, SENT);
        }

        // switch(received_packet.type) {
        //     case PACKET_COORDINATE:
        //         // Update board first
        //         u8 hit = 0;
        //         u8 x = received_packet.packet.data.indi;
        //         u8 y = received_packet.packet.data.indj;
        //         if (board[x][y].sym == '#') {
        //             hit = 1;
        //         }
        //         packet_t check = {.type = PACKET_HIT, .hit = hit};
        //         send(conn, &check, sizeof(check), 0);
        //         update_board(board, received_packet.packet.data, hit, RECEIVED);
        //         // Get input
        //         char letter; 
        //         u8 letter_val = 0;
        //         u8 number = 0;
        //         char *choice = get_input_pt_choice("Enter your choice: ");
        //         if (!strncmp(choice, "quit", 4)) {
        //             packet_t quit_packet = {.type = PACKET_QUIT};
        //             send(conn, &quit_packet, sizeof(quit_packet), 0);
        //             break;
        //         }
        //         sscanf(choice, "%c %hu", &letter, &number);
        //         letter_val = letter - 65;
        //         printf("%hu %hu\n", letter_val, number);
        //         Tile send = board[number][letter_val];
        //         to_send.type = PACKET_COORDINATE;
        //         to_send.packet.data = send;
        //         break;
            
        //     default:
        //         printf("Unrecognized packet type!\n");
        //         exit(1);
        // }
        
        // Send to server
        // send(conn, &to_send, sizeof(to_send), 0);
	}
	return;
}

void update_board(Tile **board, Tile data, u8 hit, int type) {
    u8 x, y;
    x = data.indi, y = data.indj;
    Tile *updating = &board[x][y];
    // If there's a boat on our end

    printf("%d %d\n", x, y);
    
    if (type == SENT) {
        printf("Sent\n");
        if (hit) {
            printf("Hit\n");
            if (updating->sym == '#') {
                updating->color = RED;
            }
            else {
                updating->sym = 'X';
                updating->color = RED;
                updating->hit = hit;
            }
        }
        else {
            printf("No Hit\n");
            if (updating->sym == '#') {
                updating->color = GREY;
            }
            else {
                updating->sym = 'O';
                updating->color = GREY;
                updating->hit = hit;
            }
        }
    }

    if (type == RECEIVED) {
        printf("Received ");
        if (hit) {
            printf("Hit\n");
            if(updating->color == RED || updating->color == GREY) {
                remove_tile(updating);
            }
            else {
                clear_tile(updating);
            }
        }
        else {
            printf("No Hit\n");
            clear_tile(updating);
        }
    }
    print_board(board);
}

void close_socks() {
    close(ssock);
    close(csock);
    close(conn);
}
