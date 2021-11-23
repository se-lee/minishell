#include "minishell.h"

void	ft_append(char **a, char *str)
{
	char	*result;

	result = ft_strjoin(*a, str);
	free(*a);
	*a = result;
}

void	*protected_malloc(size_t count, size_t size)
{
	void	*p;
	size_t	i;

	i = 0;
	p = malloc(size * count);
	if (!p)
	{
		perror("Malloc failed\n");
		exit(-1);
	}
	return (p);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	display_cmd_error(t_command *current_cmd, char *message, int show_arg)
{
	char	*command_name;
	char	*argument;

	command_name = current_cmd->command[0];
	argument = current_cmd->command[1];
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command_name, 2);
	ft_putstr_fd(": ", 2);
	if (show_arg == TRUE)
	{
		ft_putstr_fd(argument, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(message, 2);
}

void	display_syntax_error(t_token *current_token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error near unexpected token ", 2);
	ft_putchar_fd('`', 2);
	ft_putstr_fd(current_token->buffer.str, 2);
	ft_putchar_fd('\'', 2);
	ft_putchar_fd('\n', 2);
}
