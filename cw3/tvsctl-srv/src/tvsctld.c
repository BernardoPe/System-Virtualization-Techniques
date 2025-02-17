#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include "handler/tvsctld_commands.h"
#include "tvsctld.h"

volatile int interrupt_flag = 0;
int server_sockfd;

void thread_error(char *msg)
{
    perror(msg);
    pthread_exit((void *)(intptr_t)1);
}

void sigterm_handler(int sig)
{
    printf("Received SIGTERM\n");
    close(server_sockfd);
    interrupt_flag = 1;
}

void sigint_handler(int sig)
{
    printf("Received SIGINT\n");
    close(server_sockfd);
    interrupt_flag = 1;
}

void init_signal_handler()
{
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigint_handler);
}

int process_command(char *buffer, int out_fd)
{

    tvsctl_command command = handle_command(buffer, out_fd);

    switch (command.err_code)
    {
    case COMMAND_PARSE_ERROR:
        send(out_fd, command.err_msg, strlen(command.err_msg), 0);
        break;
    case SOCKET_ERROR:
        fprintf(stderr, "Error sending command: %s\n", command.err_msg);
        break;
    case COMMAND_EXEC_ERROR:
        send(out_fd, command.err_msg, strlen(command.err_msg), 0);
        break;
    default:
        break;
    }

    return 0;
}

void *process_connection(void *arg)
{
    int conn_fd = (int)(intptr_t)arg;
    char buffer[MAX_MSG_SIZE];
    ssize_t len;

    while (!interrupt_flag)
    {
        len = read(conn_fd, buffer, MAX_MSG_SIZE - 1);
        if (len == 0)
        {
            printf("Client disconnected\n");
            break;
        }
        if (len < 0)
        {
            perror("Error receiving data");
            break;
        }
        printf("Received: %d bytes\n", (int)len);
        buffer[len] = '\0'; // Null-terminate the received data
        if (process_command(buffer, conn_fd) < 0)
        {
            fprintf(stderr, "Error processing command\n");
            break;
        }
    }

    close(conn_fd);

    return NULL;
}

void run(int sockfd)
{
    while (!interrupt_flag)
    {
        struct sockaddr_un client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (interrupt_flag)
        {
            break;
        }
        if (client_sockfd < 0)
        {
            perror("Error connecting to client");
            continue;
        }
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, process_connection, (void *)(intptr_t)client_sockfd) != 0)
        {
            perror("Error creating client thread");
        }
        pthread_detach(client_thread);
    }
}

int main()
{

    init_signal_handler();

    int nfd = sd_listen_fds(0);

    if (nfd != 1)
    {
        fprintf(stderr, "echod: none or too many file descriptors\n");
        exit(1);
    }

    server_sockfd = SD_LISTEN_FDS_START;

    printf("Server listening on %s\n", SOCKET_PATH);

    run(server_sockfd);

    close(server_sockfd);

    unlink(SOCKET_PATH); // Clean up the socket file

    return 0;
}