#include "minishell.h"

char	*get_delimiter(t_redirect *current_in)
{
	char	*delimiter;

	if (current_in->filename == NULL)
	{
		perror("");
		return (NULL);
	}
	else 
	{
		delimiter = ft_strdup(current_in->filename);
		return (delimiter);
	}
	// else
	// 	delimiter = NULL;
}

int		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;
	pid_t	child;

	delimiter = get_delimiter(current_in);
	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
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
	if (fd < 0)
		perror(".heredoc");
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}
