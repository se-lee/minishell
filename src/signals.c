#include "minishell.h"

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
	rl_replace_line("", 0);
	rl_redisplay();
	g_vars->return_value = 1;
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
	{
		g_vars->return_value = 130;
		exit(130);
	}
	else
	{
		g_vars->return_value = 131;
		exit(131);
	}
}
