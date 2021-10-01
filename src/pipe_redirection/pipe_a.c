#include "minishell.h"

char	*find_cmd(t_vars *variable, char *cmd)
{
	int		i;
	char	*cmd_check;

	i = 0;
	cmd = ft_strjoin("/", cmd);
	while (variable->paths[i])
	{
		cmd_check = ft_strjoin(variable->paths[i], cmd);
		if (!access(cmd_check, X_OK))
		{
			free(cmd);
			return (cmd_check);
		}
		free(cmd_check);
		i++;
	}
	printf("%s: command not found\n", cmd + 1);
	free(cmd);
	exit(-1);
}

void	child_one(char *file1, char *cmd1, t_vars *variable)
{
	int		fd;
	char	*path_cmd;
	char	**call;

	call = ft_split(cmd1, ' ');
	path_cmd = find_cmd(variable, call[0]);
	fd = open(file1, O_RDWR);
	if (fd == -1)
	{
		printf("No such file exist or you have the wrong permission\n");
		exit(-1);
	}
	dup2(fd, 0);
	close(fd);
	close(variable->fd[0]);
	dup2(variable->fd[1], 1);
	close(variable->fd[1]);
	execve(path_cmd, call, variable->envp);
}

void	child_two(char *file2, char *cmd2, t_vars *variable)
{
	int		fd;
	char	*path_cmd;
	char	**call;

	call = ft_split(cmd2, ' ');
	path_cmd = find_cmd(variable, call[0]);
	fd = open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		exit(-1);
	dup2(fd, 1);
	close(fd);
	close(variable->fd[1]);
	dup2(variable->fd[0], 0);
	close(variable->fd[0]);
	execve(path_cmd, call, variable->envp);
}

void	pipex(t_vars *variable, char *cmd1, char *cmd2)
{
	int	status;
	int	child1;
	int	child2;

	pipe(variable->fd);
	child1 = fork();
	if (child1 < 0)
		exit(-1);
	else if (child1 == 0)
		child_one(variable->file1, cmd1, variable);
	child2 = fork();
	if (child2 < 0)
		exit(-1);
	else if (child2 == 0)
		child_two(variable->file2, cmd2, variable);
	close(variable->fd[0]);
	close(variable->fd[1]);
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	variable;

	variable.envp = envp;
	if (argc == 5)
	{
//		parsing(argv, envp, &variable);
		variable.file1 = argv[1];
		variable.file2 = argv[4];
		pipex(&variable, argv[2], argv[3]);
	}
	else
		printf("Wrong number of arguments\n");
	return (0);
}
