#include "minishell.h"

void	run_command_and_exit(t_vars *vars, t_command *current_cmd)
{
	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		printf("1\n");
		run_command_builtin(vars, current_cmd);
		exit(0);
	}
	else
	{
		run_command_non_builtin(vars->envp, current_cmd);
		exit(0);
	}
}

void	redirect_and_run_cmd(t_vars *vars, t_command *current_cmd, int builtin)
{
	if (builtin == TRUE)
	{
		redirection(vars, current_cmd);
		run_command_builtin(vars, current_cmd);
		exit(0);
	}
	else
	{
		redirection(vars, current_cmd);
		run_command_non_builtin(vars->envp, current_cmd);		
	}
}

void	fork_and_run_command(t_vars *vars, t_command *current_cmd, int to_close)
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
		fd_dup_and_close(current_cmd->fd[0], current_cmd->fd[1]);
		if (to_close)
			close(to_close);
		run_command_and_exit(vars, current_cmd);
	}
	else
	{
		signal(SIGINT, sigmain);
		signal(SIGQUIT, sigmain);
		fd_close(current_cmd->fd[0], current_cmd->fd[1]);
	}
}

/* void	run_command_with_pipe(t_vars *vars, t_command *current_cmd)
{
	int		i;
	int		output;
	int		input;
	int		to_close;
	
	i = 0;
	output = 1;
	input = 0;
	to_close = 0;
	while (i < count_command(vars->cmd) - 1)
	{
		if (pipe(current_cmd->fd) < 0)
			perror ("pipe");
		if (!to_close)
			to_close = current_cmd->fd[0];
		fork_and_run_command(vars, current_cmd, input, output, to_close);
		to_close = 0;
		input = current_cmd->fd[0];
		current_cmd = current_cmd->next;
		i++;
	}
	fork_and_run_command(vars, current_cmd, input, output, to_close);
}
*/

void	pipe_and_fork(t_vars *vars, t_command *current_cmd)
{
	int		to_close;
	int		input;
	int		output;
	pid_t	child;

	to_close = 0;
	input = 0;
	output = 1;
	if (pipe(current_cmd->fd) < 0)
		perror ("pipe");
	if (!to_close)
		to_close = current_cmd->fd[0];
	child = fork();
	if (child == 0)
	{
		signal(SIGINT, sigchild);
		signal(SIGQUIT, sigchild);
		redirection(vars, current_cmd);
		fd_dup_and_close(input, output);
		if (to_close)
			close(to_close);
		run_command_and_exit(vars, current_cmd);
		to_close = 0;
		input = current_cmd->fd[0];
	}
	else
	{
		signal(SIGINT, sigmain);
		signal(SIGQUIT, sigmain);
		fd_close(input, output);
	}
}


void	child_process(t_vars *vars, t_command *current_cmd, int to_close)
{
	signal(SIGINT, sigchild);
	signal(SIGQUIT, sigchild);
	redirection(vars, current_cmd);
	fd_dup_and_close(current_cmd->fd[0], current_cmd->fd[1]);
	if (to_close)
		close(to_close);
	run_command_and_exit(vars, current_cmd);
}

/*
void	run_command_with_pipe(t_vars *vars, t_command *current_cmd)
{
	int		i;
	// int		output;
	// int		input;
	int		to_close;
	pid_t	child;
	
	i = 0;
	// output = 1;
	// input = 0;
	to_close = 0;
	child = 0;
	while (i < count_command(vars->cmd) - 1)
	{
		if (pipe(current_cmd->fd) < 0)
			perror ("pipe");
		if (!to_close)
			to_close = current_cmd->fd[0];
		fork_and_run_command(vars, current_cmd, to_close);
		to_close = 0;
		// input = current_cmd->fd[0];
		current_cmd = current_cmd->next;
		i++;
	}
	fork_and_run_command(vars, current_cmd, to_close);
	i = 0;
	while (i < count_command(vars->cmd))
	{
		waitpid(child, NULL, 0);
		i++;
	}
}
*/

void	run_command_with_pipe(t_vars *vars, t_command *current_cmd)
{
	pid_t	child;
	int		input;
	int		output;
	int		i;
	int		to_close;

	i = 0;
	input = 0;
	output = 1;
	to_close = 0;
	while (i < count_command(vars->cmd) - 1)
	{
		if (pipe(current_cmd->fd) < 0)
			perror("pipe");
		if (!to_close)
			to_close = current_cmd->fd[0];
		child = fork();
		if (child == 0)
			child_process(vars, current_cmd, to_close);
		else
		{
			signal(SIGINT, sigmain);
			signal(SIGQUIT, sigmain);
			fd_close(current_cmd->fd[0], current_cmd->fd[1]);			
		}
		to_close = 0;
		input = current_cmd->fd[0];
		current_cmd = current_cmd->next;
		i++;
	}
	child_process(vars, current_cmd, to_close);
			i = 0;
	while (i < count_command(vars->cmd))
	{
		waitpid(child, NULL, 0);
		i++;
	}
}

void execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	int			i;
	// pid_t		child;

	i = 0;
	// child = 0;
	current_cmd = vars->cmd;
	vars->cmd->fd[0] = 0;
	vars->cmd->fd[1] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (count_heredoc(vars) > 0)
		update_heredoc(vars);
	if (!current_cmd->pipe)
		run_command_no_pipe(vars, current_cmd);
	else
		run_command_with_pipe(vars, current_cmd);
}
