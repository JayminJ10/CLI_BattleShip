#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include "../inc/BSUtils.h"

#define NUMTHREADS 100

//Net related functions
int make_server();
void *server(void *unused);
void server_game_loop(Tile **board);
// void *server_listen(void *unused);
// void *server_bind(void *thread_id);
void *client(int port);
void client_game_loop(int port, Tile **board);
// void *client_handler(void *thread_id);
