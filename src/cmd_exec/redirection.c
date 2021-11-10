#include "minishell.h"

// int	ft_redirect_in(t_cmd *cmd, t_redir redir)
// {
// 	int	fd;

// 	fd = -1;
// 	fd = open(redir.value, O_RDONLY, S_IRWXU);
// 	errno = 0;
// 	if (fd == -1)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(redir.value, 2);
// 		ft_putstr_fd(": No such file or directory\n", 2);
// 		return (0);
// 	}
// 	close(cmd->fd_in);
// 	if (dup2(fd, cmd->fd_in) == -1)
// 		print_error(errno);
// 	close(fd);
// 	return (1);
// }

// int	ft_redirect(t_cmd *cmd, t_redir redir)
// {
// 	int	fd;

// 	fd = -1;
// 	if (redir.type == 3)
// 		fd = open(redir.value, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
// 	else if (redir.type == 2)
// 		fd = open(redir.value, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
// 	errno = 0;
// 	if (fd == -1)
// 	{
// 		print_error(errno);
// 		return (0);
// 	}
// 	close(cmd->fd_out);
// 	if (dup2(fd, cmd->fd_out) == -1)
// 		print_error(errno);
// 	close(fd);
// 	return (1);
// }

int	redirect_input(t_command *current_cmd, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror(file);
		exit(EXIT_FAILURE);
	}
	close(current_cmd->fd[0]);
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
	// close(current_cmd->fd[1]);
	if (dup2(fd, STDOUT_FILENO) == -1)//current_cmd->fd[1]) < 0) //STDOUT_FILENO == -1))
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
	while (current_in)
	{
		if (current_in->arrow_num == 1)
			redirect_input(current_cmd, current_in->filename);
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

/*
add this to main
	if (count_heredoc(vars) > 0)
		unlink(".heredoc");
*/
