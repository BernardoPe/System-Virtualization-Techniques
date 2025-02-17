#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "./handler/handler.h"
#include "./parser/parser.h"

#define LINE_BUF 1024

int main()
{
    signal(SIGINT, SIG_IGN); // Ignore SIGINT on parent process
    char line[LINE_BUF];
    Command *command;
    while (1)
    {
        printf("$ ");
        if (!fgets(line, sizeof(line), stdin))
        {
            break;
        }
        trim(line);
        command = parse_command(line);
        if (command != NULL)
        {
            execute_command(command);
        }
    }
    return 0;
}