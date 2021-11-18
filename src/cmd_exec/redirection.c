#include "minishell.h"

int	redirect_input(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror(file);
	if (dup2(fd, STDIN_FILENO) == -1)
		exit(EXIT_FAILURE);
	close(fd);
	return (0);
}

int	redirect_out(t_command *current_cmd, t_redirect *current_out)
{
	int		fd;
	char	*file;

	file = current_out->filename;
	fd = -1;
	if (current_out->arrow_num == 1)
		fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	else if (current_out->arrow_num == 2)
		fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror(file);
	// if (dup2(fd, current_cmd->next->fd[1]) < 0)
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close (fd);
	return (0);
}

void	redirection(t_vars *vars, t_command *current_cmd)
{
	t_redirect	*current_in;
	t_redirect	*current_out;

	current_in = vars->in;
	current_out = vars->out;
	(void)current_cmd;
	while (current_in)
	{
		if (current_in->arrow_num == 1)
			redirect_input(current_in->filename);
		else if (current_in->arrow_num == 2)
			redirect_heredoc();
		current_in = current_in->next;
	}
	while (current_out)
	{
		redirect_out(current_cmd, current_out);
		current_out = current_out->next;
	}
}
