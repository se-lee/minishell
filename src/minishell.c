#include "minishell.h"

void	free_token(t_token *token)
{
	free(token->buffer.str);
	token->buffer.len = 0;
	token->token_type = 0;
	token->next = NULL;
}

void	free_struct(t_vars *vars)
{
	t_token	*current_token;

	while (vars->first->next)
	{
		current_token = vars->first;
		while (current_token)
		{
			current_token = current_token->next;
		}
		free_token(current_token);
	}
	vars->first = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;
	char	*str;

	vars.envp = envp;
	(void)argc;
	(void)argv;
	str = readline("minishell$ ");
	while (str != NULL)
	{
		if (str)
		{
			add_history(str);
			parsing(&vars, str);
			builtin_exec(&vars);

			// free_struct(&vars);
			free(str);
		}
		str = readline("minishell$ ");
	}
	return (0);
}
