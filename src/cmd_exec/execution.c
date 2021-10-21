#include "minishell.h"

void	run_command_no_pipe(t_vars *vars, t_command *current_cmd);
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

int		envlist_count(t_envlist *envp)
{
	int		count;
	t_envlist	*current_env;

	current_env = envp;
	count = 0;
	while (current_env)
	{
		count++;
		current_env = current_env->next;
	}
	return (count);
}

void	run_command_builtin(t_vars *vars, t_command *current_cmd)
{
	char	*command;

	command = current_cmd->command[0];
	if (ft_strcmp(command, "cd") == 0)
		builtin_cd(vars, current_cmd);
	else if (ft_strcmp(command, "echo") == 0)
		builtin_echo(vars, current_cmd);
	else if (ft_strcmp(command, "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command, "exit") == 0)
		builtin_exit(current_cmd);
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
			perror("");
			exit(127);
		}
	}
	else
	{
		perror("");
		exit(127);
	}
}

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

void	launch_commands(t_vars *vars, t_command *current_cmd, int input, int output)
{
	pid_t	child;

	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		fd_dup_and_close(input, output);
		if (command_is_builtin(current_cmd->command) == TRUE)
		{
			redirection(current_cmd);
			run_command_builtin(vars, current_cmd);
			exit(0);
		}
		else
		{
			redirection(current_cmd);
			run_command_non_builtin(vars->envp, current_cmd);
		}
	}
	else
	{
		if (input != 0)
			close(input);
		if (output != 1)
			close(output);
	}
}

void	execute_pipe_commands(t_vars *vars)
{
	int			fd[2];
	int			input;
	int			output;
	int			i;
	int			status;
	pid_t		child;
	t_command	 *current_cmd;

	output = 1;
	input = 0;
	current_cmd = vars->cmd;
	i = 0;
	if (!current_cmd->pipe)
		run_command_no_pipe(vars, current_cmd);
 	else
	{
		while (i < count_command(vars->cmd) - 1)
		{
			if (pipe(fd) < 0)
				perror("pipe");
			redirection(current_cmd);
			launch_commands(vars, current_cmd, input, fd[1]);
			input = fd[0];
			current_cmd = current_cmd->next;
			i++;
		}
		redirection(current_cmd);
		launch_commands(vars, current_cmd, input, output); //last command
		i = 0;
		while (i < count_command(vars->cmd))
		{
			wait(&status);
			if (status == -1)
			{
				perror("wait");
				return ;
			}
			i++;
		}
	}
}

void	run_command_no_pipe(t_vars *vars, t_command *current_cmd)
{
	pid_t	child;

	if (command_is_builtin(current_cmd->command) == TRUE)
	{
		if (current_cmd->redirect_right > 0 || current_cmd->redirect_left > 0)
		{
			child = fork();
			if (child == 0)
			{
				redirection(current_cmd);
				run_command_builtin(vars, current_cmd);
				exit(0);
			}
		}
		else
			run_command_builtin(vars, current_cmd);
	}
	else
	{
		child = fork();
		if (child == 0)
		{
			redirection(current_cmd);
			run_command_non_builtin(vars->envp, current_cmd);
		}
	}
	waitpid(child, NULL, 0);
}
