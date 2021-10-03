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

//i dont like this name; rename later
void	run_command(t_vars *vars, char **command, t_token *current_token, char **envp)
{
	if (ft_strcmp(command[0], "cd") == 0)
		builtin_cd(vars, current_token, envp);
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
	else
		execute_other_cmd(vars, envp);
}

// envp argument to be removed later
void	child_process(t_vars *vars, char **command, t_token *current_token, char **envp, int fd[2])
{
	dup2(fd[0 ], 0);
	close(fd[0]);
	close(fd[1]);
	run_command(vars, command, current_token, envp);
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
	char	**command;
	t_token	*current_token;
	int		fd[2];
	int		child1;
	int		status;

	command = vars->cmd->command;
	current_token = vars->first;
	pipe(fd);
	if (pipe(fd) < 0)
		perror("pipe");
	child1 = fork();
	if (child1 < 0)
		perror("fork");
	if (child1 == 0)
		child_process(vars, command, current_token, envp, fd);
	
	waitpid(child1, &status, 0);
}
