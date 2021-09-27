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
	t_token	*next;

	while (vars->first)
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
	t.c_cc[VINTR] = 0;  // set the INT character to 0 (disable)
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
	str = readline("minishell$ ");
	while (str != NULL)
	{
		if (str)
		{
			add_history(str);
			parsing(&vars, str);
			// free_struct(&vars);
			free(str);
		}
		str = readline("minishell$ ");
	}
	return (0);
}
