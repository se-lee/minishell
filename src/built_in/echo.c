#include "../../include/minishell.h"

/*
- [echo] with option -n

when 'echo' is called with argument
print the arguments 

- show new line character with -n option
- where to put spaces

*/

void	builtin_echo(char **argv) // argv argument to be changed later
{
	int i;
	int	n_option;

	if (ft_strncmp(argv[1], "-n", 2) == 0)
	{
		n_option = 1;
		i = 2;
	}
	else
	{
		n_option = 0;
		i = 1;
	}
	while(argv[i] != NULL)
	{
		printf("%s", argv[i]);
		i++;
	}
	if (n_option == 0)
		printf("\n");
}

int main(int argc, char **argv)
{
	builtin_echo(argv);
	return (0);
}
