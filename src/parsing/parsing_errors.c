#include "minishell.h"

int	check_error(t_token *token)
{
	if (token->token_type == PIPE_SIGN && token->buffer.len > 1)
		return (-1);
	if ((token->token_type == QUOTE || token->token_type == SINGLE_QUOTE)
		&& token->quote_nb != 2)
		return (-1);
	if (ft_piperedirect(token->token_type) == 1 && token->buffer.len > 2)
		return (-1);
	if ((token->token_type == REDIRECT_LEFT
			|| token->token_type == REDIRECT_RIGHT)
		&& (token->next == NULL || (token->next->token_type == SPACE_SIGN
				&& token->next->next == NULL)
			|| (token->next->token_type == PIPE_SIGN)
			|| (token->next->token_type == SPACE_SIGN
				&& token->next->next != NULL
				&& token->next->next->token_type == PIPE_SIGN)))
		return (-1);
	return (0);
}

int	check_error2(t_vars *vars, t_token *token)
{
	t_token	*current_token;

	current_token = token;
	if (token->token_type == PIPE_SIGN)
	{
		current_token = current_token->next;
		if (current_token && current_token->token_type == SPACE_SIGN)
			current_token = current_token->next;
		if (current_token && current_token->token_type == PIPE_SIGN)
			return (-1);
	}
	if (token == vars->first && token->token_type == PIPE_SIGN
		&& token->next == NULL)
	{
		vars->return_value = 258;
		return (-1);
	}
	if (token->token_type == PIPE_SIGN && (!token->next
			|| (token->next && token->next->token_type == SPACE_SIGN
				&& !token->next->next)))
		return (-1);
	return (0);
}
