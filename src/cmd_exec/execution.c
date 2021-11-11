#include "minishell.h"

/* remove this function later */
void	print_commands(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd->command[i])
	{
		printf(">>%s<<\n", cmd->command[i]);
		i++;
	}
}

void	run_command_no_pipe(t_vars *vars, t_command *current_cmd)
{
	pid_t	child;

	child = 0;
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		if (vars->in || vars->out)
		{
			child = fork();
			if (child == 0)
				redirect_and_run_cmd(vars, current_cmd, TRUE);
		}
		else
			run_command_builtin(vars, current_cmd);
	}
	else
	{
		child = fork();
		if (child == 0)
			redirect_and_run_cmd(vars, current_cmd, FALSE);
	}
	waitpid(child, NULL, 0);
}

void	launch_commands(t_vars *vars, t_command *current_cmd,
		int fds[2], int to_close)
{
	pid_t	child;

	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		signal(SIGINT, sigchild);
		signal(SIGQUIT, sigchild);
		redirection(vars, current_cmd);
		fd_dup_and_close(fds[0], fds[1]);
		if (to_close)
			close(to_close);
		run_command_and_exit(vars, current_cmd);
	}
	else
	{
		signal(SIGINT, sigmain);
		signal(SIGQUIT, sigmain);
		if (fds[0] != 0)
			close(fds[0]);
		if (fds[1] != 1)
			close(fds[1]);
	}
}

void	execute_with_or_without_pipe(t_vars *vars, t_command *current_cmd)
{
	int			input;
	int			output;
	pid_t		child;
	int			to_close;

	child = 0;
	output = 1;
	input = 0;
	to_close = 0;
	if (!current_cmd->pipe)
		run_command_no_pipe(vars, current_cmd);
	else
	{
		if (current_cmd->next == NULL)
			pipe_get_next_cmd(current_cmd);
		while (current_cmd->next != NULL)
		{
			pipe_and_launch_command(vars, current_cmd, input, to_close);
			to_close = 0;
			input = current_cmd->fd[0];
			current_cmd = current_cmd->next;
		}
		launch_commands(vars, current_cmd, (int [2]){input, output}, to_close);
		wait_loop(count_command(vars->cmd), child);
	}
}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;

	current_cmd = vars->cmd;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (count_heredoc(vars) > 0)
		update_heredoc(vars);
	execute_with_or_without_pipe(vars, current_cmd);
}
