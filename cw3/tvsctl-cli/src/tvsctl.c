#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>

#define TVSCTL_SOCK_PATH "/run/isel/tvsctld/request"
#define BUFFER_SIZE 1024

void show_usage(const char *prog_name)
{
    fprintf(stderr, "Usage: %s <command> [args]\n", prog_name);
    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  reset [scale [base]] - Reset the service\n");
    fprintf(stderr, "  start                - Start the service\n");
    fprintf(stderr, "  stop [-db]           - Stop the service\n");
    fprintf(stderr, "  inc [delta]          - Increment counter\n");
    fprintf(stderr, "  dec [delta]          - Decrement counter\n");
    fprintf(stderr, "  status               - Get service status\n");
}

int main(int argc, char *argv[])
{
    // These must always be the first instructions. DON'T EDIT
    close(0);
    open("/dev/null", O_RDONLY); // DON'T EDIT

    // Validate arguments
    if (argc < 2)
    {
        show_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Construct the command string
    char command[BUFFER_SIZE] = {0};
    for (int i = 1; i < argc; i++)
    {
        strncat(command, argv[i], BUFFER_SIZE - strlen(command) - 1);
        if (i < argc - 1)
        {
            strncat(command, " ", BUFFER_SIZE - strlen(command) - 1);
        }
    }

    // Create a client socket
    int cli_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (cli_sock < 0)
    {
        perror("tvsctl: error creating socket");
        return EXIT_FAILURE;
    }

    // Set up the server address
    struct sockaddr_un srv_address;
    memset(&srv_address, 0, sizeof(struct sockaddr_un));
    srv_address.sun_family = AF_UNIX;
    strncpy(srv_address.sun_path, TVSCTL_SOCK_PATH, sizeof(srv_address.sun_path) - 1);

    // Connect to the server
    if (connect(cli_sock, (struct sockaddr *)&srv_address, sizeof(struct sockaddr_un)) < 0)
    {
        fprintf(stderr, "tvsctl: error connecting to server at '%s': %s\n", TVSCTL_SOCK_PATH, strerror(errno));
        close(cli_sock);
        return EXIT_FAILURE;
    }

    // Send the command to the server
    if (write(cli_sock, command, strlen(command)) < 0)
    {
        perror("tvsctl: error sending command");
        close(cli_sock);
        return EXIT_FAILURE;
    }

    // Read the response from the server
    char response[BUFFER_SIZE] = {0};
    int bytes_read = 0;
    while ((bytes_read = read(cli_sock, response, BUFFER_SIZE - 1)) == BUFFER_SIZE - 1)
    {
        response[bytes_read] = '\0';
        printf("Server response: %s\n", response);
    }
    response[bytes_read] = '\0';
    printf("Server response: %s\n", response);

    // Close the socket
    close(cli_sock);
    return EXIT_SUCCESS;
}