#include "minishell.h"

char	*get_env_value(t_vars *vars, char *env_name)
{
	t_envlist *current_env;
	char	*value;

	current_env = vars->envp;
	value = NULL;
	while (current_env->next)
	{
		if (ft_strcmp(current_env->name, env_name) == 0 && current_env->value)
			value = ft_strdup(current_env->value);
		current_env = current_env->next;	
	}
	return (value);
}

char	**get_command_path(t_vars *vars, char *command)
{
	t_envlist	*current_env;
	char		**path_sep;
	char		*path;
	int			i;

	path = get_env_value(vars, "PATH");
printf("path:%s\n", path);
	if (path == NULL)
		perror("path invalid");
	path_sep = ft_split(path, ':');
	i = 0;
	while (path_sep[i] != NULL)
	{
		ft_append(&path_sep[i], "/");
		ft_append(&path_sep[i], command);
printf("path_sep[%d]: %s\n", i, path_sep[i]);
		i++;
	}
	return (path_sep);
}

int		envlist_count(t_vars *vars)
{
	int		count;
	t_envlist	*current_env;

	current_env = vars->envp;
	count = 0;
	while (current_env->next)
	{
		count++;
		current_env = current_env->next;
	}
	return (count);
}

char **envlist_to_char_array(t_vars *vars)
{
	char	**env_arr;
	char	*temp;
	t_envlist	*current_env;
	int		i;

	i = 0;
	current_env = vars->envp;
	env_arr = protected_malloc((envlist_count(vars) + 1), sizeof(char *));
	while (current_env->next)
	{
		env_arr[i] = ft_strjoin(current_env->name, "=");
		if (current_env->value)
			ft_append(&env_arr[i], current_env->value);
		i++;
		current_env = current_env->next;
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

void	run_command_builtin(t_vars *vars, t_token *current_token, t_command *current_cmd)
{
	char	**command;

	command = current_cmd->command;
	if (ft_strcmp(command[0], "cd") == 0)
		builtin_cd(vars, current_token);
	else if (ft_strcmp(command[0], "echo") == 0)
		builtin_echo(vars, current_token);
	else if (ft_strcmp(command[0], "env") == 0)
		builtin_env(vars);
	else if (ft_strcmp(command[0], "exit") == 0)
		builtin_exit();
	else if (ft_strcmp(command[0], "export") == 0)
		builtin_export(vars, current_token);
	else if (ft_strcmp(command[0], "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command[0], "unset") == 0)
		builtin_unset(vars, current_token);
}

void	run_command_non_builtin(t_vars *vars, t_command *current_cmd, char *const env[])
{
	char	**path_sep;
	int		i;

printf("cmd:%s\n", current_cmd->command[0]);
	path_sep = get_command_path(vars, current_cmd->command[0]);
	i = 0;
	while (path_sep[i])
	{
		execve(path_sep[i], current_cmd->command, env);
		i++;
	}
//	perror(current_cmd->command[0]);
}

void	child_one(t_vars *vars, t_command *current_cmd, t_token *current_token, char *const *env, int fd[2])
{
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	if (command_is_builtin(current_cmd->command) == TRUE)
		run_command_builtin(vars, current_token, current_cmd);
	else
		run_command_non_builtin(vars, current_cmd, env);
}

void	child_two(t_vars *vars, t_command *current_cmd, t_token *current_token, char *const *env, int fd[2])
{
	dup2(fd[0], 0);
	close(fd[0]);
	close(fd[1]);
	if (command_is_builtin(current_cmd->command) == TRUE)
		run_command_builtin(vars, current_token, current_cmd);
	else
		run_command_non_builtin(vars, current_cmd, env);
}

/*
pid_t	child_process(int no_av, int in, int fds[2], t_pipex *pipex)
{
	pid_t	child;

	child = fork();
	if (child < 0)
	{
		perror("Fork ");
		return (EXIT_FAILURE);
	}
	if (child == 0)
	{
		dup2(in, 0);
		dup2(fds[1], 1);
		close(in);
		close (fds[1]);
		close (fds[0]);
		if (exec_process(pipex->av[no_av], pipex->path, pipex->env) == -1)
			perror("execve ");
	}
	return (child);
}
*/

void	test_function_print_envarr(char **env, t_vars *vars)
{
	int	i = 0;
	while (env[i])
	{
		printf("env[%d]:%s\n", i, env[i]);
		i++;
	}
	printf("\n--------------[LINKED LIST ENV]----------------\n");
	envlist_print_all(vars->envp);
	printf("\n--------------[LINKED LIST ENV]----------------\n");

}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	t_token		*current_token;
	char *const *env;
	int		fd[2];
	int		fd2[2];
	int		child1;
	int		child2;
	int		status;
	
	env = envlist_to_char_array(vars);
	current_token = vars->first;
	current_cmd = vars->cmd;
	if (command_is_builtin(current_cmd->command) == FALSE || current_cmd->pipe == 1)
	{
		pipe(fd);
		if (pipe(fd) < 0)
			perror("pipe");
		child1 = fork();
		if (child1 < 0)
			perror("fork");
		if (child1 == 0)
			child_one(vars, current_cmd, current_token, env, fd);
	}
	else
		run_command_builtin(vars, current_token, current_cmd);
	if (child1)
		waitpid(child1, &status, 0);
}
