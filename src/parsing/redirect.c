#include "minishell.h"

void	initialize_inout(t_redirect *inout)
{
	inout->filename = NULL;
	inout->arrow_num = 0;
	inout->cmd_num = 0;
	inout->next = NULL;
}

void	prepare_inout(t_vars *vars, t_redirect **current_redirect)
{
	if (vars->inout == NULL)
	{
		vars->inout = protected_malloc(1, sizeof(t_redirect));
		(*current_redirect) = vars->inout;
		(*current_redirect)->next = NULL;
	}
	else
	{
		(*current_redirect)->next = protected_malloc(1, sizeof(t_redirect));
		(*current_redirect) = (*current_redirect)->next;
		(*current_redirect)->next = NULL;
	}
}

t_token	*fill_inout(t_vars *vars, t_token *current_token,
		t_redirect *current_inout, int cmd_num)
{
	initialize_inout(current_inout);
	current_inout->arrow_num = current_token->buffer.len;
	current_inout->cmd_num = cmd_num;
	current_inout->side = current_token->token_type;
	current_token = remove_token(vars, current_token);
	if (current_token == NULL)
		return (NULL);
	if (current_token->token_type == SPACE_SIGN)
		current_token = remove_token(vars, current_token);
	current_inout->filename = ft_strdup(current_token->buffer.str);
	current_token = remove_token(vars, current_token);
	return (current_token);
}

void	fill_redirect(t_vars *vars, int cmd_num)
{
	t_token		*current_token;
	t_redirect	*current_inout;

	current_token = vars->first;
	while (current_token)
	{
		if (current_token->token_type == PIPE_SIGN)
			cmd_num++;
		if (current_token->token_type == REDIRECT_LEFT
			|| current_token->token_type == REDIRECT_RIGHT)
		{
			prepare_inout(vars, &current_inout);
			current_token = fill_inout(vars, current_token,
					current_inout, cmd_num);
			printf("filename :%s", current_inout->filename);
		}
		else
			current_token = current_token->next;
	}
}
