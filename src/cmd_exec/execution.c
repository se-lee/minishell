#include "minishell.h"

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
	if (execve(path, current_cmd->command, env) < 0)
		perror("execution failed");
}

void	launch_commands(t_vars *vars, t_command *current_cmd, int in, int out)
{
	pid_t	child;

	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		if (in != 0)
		{
			if (dup2(in, 0) < 0)
				perror("dup2");
			close(in);
		}
		if (out != 1)
		{
			if (dup2(out, 1) < 0)
				perror("dup2");
			close(out);
		}
		if (command_is_builtin(current_cmd->command) == TRUE)
		{
			run_command_builtin(vars, current_cmd);
			exit(0);
		}
		else
			run_command_non_builtin(vars->envp, current_cmd);
	}
	else
	{
		if (in != 0)
			close(in);
		if (out != 1)
			close(out);
	}
}

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

void	execute_pipe_commands(t_vars *vars)
{
	int			fd[2];
	int			in;
	int			out;
	int			i;
	int			status;
	pid_t		child;
	t_command	*current_cmd;

	out = 1;
	in = 0;
	current_cmd = vars->cmd;
	i = 0;
	if (command_is_builtin(current_cmd->command) == TRUE && current_cmd->pipe == 0)
		run_command_builtin(vars, current_cmd);
	else if (current_cmd->pipe == 0)
	{
		child = fork();
		if (child == 0)
			run_command_non_builtin(vars->envp, current_cmd);
		waitpid(child, &status, 0);
	}
 	else
	{
		while (i < count_command(vars->cmd) - 1)
		{
			print_commands(current_cmd);
			if (pipe(fd) < 0)
				perror("pipe");
			launch_commands(vars, current_cmd, in, fd[1]);
			in = fd[0];
			current_cmd = current_cmd->next;
			i++;
		}
		launch_commands(vars, current_cmd, in, out); //last command
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
