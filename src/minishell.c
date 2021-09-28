#include "minishell.h"

void	free_token(t_token *token)
{
	free(token->buffer.str);
	token->buffer.len = 0;
	token->token_type = 0;
	token->next = NULL;
	free(token);
}

void	free_struct(t_vars *vars)
{
	t_token	*current_token;
	t_token	*next;

	current_token = vars->first;
	while (current_token->next)
	{
		next = current_token->next;
		free_token(current_token);
		current_token = next;
	}
	if (current_token)
		free(current_token);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars			vars;
	char			*str;
	struct termios	t;

	(void)argc;
	(void)argv;
	create_envlist(&vars, envp);
	tcgetattr(0, &t);
	t.c_cc[VINTR] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
	str = readline("minishell$ ");
	while (str != NULL)
	{
		if (str[0])
		{
			add_history(str);
			parsing(&vars, str);
			builtin_exec(&vars, envp);
			free(str);
			free_struct(&vars);
		}
		str = readline("minishell$ ");
	}
	return (0);
}
