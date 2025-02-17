#include "parent_command.h"
#include <stdlib.h>

ParentCommand *new_parent_command()
{
    ParentCommand *command = (ParentCommand *)malloc(sizeof(ParentCommand));
    command->args = (char **)malloc(INITIAL_ARGS_SIZE * sizeof(char *));
    command->argc = 0;
    return command;
}

void free_parent_command(ParentCommand *command)
{
    free(command->args);
    free(command);
}