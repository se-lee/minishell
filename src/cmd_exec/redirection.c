#include "minishell.h"

int	redirect_input(t_command *current_cmd, t_redirect *current_in)
{
	int		fd;
	char	*file;

	file = current_in->filename;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror(file);
printf("[in] current_cmd:%s    file:%s    in_fd:%d    in_currnt_fd[1]:%d    in_cmd_nbr:%d\n", current_cmd->command[0], current_in->filename, fd, current_cmd->fd[1], current_in->cmd_num);
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
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
printf("[out] current_cmd:%s    file:%s    out_fd:%d    out_currnt_fd[1]:%d    out_cmd_nbr:%d\n", current_cmd->command[0], current_out->filename, fd, current_cmd->fd[1], current_out->cmd_num);
	// if (current_cmd->pipe)
	// {
	// 	if (dup2(fd, current_cmd->fd[0]) == -1)
	// 		exit(EXIT_FAILURE);
	// }
	// else
	// {
		if (dup2(fd, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
	// }
	close (fd);
	return (0);
}

void	redirection(t_vars *vars, t_command *current_cmd)
{
	t_redirect	*current_in;
	t_redirect	*current_out;
	int			i;

	current_in = vars->in;
	current_out = vars->out;
	i = 0;
	while (current_in)
	{
		if (current_in->arrow_num == 1)
			redirect_input(current_cmd, current_in);
		else if (current_in->arrow_num == 2)
			redirect_heredoc();
		current_in = current_in->next;
	}
	while (current_out)
	{
		redirect_out(current_cmd, current_out);
		i++;
		current_out = current_out->next;
	}
}
