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

void	set_termios(void)
{
	struct termios	termios;

	tcgetattr(0, &termios);
	termios.c_cc[VINTR] = 0;
	termios.c_cc[VQUIT] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars			vars;
	char			*str;

	(void)argc;
	(void)argv;
	vars.error = 0;
	envlist_create(&vars, envp);
	tcgetattr(0, &vars.saved_termios);
	set_termios();
	str = readline("minishell$ ");
	while (str != NULL)
	{
		if (str[0])
		{
			add_history(str);
			parsing(&vars, str);
			if (vars.error == 0)
				execute_command(&vars, envp);
			free(str);
			free_struct(&vars);
		}
		str = readline("minishell$ ");
	}
	printf("exit\n");
	return (0);
}
