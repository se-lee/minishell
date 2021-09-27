#include "minishell.h"

void	free_token(t_token *token)
{
	free(token->buffer.str);
	free(token);
	// token->buffer.len = 0;
	// token->token_type = 0;
	// token->next = NULL;
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
}

int	main(int argc, char **argv, char **envp)
{
	t_vars			vars;
	char			*str;
	struct termios	t;

	vars.envp = envp;
	(void)argc;
	(void)argv;
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
			free(str);
			free_struct(&vars);
		}
		str = readline("minishell$ ");
	}
	return (0);
}
