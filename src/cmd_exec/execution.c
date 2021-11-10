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

void	run_command_builtin(t_vars *vars, t_command *current_cmd)
{
	char	*command;

	command = current_cmd->command[0];
	if (ft_strcmp(command, "cd") == 0)
		builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		builtin_echo(current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(vars, current_cmd);
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, current_cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, current_cmd);
}

void	run_command_non_builtin(t_envlist *envlist, t_command *current_cmd)
{
	char	*path;
	char	**env;

	env = envlist_to_char_array(envlist);
	path = get_command_path(envlist, current_cmd->command[0]);
	if (path != NULL)
	{
		if (execve(path, current_cmd->command, env) < 0)
		{
			perror(current_cmd->command[0]);
			free(path);
			exit(127);
		}
	}
	else
	{
		printf("%s: command not found\n", current_cmd->command[0]);
		free(path);
		exit(127);
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

void	launch_command(t_vars *vars, t_command *current_cmd, int input, int output, int to_close)
{
	pid_t	child;

	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		signal(SIGINT, sigchild);
		signal(SIGQUIT, sigchild);
		// redirection(vars, current_cmd);
		fd_dup_and_close(input, output);
printf("current_cmd fd[0]:%d\n", current_cmd->fd[0]);
		if (!to_close)
			close(to_close);
		run_command_and_exit(vars, current_cmd);
	}
	else
	{
		signal(SIGINT, sigmain);
		signal(SIGQUIT, sigmain);
		fd_close(input, output);
	}
}

void	execute_command(t_vars *vars)
{
	int			input;
	int			output;
	pid_t		child;
	t_command	*current_cmd;
	int			to_close;

	child = 0;
	output = 1;
	input = 0;
	current_cmd = vars->cmd;
	current_cmd->fd[0] = 0;
	to_close = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
	if (count_heredoc(vars) > 0)
		update_heredoc(vars);
	if (!current_cmd->pipe)
		run_command_no_pipe(vars, current_cmd);
	else
	{
		while (current_cmd->next != NULL)
		{
			pipe_and_launch_commands(vars, current_cmd, input, to_close);
			to_close = 0;
			input = current_cmd->fd[0];
	printf("input:%d\n", input);
			current_cmd = current_cmd->next;
		}
		launch_command(vars, current_cmd, input, output, to_close);
	}
		wait_loop(count_command(vars->cmd), child);
}
