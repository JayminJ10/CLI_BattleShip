#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include "../inc/BSUtils.h"

#define NUMTHREADS 100

#define SENT                0
#define RECEIVED            1

#define PACKET_PING         1
#define PACKET_COORDINATE   2
#define PACKET_HIT          3
#define PACKET_QUIT         4

//Net related functions
int make_server();
void *server(void *unused);
void server_game_loop(Tile **board);
void *client(int port);
void client_game_loop(int port, Tile **board);
void close_socks();
