#include "minishell.h"

int	echo_n(char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	if (str[0] == '-' && ft_strlen(str) == 1)
		return (0);
	i = 1;
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i])
		return (0);
	return (1);
}

void	builtin_echo(t_command *current_cmd)
{
	int	n_option;
	int	i;

	n_option = 0;
	i = 1;
print_commands(current_cmd);
	if (current_cmd && current_cmd->command[1] != NULL)
	{
		if (current_cmd->command[1][0] == '-')
		{
			while (echo_n(current_cmd->command[i]) == 1)
			{
				n_option = 1;
				i++;
			}
		}
printf("i:%d\n", i);
		while (current_cmd->command[i])
		{
			printf("%s", current_cmd->command[i]);
			i++;
			if (current_cmd->command[i])
				printf(" ");
		}
	}
	if (n_option == 0)
		printf("\n");
}
