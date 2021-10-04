#include "minishell.h"

void	ft_append(char **a, char *str)
{
	char	*result;

	result = ft_strjoin(*a, str);
	free(*a);
	*a = result;
}

int		count_number_str_in_list(t_vars *vars)
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
	char **command;
	int	str_count;
	int	i;

	str_count = count_number_str_in_list(vars);
	command = malloc(sizeof(char *) * (str_count + 1));
	i = 0;
	while (current_token && ft_piperedirect(current_token->token_type) == 0)
	{
		if (current_token->token_type == WORD)
		{
			command[i] = ft_strdup(current_token->buffer.str);
//		printf("command[%d]:%s\n", i, command[i]);
			i++;
		}
		current_token = current_token->next;
	}
	command[i] = NULL;
//	printf("command[%d]:%s\n", i, command[i]);
	return (command);
}

static char	**get_command_path(t_vars *vars, char *command)
{
	char		**path_sep;
	char		*path;
	int			i;

	path = getenv("PATH");
	if (path == NULL)
		perror("path invalid");
	path_sep = ft_split(path, ':');
	i = 0;
	while (path_sep[i] != NULL)
	{
		ft_append(&path_sep[i], "/");
		ft_append(&path_sep[i], command);
		i++;
	}
	return (path_sep);
}

/*
void	execute_other_cmd(t_vars *vars, char **envp)
{
	t_token	*current_token;
	char	**command_arr;
	char	**path_sep;
	int		i;

	current_token = vars->first;
	command_arr = get_command(vars, current_token);
	path_sep = get_command_path(vars, command_arr[0]);

	i = 0;
	while (path_sep[i])
	{
		execve(path_sep[i], command_arr, envp);
		i++;
	}
	perror(command_arr[0]);
}
*/

int		envlist_count(t_vars *vars)
{
	int		count;
	t_envlist	*current_envlist;

	current_envlist = vars->envp;
	count = 0;
	while (current_envlist)
	{
		count++;
		current_envlist = current_envlist->next;
	}
	return (count);
}

char **envlist_to_char_array(t_vars *vars)
{
	char	**env_arr;
	char	*temp;
	t_envlist	*current_envlist;
	int		i;

	i = 0;
	env_arr = protected_malloc((envlist_count(vars) + 1), sizeof(char *));
	if (current_envlist)
	{
		temp = ft_strjoin(current_envlist->name, "=");
		env_arr[i] = ft_strjoin(temp, current_envlist->value);
		free(temp);
		i++;
		current_envlist = current_envlist->next;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

void	execute_other_cmd(t_vars *vars, t_command *current_cmd)
{
	t_token	*current_token;
	char	**env;
	char	**path_sep;
	int		i;

	current_token = vars->first;
	path_sep = get_command_path(vars, current_cmd->command[0]);
	env = envlist_to_char_array(vars);
	i = 0;
	while (path_sep[i])
	{
		execve(path_sep[i], current_cmd->command, env);
		i++;
	}
	perror(current_cmd->command[0]);
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

void		run_command_builtin(t_vars *vars, char **command, t_token *current_token, char **envp)
{
	if (ft_strcmp(command[0], "cd") == 0)
		builtin_cd(vars, current_token, envp);
	else if (ft_strcmp(command[0], "echo") == 0)
	if (ft_strcmp(command, "cd") == 0)
		builtin_cd(vars, current_token);
	else if (ft_strcmp(command, "echo") == 0)
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

// envp argument to be removed later
void	child_one(t_vars *vars, t_command *current_cmd, t_token *current_token, char **env, int fd[2])
{
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	if (command_is_builtin(current_cmd->command) == TRUE)
		run_command_builtin(vars, current_cmd->command, current_token, env);
	else
		execute_other_cmd(vars, current_cmd);
}

void	child_two(t_vars *vars, t_command *current_cmd, t_token *current_token, char **env, int fd[2])
{
	dup2(fd[0], 0);
	close(fd[0]);
	close(fd[1]);
	if (command_is_builtin(current_cmd->command) == TRUE)
		run_command_builtin(vars, current_cmd->command, current_token, env);
	else
		execute_other_cmd(vars, current_cmd);
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

void	execute_command(t_vars *vars, char **envp)
{
	t_command	*current_cmd;
	t_token		*current_token;
	int		fd[2];
	int		fd2[2];
	int		child1;
	int		child2;
	int		status;
	
	current_token = vars->first;
	current_cmd = vars->cmd;
printf("pipe: %d\n", current_cmd->pipe);
	if (command_is_builtin(current_cmd->command) == FALSE || current_cmd->pipe == 1)
	{
printf("cmd[0]: %s\n", current_cmd->command[0]);
printf("builtin: %d\n", command_is_builtin(current_cmd->command));
		pipe(fd);
		if (pipe(fd) < 0)
			perror("pipe");
		child1 = fork();
		if (child1 < 0)
			perror("fork");
		if (child1 == 0)
			child_one(vars, current_cmd, current_token, envp, fd);
	}
	else
		run_command_builtin(vars, current_cmd->command, current_token, envp);
	// if (child1)
	// 	waitpid(child1, &status, 0);
}

// void	execute_command(t_vars *vars, char **envp)
// {
// 	t_token *current_token;
// 	int	fd[2][2];
// 	int		child1;
// 	int		child2;
// 	int		status;
	
// 	current_token = vars->first;
// 	pipe(fd[0]);
// 	if (pipe(fd[0]) < 0)
// 		perror("pipe");
// 	child1 = fork();
// 	if (child1 < 0)
// 		perror("fork");
// 	if (child1 == 0)
// 		child_one(vars, vars->cmd->command, current_token, envp, &fd[2]);
// 	if (vars->cmd->pipe == 1)
// 	{
// 		child2 = fork();
// 		if (child2 == 0)
// 		child_two(vars, vars->cmd->command, current_token, envp, &fd[2]);
// 	}
// 	waitpid(child1, &status, 0);
// 	waitpid(child1, &status, 0);
// }