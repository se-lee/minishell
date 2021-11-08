#include "minishell.h"

void	free_token(t_token *token)
{
	free(token->buffer.str);
	token->buffer.len = 0;
	token->token_type = 0;
	token->next = NULL;
	free(token);
}

void	free_tokens(t_vars *vars)
{
	t_token	*current_token;
	t_token	*next;

	current_token = vars->first;
	while (current_token)
	{
		next = current_token->next;
		free_token(current_token);
		current_token = next;
	}
}

void	free_commands(t_vars *vars)
{
	t_command	*current_cmd;
	t_command	*next;

	current_cmd = vars->cmd;
	if (current_cmd)
	{
		while (current_cmd->next)
		{
			next = current_cmd->next;
			free_array(current_cmd->command);
			free(current_cmd);
			current_cmd = next;
		}
	}
	if (current_cmd)
	{
		free_array(current_cmd->command);
		free(current_cmd);
	}
}

void	set_termios(void)
{
	struct termios	termios;

	tcgetattr(0, &termios);
	termios.c_lflag &= ~ECHOCTL;
	termios.c_cc[VQUIT] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
}

void	control_c(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	// rl_replace_line("", 0);
	rl_redisplay();
}

void	sigmain(int sig)
{
	if (sig == SIGQUIT)
		printf("Quit: 3\n");
	else
		printf("\n");
}

void	sigchild(int sig)
{
	if (sig == SIGINT)
		exit(130);
	else
		exit(131);
}

void	free_inout(t_vars *vars)
{
	t_redirect	*current_inout;
	t_redirect	*temp_next;

	current_inout = vars->in;
	while (current_inout)
	{
		temp_next = current_inout->next;
		free(current_inout->filename);
		free(current_inout);
		current_inout = temp_next;
	}
	current_inout = vars->out;
	while (current_inout)
	{
		temp_next = current_inout->next;
		free(current_inout->filename);
		free(current_inout);
		current_inout = temp_next;
	}
	vars->in = NULL;
	vars->out = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_vars			vars;
	char			*str;

	(void)argc;
	(void)argv;
	envlist_create(&vars, envp);
	tcgetattr(0, &vars.saved_termios);
	str = "";
	vars.in = NULL;
	vars.out = NULL;
	vars.return_value = 0;
	while (str != NULL)
	{
		vars.error = 0;
		if (str[0])
		{
			add_history(str);
			parsing(&vars, str);
			if (vars.error == 0 && vars.cmd != NULL)
				execute_pipe_commands(&vars);
			free(str);
			free_tokens(&vars);
			free_commands(&vars);
			free_inout(&vars);
		}
		set_termios();
		signal(SIGINT, control_c);
		str = readline("\x1B[32mminishell$\x1B[0m: ");
	}
	printf("exit\n");
}
