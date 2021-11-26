/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_no_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acusanno <acusanno@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 07:42:28 by acusanno          #+#    #+#             */
/*   Updated: 2021/11/26 07:42:30 by acusanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_in_no_cmd(t_redirect *current_inout)
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
		return (EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		perror("dup2");
	close(fd);
	return (0);
}

void	redir_out_no_cmd(t_redirect *current_inout)
{
	int	std_in;
	int	std_out;

	std_in = dup(STDIN_FILENO);
	std_out = dup(STDOUT_FILENO);
	redirect_output(current_inout);
	dup2(std_in, 0);
	dup2(std_out, 1);
	close(std_in);
	close(std_out);
}

int	redirect_without_cmd(t_vars *vars)
{
	t_redirect	*current_inout;

	current_inout = vars->inout;
	while (current_inout)
	{
		if (current_inout->side == REDIRECT_LEFT)
		{
			if (current_inout->arrow_num == 1)
				return (redir_in_no_cmd(current_inout));
		}
		else if (current_inout->side == REDIRECT_RIGHT)
			redir_out_no_cmd(current_inout);
		current_inout = current_inout->next;
	}
	if (count_heredoc(vars) > 0)
		unlink(".heredoc");
	return (0);
}
