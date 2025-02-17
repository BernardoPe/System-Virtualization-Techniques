#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

#define STARTING_ARGS_SIZE 10

typedef struct
{
    char **args;
    char *input;
    char *output;
    int argc;
} SimpleCommand;

SimpleCommand *new_simple_command(); // Creates a new SimpleCommand struct

void free_simple_command(SimpleCommand *command); // Frees a SimpleCommand struct

#endif // SIMPLE_COMMAND_H