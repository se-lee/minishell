#include "minishell.h"

void	run_command_no_pipe(t_vars *vars, t_command *current_cmd)
{
	pid_t	child;

	signal(SIGQUIT, sigchild);
	signal(SIGINT, sigchild);
	child = 0;
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		vars->std_in = dup(STDIN_FILENO);
		vars->std_out = dup(STDOUT_FILENO);
		redirect_and_run_cmd(vars, current_cmd, TRUE);
		dup2(vars->std_in, 0);
		dup2(vars->std_out, 1);
		close (vars->std_in);
		close(vars->std_out);
	}
	else
	{
		child = fork();
		if (child == 0)
			redirect_and_run_cmd(vars, current_cmd, FALSE);
	}
	if (child != 0)
		wait_loop(vars, child);
}

int	launch_commands(t_vars *vars, t_command *current_cmd,
		int fds[2], int to_close)
{
	pid_t	child;

	signal(SIGQUIT, sigchild);
	signal(SIGINT, sigchild);
	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		redirection(vars, current_cmd);
		fd_dup_and_close(fds[0], fds[1]);
		redirection(vars, current_cmd);
		if (to_close != 0)
			close(to_close);
		run_command_and_exit(vars, current_cmd);
	}
	else
	{
		if (fds[0] != 0)
			close(fds[0]);
		if (fds[1] != 1)
			close(fds[1]);
	}
	return (child);
}

void	execute_with_pipe(t_vars *vars, t_command *current_cmd)
{
	int			input;
	int			output;
	int			to_close;
	int			i;
	pid_t		child;

	input = 0;
	output = 1;
	to_close = 0;
	i = 0;
	while (i < count_command(vars->cmd) - 1)
	{
		pipe_and_launch_command(vars, current_cmd, input, to_close);
		to_close = 0;
		input = current_cmd->fd[0];
		current_cmd = current_cmd->next;
		i++;
		current_cmd->cmd_index = i;
	}
	child = launch_commands(vars, current_cmd,
			(int [2]){input, output}, to_close);
	wait_loop(vars, child);
}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;

	if (!vars->cmd)
		return ;
	current_cmd = vars->cmd;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (current_cmd)
	{
		if (!current_cmd->pipe)
			run_command_no_pipe(vars, current_cmd);
		else
			execute_with_pipe(vars, current_cmd);
	}
	if (count_heredoc > 0)
		unlink(".heredoc");
}
