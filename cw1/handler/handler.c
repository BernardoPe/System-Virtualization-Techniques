#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "handler.h"
#include "../utils/io.h"

int execute_command(Command *command)
{
    if (command->parent_command != NULL)
    {
        handle_parent_command(command);
        free_command(command);
        return 0;
    }

    int in_fd = 0;
    int pipe_fds[2];
    pid_t pid;
    int status;

    for (int i = 0; i < command->num_commands; i++)
    {
        SimpleCommand *simple_command = command->simple_commands[i];

        if (i < command->num_commands - 1)
        {
            pipe(pipe_fds);
            pid = execute_simple_command(simple_command, in_fd, pipe_fds[1]);
            close(pipe_fds[1]);
            in_fd = pipe_fds[0];
        }
        else
        {
            pid = execute_simple_command(simple_command, in_fd, 1);
        }
    }

    for (int i = 0; i < command->num_commands; i++)
    {
        waitpid(pid, &status, 0);
    }

    free_command(command);

    return 0;
}

int execute_simple_command(SimpleCommand *simple_command, int in_fd, int out_fd)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        signal(SIGINT, SIG_DFL); // Allow SIGINT on child process
        if (simple_command->input != NULL)
        {
            int file_fd = safe_open(simple_command->input, O_RDONLY, 0644);
            dup2(in_fd, file_fd);
            safe_close(in_fd);
        }
        else if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            safe_close(in_fd);
        }

        if (simple_command->output != NULL)
        {
            int file_fd = safe_open(simple_command->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(file_fd, STDOUT_FILENO);
            safe_close(file_fd);
        }
        else if (out_fd != STDOUT_FILENO)
        {
            dup2(out_fd, STDOUT_FILENO);
            safe_close(out_fd);
        }
        execvp(simple_command->args[0], simple_command->args);
        perror("bach");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

int handle_parent_command(Command *command)
{
    ParentCommand *parent_command = command->parent_command;
    switch (parent_command->command_code)
    {
    case CD_COMMAND_CODE:
        handle_cd(parent_command);
        break;
    case EXIT_COMMAND_CODE:
        free_command(command);
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
    return 0;
}

void handle_cd(ParentCommand *parent_command)
{
    if (parent_command->argc == 1)
    {
        chdir(getenv("HOME"));
    }
    else
    {
        if (chdir(parent_command->args[1]) != 0)
        {
            perror("cd");
        }
    }
}

void get_curr_dir(char *cwd, size_t size)
{
    if (getcwd(cwd, size) == NULL)
    {
        perror("getcwd");
    }
}