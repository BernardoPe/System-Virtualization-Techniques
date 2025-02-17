#include "simple_command.h"
#include <stdlib.h>
#include "../utils/alloc.h"

SimpleCommand *new_simple_command()
{
    SimpleCommand *command = (SimpleCommand *)safe_malloc(sizeof(SimpleCommand));
    command->args = (char **)safe_malloc(STARTING_ARGS_SIZE * sizeof(char *));
    command->argc = 0;
    command->input = NULL;
    command->output = NULL;
    return command;
}

void free_simple_command(SimpleCommand *command)
{
    free(command->args);
    free(command);
}