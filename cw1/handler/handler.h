#ifndef HANDLER_H
#define HANDLER_H
#include <stddef.h>
#include "../structs/command.h"

#define CD_COMMAND "cd"
#define EXIT_COMMAND "exit"
#define PATH_MAX 256

// Executes a command
int execute_command(Command *command);

// Executes a simple command
int execute_simple_command(SimpleCommand *simple_command, int in_fd, int out_fd);

// Handles a parent command
int handle_parent_command(Command *command);

// Handles the cd command
void handle_cd(ParentCommand *parent_command);

// Gets the current directory
void get_curr_dir(char *cwd, size_t size);

#endif // HANDLER_H
