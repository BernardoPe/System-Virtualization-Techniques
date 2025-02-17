#ifndef PARENT_COMMAND_H
#define PARENT_COMMAND_H

#define INITIAL_ARGS_SIZE 10

#define EXIT_COMMAND "exit"
#define EXIT_COMMAND_CODE 0

#define CD_COMMAND "cd"
#define CD_COMMAND_CODE 1

typedef struct
{
    char **args;
    int argc;
    int command_code;
} ParentCommand;

ParentCommand *new_parent_command(); // Creates a new ParentCommand struct

void free_parent_command(ParentCommand *command); // Frees a ParentCommand struct

#endif // PARENT_COMMAND_H