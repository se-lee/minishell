#include "minishell.h"

int		redirect_heredoc(t_redirect *current_in);

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

/*
void		put_to_heredoc(t_redirect *current_in)
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
		while (line && ft_strcmp(line, delimiter))
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
}

void		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;
	pid_t	child;

	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
	delimiter = get_delimiter(current_in);
	line = readline("> ");
	while (line && ft_strcmp(line, delimiter))
	{
		ft_putendl_fd(line, fd);
		free(line);
		line = readline("> ");
	}
	close(fd);
}



void	exec_heredoc(t_shell *shell, t_pars *parse, int fd)
{
	char	*line;
	int		ret;
	int		i;

	i = 0;
	ret = 1;
	line = NULL;
	while (ret && g_heredoc)
	{
		ft_putstr_fd("> ", shell->cmd.fd_stdout);
		ret = ft_get_next_line(shell->cmd.fd_stdin, 2, &line);
		if (g_heredoc && ft_strcmp(line, parse->next->value) != 0)
			fill_file(line, fd);
		else
			break ;
		free(line);
		line = NULL;
	}
	if (line && g_heredoc)
	{
		free(line);
		line = NULL;
	}
}

f (child == 0)
	{
		delimiter = get_delimiter(current_in);
		line = readline("> ");
		while (line && ft_strcmp(line, delimiter))
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
}

*/
void		put_to_heredoc(t_redirect *current_in)
{
	int		fd;
	char	*delimiter;
	char	*line;
	int ret;

	fd = open(".heredoc", O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror("heredoc_file");
	delimiter = get_delimiter(current_in);
	line = NULL;
	ret = 1;
	while (line && ret)
	{
		ft_putstr_fd("> ", OUT);
		ret = ft_get_next_line(fd, &line);
	printf("line:%s\n", line);
		if (ft_strcmp(line, delimiter))
		{
			ft_putstr_fd("> ", OUT);
			ft_putstr_fd(line, fd);
			ft_putstr_fd("\n", fd);
		}
		else
		{
	printf("break\n");
			break ;
		}
		line = NULL;
		// free(line);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	close(fd);
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

int		heredoc_count(t_vars *vars)
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
