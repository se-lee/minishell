#include "minishell.h"

/*
Signals : ctrl-D
*/

char	*get_delimiter(t_redirect *current_in)
{
	char	*delimiter;

	if (current_in->arrow_num == 2 && current_in->filename)
		delimiter = ft_strdup(current_in->filename);
	else
		delimiter = NULL;
	return (delimiter);
}

int		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;
	pid_t	child;

	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	delimiter = get_delimiter(current_in);
	if (fd < 0)
		perror("heredoc_file");
	child = fork();
	if (child == 0)
	{
		line = readline("> ");
		while (ft_strcmp(line, delimiter))
		{
			ft_putendl_fd(line, fd);
			free(line);
			line = readline("> ");
		}
	// printf("fd_heredoc:%d\n", fd);
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("dup2");
		close(fd);
		exit(0);
	}
	else
		waitpid(child, NULL, 0);
	return (0);
}

int		redirect_heredoc(t_redirect *current_in)
{
	int		fd;

	fd = open(".heredoc", O_RDONLY);
// printf("redirect_heredoc_file:%s\n", current_in->filename);
	if (fd < 0)
		perror(".heredoc");
// printf("fd_in1:%d\n", fd);
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

// void		heredoc(t_redirect *current_in)
// {
// 	int		fd;
// 	char	*delimiter;
// 	char	*line;

// 	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
// 	delimiter = get_delimiter(current_in);
// 	if (fd < 0)
// 		perror("heredoc_file");
// 	line = readline("> ");
// 	while (ft_strcmp(line, delimiter))
// 	{
// 		ft_putendl_fd(line, fd);
// 		free(line);
// 		line = readline("> ");
// 	}
// printf("fd_heredoc:%d\n", fd);
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 		perror("dup2");
// 	close(fd);
// 	exit(0);
// }
