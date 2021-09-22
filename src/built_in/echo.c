#include "minishell.h"

/*
- [echo] with option -n

when 'echo' is called with argument
print the arguments 

- show new line character with -n option
- where to put spaces
*/

void	display_newline();

void	builtin_echo(char **argv) // argv argument to be changed later
{
	int i;

	i = 2;
	while(argv[i] != NULL)
	{
		printf("%s", argv[i]);

		i++;
	}
}


