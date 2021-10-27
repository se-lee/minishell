#include "minishell.h"

int	redirect_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	// close(fd);
	return (0);
}

int	redirect_output_overwrite(char *file)
{
	int	fd;

	fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror("dup2");
	// close(fd);
	return (0);
}

int	redirect_output_append(char *file)
{
	int	fd;
	fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDOUT_FILENO) == -1)
		perror ("dup2");
	// close (fd);
	return (0);
}

void	redirection(t_vars *vars)
{
	t_redirect	*current_in;
	t_redirect	*current_out;

	current_in = vars->in;
	current_out = vars->out;
	while (current_in)
	{
		if (current_in->arrow_num == 1)
			redirect_input(current_in->filename);
		else if (current_in->arrow_num == 2)
		{
			put_to_heredoc(current_in);
			redirect_heredoc(current_in);
			unlink(".heredoc");
		}
		current_in = current_in->next;
	}
	while (current_out)
	{
		if (current_out->arrow_num == 1)
			redirect_output_overwrite(current_out->filename);
		else if (current_out->arrow_num == 2)
			redirect_output_append(current_out->filename);
		current_out = current_out->next;
	}
}
