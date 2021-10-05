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
	result = ft_strjoin(*a, str);
	free(*a);
	*a = result;
}

int	count_number_str_in_list(t_vars *vars)
{
	int		count;
	t_token	*index;

	index = vars->first;
	count = 0;
	while (index)
	{
		if (index->token_type == WORD)
			count++;
		index = index->next;
	}
	return (count);
}

static char	**get_command(t_vars *vars, t_token *current_token)
{
	char	**command;
	int		str_count;
	int		i;

	str_count = count_number_str_in_list(vars);
	command = malloc(sizeof(char *) * (str_count + 1));
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		if (current_token->token_type == WORD)
		{
			command[i] = ft_strdup(current_token->buffer.str);
			i++;
		}
		current_token = current_token->next;
	}
	command[i] = NULL;
	return (command);
}

char	*get_command_path(t_envlist *envp, char *command)
{
	char	**path_sep;
	char	*path;
	int		i;

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

	current_token = vars->first;
	command_arr = get_command(vars, current_token);
	path_sep = get_command_path(vars, command_arr[0]);
	i = 0;
	current_env = envp;
	env_arr = protected_malloc((envlist_count(envp) + 1), sizeof(char *));
	while (current_env)
	{
		env_arr[i] = ft_strjoin(current_env->name, "=");
		if (current_env->value)
			ft_append(&env_arr[i], current_env->value);
		current_env = current_env->next;
		tcsetattr(STDIN_FILENO, TCSANOW, &vars->saved_termios);
		execve(path_sep[i], command_arr, envp);
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
	char		*command;
	t_command	*current_cmd;

	command = vars->first->buffer.str;
	current_token = vars->first;
	if (ft_strcmp(command, "cd") == 0)
		builtin_cd(vars, vars->cmd);
	else if (ft_strcmp(command, "echo") == 0)
		builtin_echo(vars, vars->cmd);
	else if (ft_strcmp(command, "env") == 0)
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
		int fd[2], int	fd2[2], char **env)
{
	t_envlist *envlist;

	dup2(fd[0], 0);
	dup2(fd2[1], 1);
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

	dup2(fd[1], 1);
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


void	test_func_print_commands(t_command *current_cmd)
{
	while (current_cmd)
	{
		printf("cmd:%s\n", current_cmd->command[0]);
		current_cmd = current_cmd->next;
	}
}

void	execute_command(t_vars *vars)
{
	t_command	*current_cmd;
	t_token		*current_token;
	char **env;
	int		fd[2];
	int		fd2[2];
	pid_t	child1;
	pid_t	child2;
	int		status;
	
	current_token = vars->first;
	current_cmd = vars->cmd;
	env = envlist_to_char_array(vars->envp);

test_func_print_commands(current_cmd);

	if (command_is_builtin(current_cmd->command) == FALSE || current_cmd->pipe > 0)
	{
		while (current_cmd)
		{
			pipe(fd);
			if (pipe(fd) < 0)
				perror("pipe");
			child1 = fork();
			if (child1 < 0)
				perror("fork");
			if (child1 == 0)
				child_one(vars, current_cmd, current_token, fd, fd2, env);
		current_cmd = current_cmd->next;
printf("next cmd:%s\n", current_cmd->command[0]);
		current_token = current_token->next;
			if (current_cmd->pipe > 0)
			{
				pipe(fd2);
				if (pipe(fd2) < 0)
					perror("pipe2");
				child2 = fork();
				if (child2 < 0)
					perror("fork2");
				if (child2 == 0)
					child_two(vars, current_cmd, current_token, fd2, env);
			}
		}
	}
	else if (ft_strcmp(command, "export") == 0)
		builtin_export(vars, vars->cmd);
	else if (ft_strcmp(command, "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(command, "unset") == 0)
		builtin_unset(vars, vars->cmd);
	else
		run_command_builtin(vars, current_token, current_cmd);
	if (child1)
		waitpid(child1, &status, 0);
}
