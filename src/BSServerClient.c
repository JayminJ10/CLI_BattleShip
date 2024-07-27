#include "../inc/BSNet.h"

void *no_ret = NULL;
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
        return no_ret;
    }

    // read port
    socklen_t addr_len = sizeof(addr);
    getsockname(ssock, (struct sockaddr*) &addr, &addr_len);
    printf("server is on port %d\n", (int) ntohs(addr.sin_port));

    if (listen(ssock, 1)) {
        perror("listen error:");
        return no_ret;
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
        return no_ret;
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
        packet_t recieved_packet;
		packet_t to_send;
		char *choice = get_input_pt_choice("Enter your choice: ");
		sscanf(choice, "%c %hu", &letter, &number);
        letter_val = letter - 65;
        printf("%hu %hu\n", letter_val, number);
		to_send.i = htons(letter_val);
		to_send.j = htons(number);

        // Send first
        //const char *msg = "server";
        send(csock, &to_send, sizeof(to_send), 0);

        printf("WAITING FOR P2\n");

        // Wait fot client
        char buf[1024];
        recv(csock, buf, sizeof(buf), 0);
        printf("client says:\n    %s\n", buf);

        close(csock);
        close(ssock);

		break;
	}
	return;
}

void client_game_loop(int port, Tile **board) {
    int lose = 0;
    client(port);
    while (!lose) {
		// Print board
		printf("PLAYER:\n");
		print_board(board);

        printf("WAITING FOR P1\n");

        // Wait for server
        // char buf[128];
        // char *rec_msg = (char*)malloc(128);
        int bytes = 0;
        packet_t recieved_packet;
		packet_t to_send;
        while(1) {
            // bzero(buf, 128);
            // bzero(rec_msg, 128);
            // bytes = recv(conn, buf, sizeof(buf), 0);
            bytes = recv(conn, &recieved_packet, sizeof(recieved_packet), 0);
            // strncpy(rec_msg, buf, strlen(buf));
            if (bytes > 0) {
                //printf("%s\n", buf);
                break;
            }
            // if (!strncmp(buf, "server", 6)) {
            //     printf("server says:\n    %s\n", buf);
            //     break;
            // }
        }
        printf("%d\n", (int)(ntohs(recieved_packet.i)));
        printf("%d\n", (int)(ntohs(recieved_packet.j)));

		// Get input
		char letter; 
        u8 letter_val = 0;
		u8 number = 0;
		char *choice = get_input_pt_choice("Enter your choice: ");
		sscanf(choice, "%c %hu", &letter, &number);
        letter_val = letter - 65;
        printf("%hu %hu\n", letter_val, number);
		to_send.i = htons(letter_val);
		to_send.j = htons(number);

        // Send to server
        const char *msg = "the client says hello!";
        send(conn, msg, strlen(msg) + 1, 0);

        close(conn);
		break;
	}
	return;
}
