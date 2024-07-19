#include "../inc/BSUtils.h"
#include <pthread.h>

void *no_ret = NULL;

void *server(void *unused) {
    // char buf[1024];
    // // create socket
    // const int fd = socket(PF_INET, SOCK_STREAM, 0);

    // // bind to open port
    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = INADDR_ANY;

    // if (bind(fd, (struct sockaddr*) &addr, sizeof(addr))) {
    //     perror("bind error:");
    //     return no_ret;
    // }

    // // read port
    // socklen_t addr_len = sizeof(addr);
    // getsockname(fd, (struct sockaddr*) &addr, &addr_len);
    // printf("server is on port %d\n", (int) ntohs(addr.sin_port));


    // if (listen(fd, 1)) {
    //     perror("listen error:");
    //     return no_ret;
    // }

    // // accept incoming connection
    // struct sockaddr_storage caddr;
    // socklen_t caddr_len = sizeof(caddr);
    // const int cfd = accept(fd, (struct sockaddr*) &caddr, &caddr_len);

    // while(1) {
    //     // read from client with recv!
    //     bzero(buf, 1024);
    //     recv(cfd, buf, sizeof(buf), 0);

    //     if (!strncmp(buf, "quit", 4)) {
    //         close(cfd);
    //         close(fd);
    //         exit(1);
    //     }

    //     printf("client says:\n    %s\n", buf);
    // }

    char *ip = "127.0.0.1";
    int port = 5566;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    // Msgs
    char server_message[2000], client_message[2000];
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0){
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0){
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("[+]Client connected.\n");

    while(1) {
        bzero(buffer, 1024);
        // Clean buffers:
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));

        printf("Enter message: ");
        fgets(server_message, 1024, stdin);
        printf("\n");

        // Receive client's message:
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0){
            printf("Couldn't receive\n");
            return no_ret;
        }
        printf("Msg from client: %s\n", client_message);
        
        // Respond to client:
        strcpy(server_message, "This is the server's message.");
        
        if (send(client_sock, server_message, strlen(server_message), 0) < 0){
            printf("Can't send\n");
            return no_ret;
        }
    }

}

void *client(int port) {
    // const int fd = socket(PF_INET, SOCK_STREAM, 0);

    // struct sockaddr_in addr = { 0 };
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons((short) port);

    // // connect to local machine at specified port
    // char addrstr[NI_MAXHOST + NI_MAXSERV + 1];
    // snprintf(addrstr, sizeof(addrstr), "127.0.0.1:%d", port);

    // // parse into address
    // inet_pton(AF_INET, addrstr, &addr.sin_addr);

    // // connect to server
    // if (connect(fd, (struct sockaddr*) &addr, sizeof(addr))) {
    //     perror("connect error:");
    //     return;
    // }

    // // Get message to send
    // while(1) {
    //     char buf[128];
    //     char *msg;
        
    //     printf("Enter a message: ");
    //     fgets(buf, 128, stdin);
    //     printf("\n");
    //     sscanf(buf, "%s", msg);

    //     if (!strncmp(msg, "quit", 4)) {
    //         printf("Exiting...\n");
    //         break;
    //     }

    //     send(fd, msg, strlen(msg) + 1, 0);
    // }

    char *ip = "127.0.0.1";
    int p = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    // Msgs
    char server_message[2000], client_message[2000];
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0){
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = p;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    printf("Connected to the server.\n");

    while(1) {
        bzero(buffer, 1024);
        // Clean buffers:
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));

        printf("Enter message: ");
        fgets(client_message, 1024, stdin);
        printf("\n");

        // Send the message to server:
        if(send(sock, client_message, strlen(client_message), 0) < 0){
            printf("Unable to send message\n");
            return no_ret;
        }
        
        // Receive the server's response:
        if(recv(sock, server_message, sizeof(server_message), 0) < 0){
            printf("Error while receiving server's msg\n");
            return no_ret;
        }
        
        printf("Server's response: %s\n",server_message);
    }

//TODO: do threads
    //close(fd);
}

// void *c_send(void *args) {
//     int sock = args[0];
//     while(1) {
//         char msg[1024];
//         bzero(buffer, 1024);
//         printf("Enter message: ");
//         fgets(msg, 1024, stdin);
//         printf("\n");

//         if(!strncmp(buffer, "exit", 4)) {
//             close(sock);
//             printf("Disconnected from the server.\n");
//             break;
//         }
//         if(!strncmp(buffer, "reconnect", 9)) {
//             printf("[=]Reconnecting.\n");
//             connect(sock, (struct sockaddr*)&addr, sizeof(addr));
//             printf("[+]Connected to the server.\n");
//         }
//         send(sock, buffer, strlen(buffer), 0);
//     }
// }

// void *s_send(void *args) {
//     int sock = args[0];
//     while(1) {
//         char msg[1024];
//         bzero(buffer, 1024);
//         printf("Enter message: ");
//         fgets(msg, 1024, stdin);
//         printf("\n");

//         if(!strncmp(buffer, "exit", 4)) {
//             close(sock);
//             printf("Disconnected from the server.\n");
//             break;
//         }
//         if(!strncmp(buffer, "reconnect", 9)) {
//             printf("[=]Reconnecting.\n");
//             connect(sock, (struct sockaddr*)&addr, sizeof(addr));
//             printf("[+]Connected to the server.\n");
//         }
//         send(sock, buffer, strlen(buffer), 0);
//     }
// }

// void *c_rec(void *args) {
//     int client_sock = args[0];
//     bzero(buffer, 1024);
//     recv(client_sock, buffer, sizeof(buffer), 0);

//     char msg[1024];
//     bzero(buffer, 1024);
//     printf("Enter message: ");
//     fgets(msg, 1024, stdin);
//     printf("\n");

//     if(!strncmp(buffer, "exit", 4)) {
//         close(client_sock);
//         printf("[+]Server disconnected.\n\n");
//     break;
//     }

//     printf("Client: %s\n", buffer);
// }

// void *s_rec(void *args) {
//     int sock = args[0];
//     bzero(buffer, 1024);
//     recv(sock, buffer, sizeof(buffer), 0);

//     char msg[1024];
//     bzero(buffer, 1024);
//     printf("Enter message: ");
//     fgets(msg, 1024, stdin);
//     printf("\n");

//     if(!strncmp(buffer, "exit", 4)) {
//         close(sock);
//         printf("[+]Client disconnected.\n\n");
//     break;
//     }

//     printf("Client: %s\n", buffer);
// }