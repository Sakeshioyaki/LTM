// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <pthread.h>
// #include <signal.h>
// #include "user.c"

// #define MAX_CLIENTS 100
// #define BUFFER_SZ 2048
// #define NAME_LEN 32

// static int client_count = 0;
// static int uid = 10;

// typedef struct
// {
//     struct sockaddr_in address;
//     int sockfd;
//     int uid;
//     char name[NAME_LEN];
// } Client_t;

// Client_t *clients[MAX_CLIENTS];

// void str_overwrite_stdout()
// {
//     printf("\r%s", "> ");
//     fflush(stdout);
// }

// void str_trim_lf(char *arr, int len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         if (arr[i] == '\n')
//         {
//             arr[i] = '\0';
//             break;
//         }
//     }
// }

// void add_client(Client_t *cl)
// {
//     pthread_mutex_lock(&clients_mutex);
//     for (int i = 0; i < MAX_CLIENTS; i++)
//     {
//         if (clients[i] == NULL)
//         {
//             clients[i] = cl;
//             break;
//         }
//     }
//     pthread_mutex_unlock(&clients_mutex);
// }

// void remove_client(int uid)
// {
//     pthread_mutex_lock(&clients_mutex);
//     for (int i = 0; i < MAX_CLIENTS; i++)
//     {
//         if (clients[i] != NULL && clients[i]->uid == uid)
//         {
//             clients[i] = NULL;
//             break;
//         }
//     }
//     pthread_mutex_unlock(&clients_mutex);
// }

// void send_message(char *msg, int uid)
// {
//     pthread_mutex_lock(&clients_mutex);
//     for (int i = 0; i < MAX_CLIENTS; i++)
//     {
//         if (clients[i] != NULL && clients[i]->uid != uid)
//         {
//             int wrt_status = write(clients[i]->sockfd, msg, strlen(msg));
//             if (wrt_status < 0)
//             {
//                 printf("ERROR: write to descriptor failed!\n");
//                 break;
//             }
//         }
//     }
//     pthread_mutex_unlock(&clients_mutex);
// }

// void *handle_client(void *arg)
// {
//     char buffer[BUFFER_SZ];
//     char name[NAME_LEN];
//     int leave_flag = 0;
//     client_count++;

//     Client_t *cli = (Client_t *)arg;

//     // Name
//     if (recv(cli->sockfd, name, NAME_LEN, 0) <= 0 || strlen(name) >= NAME_LEN - 1)
//     {
//         printf("Enter name correctly\n");
//         leave_flag = 1;
//     }
//     else
//     {
//         strcpy(cli->name, name);
//         sprintf(buffer, "%s has joined\n", cli->name); // write to buffer
//         printf("%s", buffer);                          // print on server
//         send_message(buffer, cli->uid);                // print to all other clients
//     }
//     bzero(buffer, BUFFER_SZ);

//     while (1)
//     {
//         if (leave_flag)
//             break;

//         int receive = recv(cli->sockfd, buffer, BUFFER_SZ, 0);
//         if (receive > 0)
//         {
//             send_message(buffer, cli->uid);
//             str_trim_lf(buffer, strlen(buffer));
//             printf("%s\n", buffer);
//         }
//         else if (receive == 0 || strcmp(buffer, "exit") == 0)
//         {
//             sprintf(buffer, "%s has left\n", cli->name);
//             printf("%s", buffer);
//             send_message(buffer, cli->uid);
//             leave_flag = 1;
//         }
//         else
//         {
//             printf("ERROR: -1\n");
//             leave_flag = 1;
//         }
//         bzero(buffer, BUFFER_SZ);
//     }
//     close(cli->sockfd);
//     remove_client(cli->uid);
//     free(cli);
//     client_count--;
//     pthread_detach(pthread_self());

//     return NULL;
// }