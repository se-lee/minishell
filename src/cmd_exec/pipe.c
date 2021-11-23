#include "minishell.h"

void	fd_dup_and_close(int input, int output)
{
	if (input != 0)
	{
		if (dup2(input, 0) < 0)
			perror("dup2");
		close(input);
	}
	if (output != 1)
	{
		if (dup2(output, 1) < 0)
			perror("dup2");
		close(output);
	}
}

void	pipe_and_launch_command(t_vars *vars, t_command *current_cmd,
			int input, int to_close)
{
	if (pipe(current_cmd->fd) < 0)
		ft_putendl_fd("error: pipe", 2);
	if (!to_close)
		to_close = current_cmd->fd[0];
	launch_commands(vars, current_cmd,
		(int [2]){input, current_cmd->fd[1]}, to_close);
}

void	wait_loop(t_vars *vars, pid_t child)
{
	int	i;
	int	status;
	int	nl;

	nl = 0;
	i = 0;
	waitpid(child, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			vars->return_value = 130;
		else if (WTERMSIG(status) == SIGQUIT)
		{
			vars->return_value = 131;
			ft_putstr_fd("Quit: 3", 2);
		}
		nl = 1;
	}
	else
		vars->return_value = WEXITSTATUS(status);
	while (waitpid(-1, &status, 0) != -1)
	{
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			nl = 1;
	}
	if (nl)
		write(1, "\n", 1);
	signal(SIGINT, control_c);
	set_termios();
}
