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
	wait_loop(count_command(vars->cmd), child);
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
		fd_dup_and_close(fds[0], fds[1]);
		redirection(vars, current_cmd);
		if (to_close != 0)
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
	input = 0;
	output = 1;
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
printf("current_cmd:%s		input:%d	output:%d   fd[0]:%d   fd[1]:%d\n", current_cmd->command[0], input, output, current_cmd->fd[0], current_cmd->fd[1]);
			to_close = 0;
			input = current_cmd->fd[0];
printf("                   input_1:%d   output_1:%d\n", input, output);
			current_cmd = current_cmd->next;
		}
		current_cmd->fd[1] = 1;
printf("(last) current_cmd:%s   input:%d   output:%d   fd[0]:%d   fd[1]:%d\n", current_cmd->command[0], input, output, current_cmd->fd[0], current_cmd->fd[1]);
		launch_commands(vars, current_cmd, (int [2]){input, output}, to_close);
		wait_loop(count_command(vars->cmd), child);
	}
}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;

	if (!vars->cmd)
		return ;
	current_cmd = vars->cmd;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (current_cmd)
		execute_with_or_without_pipe(vars, current_cmd);
}
