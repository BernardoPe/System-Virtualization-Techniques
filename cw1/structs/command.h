#ifndef COMMAND_H
#define COMMAND_H

#include "simple_command.h"
#include "parent_command.h"

#define STARTING_COMMANDS_SIZE 10

typedef struct
{
    SimpleCommand **simple_commands;
    ParentCommand *parent_command;
    int num_commands;
} Command;

Command *new_command(); // Creates a new Command struct

void free_command(Command *command); // Frees a Command struct

#endif
