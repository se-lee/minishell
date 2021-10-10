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

void	free_array(char **array)
{
    int    i;

    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char	*get_command_path(t_envlist *envp, char *command)
{
	char	**path_sep;
	char	*path;
	int		i;

	path = get_env_value(envp, "PATH");
	if (path == NULL)
	{
		perror("path invalid");
		free(path);
	}
	path_sep = ft_split(path, ':');
	free(path);
	i = 0;
	while (path_sep[i])
	{
		ft_append(&path_sep[i], "/");
		ft_append(&path_sep[i], command);
		if (access(path_sep[i], X_OK) == 0)
		{
			path = path_sep[i];
			// free_array(path_sep);
			return(path);
		}
		free(path_sep[i]);
		i++;
	}
	perror("invalid path");
	free_array(path_sep);
	return (NULL);
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

int		command_is_builtin(char **command)
{
	if ((ft_strcmp(command[0], "cd") == 0)
		|| (ft_strcmp(command[0], "echo") == 0)
		|| (ft_strcmp(command[0], "env") == 0)
		|| (ft_strcmp(command[0], "exit") == 0)
		|| (ft_strcmp(command[0], "export") == 0)
		|| (ft_strcmp(command[0], "pwd") == 0)
		|| (ft_strcmp(command[0], "unset") == 0))
		return (TRUE);
	else
		return (FALSE);
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

/*void	child_process(t_vars *vars, t_command *current_cmd, char **env)
{
	t_envlist	*envlist;
	pid_t		child;

	envlist = vars->envp;
	child = fork();

	if (child < 0)
		perror("fork");
	if (child == 0)
	{
		if (current_cmd->prev)
		{
			pipe_flow(current_cmd->prev->fd, 1);
		}
		pipe_flow(current_cmd->fd, 0);
		if (command_is_builtin(current_cmd->command) == TRUE)
		{
			run_command_builtin(vars, current_cmd);
//			exit(0);
		}
		else
			run_command_non_builtin(envlist, current_cmd, env);
	}
}*/

int		count_command(t_command *cmd)
{
	int			count;
	t_command	*current_cmd;

	current_cmd = cmd;
	count = 0;
	while (current_cmd)
	{
		count++;
		current_cmd = current_cmd->next;
	}
	return (count);
}
/*
arsene
void    child_command(t_vars *vars, t_command *current_cmd, int in, int out)
{
	pid_t	child;

	child = fork();
		if (child < 0)
		{
			perror("fork");
			return ;
		}
	if (child == 0)
	{
		if (in)
		{
			dup2(in, 0);
			close(in);
		}
		if (out != 1) 
		{
			dup2(out, 1);
			close(out);
		}
		if (command_is_builtin(current_cmd->command) == TRUE)
			run_command_builtin(vars, current_cmd);
		else
		{
			run_command_non_builtin(vars->envp, current_cmd);
		}
	}
}*/

void	child_processes(t_vars *vars, int cmd_count)
{
	t_command	*current_cmd;
	t_envlist	*envlist;
	int		i;
	pid_t	child;

	current_cmd = vars->cmd;
	envlist = vars->envp;
	i = 0;
	while (i < cmd_count)
	{
		if (pipe(current_cmd->fd) < 0)
			perror("pipe");
		child = fork();
		if (child == 0)
		{
			if (i == 0) //first commands
			{
				dup2(current_cmd->fd[1], 1);
				close(current_cmd->fd[0]);
				close(current_cmd->fd[1]);
			}
			else if (i == cmd_count - 1 && cmd_count != 1) //last command
			{
				dup2(current_cmd->prev->fd[0], 0);
				close(current_cmd->prev->fd[0]);
				close(current_cmd->prev->fd[1]);
			}
			else //middle command
			{
				dup2(current_cmd->prev->fd[0], 0);
				dup2(current_cmd->prev->fd[1], 1);
				close(current_cmd->fd[0]);
				close(current_cmd->fd[1]);			
				close(current_cmd->prev->fd[0]);
				close(current_cmd->prev->fd[1]);			
			}
			if (command_is_builtin(current_cmd->command) == TRUE)
				run_command_builtin(vars, current_cmd);
			else
				run_command_non_builtin(envlist, current_cmd);
		}
		if (current_cmd->next)
			current_cmd = current_cmd->next;
		i++;
	}
	waitpid(child, )
}


// void	child_processes(t_vars *vars, int cmd_count)
// {
// 	t_command	*current_cmd;
// 	t_envlist	*envlist;
// 	int		i;
// 	pid_t	child;

// 	current_cmd = vars->cmd;
// 	envlist = vars->envp;
// 	i = 0;
// 	vars->fd = protected_malloc(cmd_count, sizeof(int *));
// 	while(i < cmd_count - 1)
// 	{
// 		pipe(vars->fd[i]);
// 	printf("fd[%d]:%d\n", i, vars->fd[i][0]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < cmd_count)
// 	{
// 		child = fork();
// 		if (child == 0)
// 		{
// 			if (i == 0) //first commands
// 			{
// 				dup2(vars->fd[i][1], 1);
// 				close(vars->fd[i][0]);
// 				close(vars->fd[i][1]);
// 			}
// 			else if (i == cmd_count - 1) //last command
// 			{
// 				dup2(vars->fd[i - 1][0], 0);
// 				close(vars->fd[i - 1][0]);
// 				close(vars->fd[i - 1][1]);
// 			}
// 			else //middle command
// 			{
// 				dup2(vars->fd[i - 1][0], 0);
// 				dup2(vars->fd[i][1], 1);
// 				close(vars->fd[i][0]);
// 				close(vars->fd[i][1]);			
// 				close(vars->fd[i - 1][0]);
// 				close(vars->fd[i - 1][1]);			
// 			}
// 			if (command_is_builtin(current_cmd->command) == TRUE)
// 				run_command_builtin(vars, current_cmd);
// 			else
// 				run_command_non_builtin(envlist, current_cmd);
// 		}
// 		i++;
// 	}
// }

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	int			status;
	int			cmd_count;
	int			i = 0;

	current_cmd = vars->cmd;
	cmd_count = count_command(vars->cmd);
	if (command_is_builtin(current_cmd->command) == FALSE || current_cmd->pipe > 0)
		child_processes(vars, cmd_count);
	else
		run_command_builtin(vars, current_cmd);
}
