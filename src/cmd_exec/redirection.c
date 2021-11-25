#include "minishell.h"

int	redirect_input(t_redirect *current_inout)
{
	int		fd;
	char	*file;

	file = current_inout->filename;
	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd("No such file or directory", 2);
		exit (EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

int	redirect_output(t_redirect *current_inout)
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
	if (dup2(fd, STDOUT_FILENO) == -1)
		exit(EXIT_FAILURE);
	close (fd);
	return (0);
}

int	redirect_heredoc(void)
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
				redirect_input(current_inout);
			else if (current_inout->arrow_num == 2)
				redirect_heredoc();
		}
		else if (current_inout->side == REDIRECT_RIGHT)
			redirect_output(current_inout);
		current_inout = current_inout->next;
	}
}
