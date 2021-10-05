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

char	*get_command_path(t_envlist *envp, char *command)
{
	char		**path_sep;
	char		*path;
	int			i;

	path = get_env_value(envp, "PATH");
//printf("path:%s\n", path);
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
//printf("path_sep:%s\n", path_sep[i]);
		if (access(path_sep[i], X_OK) == 0)
		{
			path = path_sep[i];
			return(path);  //<!> the path_sep[i] after this will remain unfree-ed <!>
		}
		free(path_sep[i]);
		i++;
	}
	perror("invalid path");
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

void	run_command_non_builtin(t_envlist *envlist, t_command *current_cmd, char **env)
{
	char	*path;

	path = get_command_path(envlist, current_cmd->command[0]);
// printf("path:%s\n", path);
// for(int i = 0; current_cmd->command[i]; i++)
// 		printf("cmd[%d]:%s\n", i, current_cmd->command[i]);
	if (execve(path, current_cmd->command, env) < 0)
		perror("execution failed");
}

void	child_one(t_vars *vars, t_command *current_cmd, t_token *current_token, 
		int fd[2], char **env)
{
	t_envlist *envlist;

	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);

	envlist = vars->envp;
	if (command_is_builtin(current_cmd->command) == TRUE)
		run_command_builtin(vars, current_token, current_cmd); // <!> to be merged Adam's changes
	else
		run_command_non_builtin(envlist, current_cmd, env);
}

void	child_two(t_vars *vars, t_command *current_cmd, t_token *current_token,
		int fd[2], char **env)
{
	t_envlist	*envlist;

	dup2(fd[0], 0);
	close(fd[0]);
	close(fd[1]);

	envlist = vars->envp;
	if (command_is_builtin(current_cmd->command) == TRUE)
		run_command_builtin(vars, current_token, current_cmd); // <!> to be merged Adam's changes
	else
		run_command_non_builtin(envlist, current_cmd, env);
}

/* pid_t	child_process(int no_av, int in, int fds[2], t_pipex *pipex)
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
	printf("\n--------------[CHAR ** ENV]----------------\n");
	while (env[i])
	{
		printf("env[%d]:%s\n", i, env[i]);
		i++;
	}
	printf("\n--------------[LINKED LIST ENV]----------------\n");
	envlist_print_all(vars->envp);
	printf("\n-----------------------------------------------\n");
}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	t_token		*current_token;
	char **env;
	int		fd[2];
	pid_t	child1;
	pid_t	child2;
	int		status;
	
	current_token = vars->first;
	current_cmd = vars->cmd;
	env = envlist_to_char_array(vars->envp);
	if (command_is_builtin(current_cmd->command) == FALSE || current_cmd->pipe > 0)
	{
		pipe(fd);
		if (pipe(fd) < 0)
			perror("pipe");
		child1 = fork();
		if (child1 < 0)
			perror("fork");
		if (child1 == 0)
			child_one(vars, current_cmd, current_token, fd, env);
	}
	else
		run_command_builtin(vars, current_token, current_cmd);
	if (child1)
		waitpid(child1, &status, 0);
	if (child2)
		waitpid(child2, &status, 0);
}
