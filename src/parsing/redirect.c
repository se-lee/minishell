#include "minishell.h"

void	prepare_inout(t_vars *vars, t_redirect **current_redirect, int inout)
{
	if (inout == IN && vars->in == NULL)
	{
		printf("in1\n");
		vars->in = protected_malloc(1, sizeof(t_redirect));
		(*current_redirect) = vars->in;
		(*current_redirect)->next = NULL;
	}
	else if (inout == OUT && vars->out == NULL)
	{
		printf("in2\n");
		vars->out = protected_malloc(1, sizeof(t_redirect));
		(*current_redirect) = vars->out;
		(*current_redirect)->next = NULL;
	}
	else
	{
		printf("in3\n");
		(*current_redirect)->next = protected_malloc(1, sizeof(t_redirect));
		(*current_redirect) = (*current_redirect)->next;
		(*current_redirect)->next = NULL;
	}
}

t_token	*fill_inout(t_vars *vars, t_token *current_token,
		t_redirect *current_inout, int cmd_num)
{
	current_inout->arrow_num = current_token->buffer.len;
	current_inout->cmd_num = cmd_num;
	current_token = remove_token(vars, current_token);
	if (current_token->token_type == SPACE_SIGN)
		current_token = remove_token(vars, current_token);
	current_inout->filename = ft_strdup(current_token->buffer.str);
	current_token = remove_token(vars, current_token);
	return (current_token);
}

void	fill_redirect(t_vars *vars)
{
	t_token		*current_token;
	t_redirect	*current_in;
	t_redirect	*current_out;
	int			cmd_num;

	current_token = vars->first;
	cmd_num = 0;
	while (current_token)
	{
		if (current_token->token_type == PIPE_SIGN)
			cmd_num++;
		if (current_token->token_type == REDIRECT_LEFT)
		{
			printf("1\n");
			prepare_inout(vars, &current_in, IN);
			printf("2\n");
			current_token = fill_inout(vars, current_token,
					current_in, cmd_num);
		}
		else if (current_token->token_type == REDIRECT_RIGHT)
		{
			printf("3\n");
			prepare_inout(vars, &current_out, OUT);
			printf("4\n");
			current_token = fill_inout(vars, current_token, current_out, cmd_num);
		}
		else
			current_token = current_token->next;
	}
}
