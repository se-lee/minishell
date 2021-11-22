#include "minishell.h"

int	redirect_input(t_command *current_cmd, t_redirect *current_inout)
{
	int		fd;
	char	*file;

	file = current_inout->filename;
	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_putendl_fd("No such file or directory", 2);
		// perror(file);
printf("[in] current_cmd:%s    file:%s    in_fd:%d    in_currnt_fd[1]:%d    in_cmd_nbr:%d\n", current_cmd->command[0], current_inout->filename, fd, current_cmd->fd[1], current_inout->cmd_num);
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	redirect_out(t_command *current_cmd, t_redirect *current_inout)
{
	int		fd;
	char	*file;

	file = current_inout->filename;
	fd = -1;
	if (current_inout->arrow_num == 1)
		fd = open(file, O_TRUNC | O_CREAT | O_WRONLY, 0644);
	else if (current_inout->arrow_num == 2)
		fd = open(file, O_APPEND | O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		perror(file);
printf("[out] current_cmd:%s    file:%s    out_fd:%d    out_currnt_fd[1]:%d    out_cmd_nbr:%d\n", current_cmd->command[0], current_inout->filename, fd, current_cmd->fd[1], current_inout->cmd_num);
	if (current_cmd->pipe)
	{
		if (dup2(fd, current_cmd->fd[0]) == -1)
			exit(EXIT_FAILURE);
	}
	else
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
	}
	close (fd);
	return (0);
}

void	redirection(t_vars *vars, t_command *current_cmd)
{
	t_redirect	*current_inout;

	current_inout = vars->inout;
	while (current_inout && current_inout->cmd_num != current_cmd->cmd_index)
		current_inout = current_inout->next;
	while (current_inout && current_inout->cmd_num == current_cmd->cmd_index)
	{
		if (current_inout->side == REDIRECT_LEFT)
		{
			if (current_inout->arrow_num == 1)
				redirect_input(current_cmd, current_inout);
			else if (current_inout->arrow_num == 2)
				redirect_heredoc();
		}
		else if (current_inout->side == REDIRECT_RIGHT)
			redirect_out(current_cmd, current_inout);
		current_inout = current_inout->next;
	}
}
