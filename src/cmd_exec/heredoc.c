#include "minishell.h"

int		count_heredoc(t_vars *vars)
{
	t_redirect	*current_in;
	int			heredoc_count;

	current_in = vars->in;
	heredoc_count = 0;
	while (current_in)
	{
		if (current_in->arrow_num == 2)
			heredoc_count++;
		current_in = current_in->next;
	}
	return (heredoc_count);
}

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

void		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;

	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror("heredoc_file");
	delimiter = get_delimiter(current_in);
	line = NULL;
	ft_putstr_fd("> ", OUT);
	while (get_next_line(IN, &line))
	{
		if (ft_strcmp(line, delimiter))
		{
			ft_putstr_fd("> ", OUT);
			ft_putstr_fd(line, fd);
			ft_putstr_fd("\n", fd);
			free(line);
		}
		else
			break ;
	}
	close(fd);
}

void	multiple_heredoc(t_vars *vars)
{
	t_redirect	*current_in;

	current_in = vars->in;
	while (current_in)
	{
		if (current_in->arrow_num == 2)
			put_to_heredoc(current_in);
		current_in = current_in->next;
	}
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
