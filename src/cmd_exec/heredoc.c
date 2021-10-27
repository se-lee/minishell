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
}

int		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;
	pid_t	child;

	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
			perror("heredoc_file");
	child = fork();
	if (child == 0)
	{
		delimiter = get_delimiter(current_in);
		line = readline("> ");
		while (ft_strcmp(line, delimiter))
		{
			ft_putendl_fd(line, fd);
			free(line);
			line = readline("> ");
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("dup2");
		// close(fd);
		exit(0);
	}
	else
		waitpid(child, NULL, 0);
	return (0);
}

/*
int		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;
	int ret;

		fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
		line = ft_strdup("");
		ret = 1;
		if (fd < 0)
			perror("heredoc_file");
		delimiter = get_delimiter(current_in);
		while (get_next_line(fd, &line) > 0)
		{
		printf("line:%s\n", line);
			ft_putstr_fd("> ", 1);
			ret = get_next_line(fd, &line);
			if (ft_strcmp(line, delimiter) != 0)
			{
				ft_putstr_fd("> ", 1);
				ft_putendl_fd(line, fd);
			}
			else
			{
				printf("break\n");
				break ;
			}
			free(line);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
			perror("dup2");
		close(fd);
		return (0);
}
*/

int		redirect_heredoc(t_redirect *current_in)
{
	int		fd;

	fd = open(".heredoc", O_RDONLY);
	if (fd < 0)
		perror(".heredoc");
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	// close(fd);
	return (0);
}
