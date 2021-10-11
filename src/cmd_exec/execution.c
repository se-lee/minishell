#include "minishell.h"

char	*get_env_value(t_envlist *envp, char *env_name)
{
	t_envlist *current_env;
	char	*value;

	current_env = envp;
	value = NULL;
	while (current_env)
	{
		if (ft_strcmp(current_env->name, env_name) == 0 && current_env->value)
			value = ft_strdup(current_env->value);
		current_env = current_env->next;	
	}
	return (value);
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

char **envlist_to_char_array(t_envlist *envp)
{
	char	**env_arr;
	char	*temp;
	t_envlist	*current_env;
	int		i;

	i = 0;
	current_env = envp;
	env_arr = protected_malloc((envlist_count(envp) + 1), sizeof(char *));
	while (current_env)
	{
		env_arr[i] = ft_strjoin(current_env->name, "=");
		if (current_env->value)
			ft_append(&env_arr[i], current_env->value);
		current_env = current_env->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
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
		builtin_exit();
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

	path = get_command_path(envlist, current_cmd->command[0]);
	env = envlist_to_char_array(envlist);
	if (execve(path, current_cmd->command, env) < 0)
		perror("execution failed");
}

/*manon*/
void	launch_commands(t_vars *vars, t_command *current_cmd, int in, int out)
{
	pid_t	child;

	child = fork();
	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		if (in != 0) //in = 0
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
		close(in);
		close(out);
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
			if (pipe(fd) < 0)
				perror("pipe");
			launch_commands(vars, current_cmd, in, fd[1]);
			in = fd[0];
			current_cmd = current_cmd->next;
			i++;
		}
		launch_commands(vars, current_cmd, in, out); //last command
		i = 0;
		while (i < count_command(vars->cmd)) //wait for all sons' pid)
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

// void	execute_command(t_vars *vars)
// {
// 	t_command	*current_cmd;
// 	int			status;
// 	pid_t		child;
// 	int			cmd_count;
// 	int			i;

// 	current_cmd = vars->cmd;
// 	cmd_count = count_command(vars->cmd);
// 		i = 0;
// 	if (command_is_builtin(current_cmd->command) == FALSE && current_cmd->pipe > 0)
// 	{
// printf("pipe:%d\n", current_cmd->pipe);
// printf("count:%d\n", count_command(vars->cmd));
// 		while (i < cmd_count - 1)
// 		{
// 			child_processes(vars, current_cmd, i, cmd_count);
// 			if (current_cmd->next)
// 				current_cmd = current_cmd->next;
// 			i++;
// 		}
// 	}
// 	else if (command_is_builtin(current_cmd->command) == FALSE)
// 	{
// 		child = fork();
// 		if (child == 0)
// 			run_command_non_builtin(vars->envp, current_cmd);
// 		waitpid(child, &status, 0);
// 	}
// 	else
// 		run_command_builtin(vars, current_cmd);
// 	// waitpid(child, &status, 0);
// }
