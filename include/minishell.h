#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include "../libft/libft.h"

# define TRUE	1
# define FALSE	0

typedef struct s_vars		t_vars;
typedef struct s_command	t_command;
typedef struct s_token		t_token;
typedef struct s_string		t_string;
typedef enum e_type			t_type;

enum e_type
{
	WORD,
	QUOTE,
	SINGLE_QUOTE,
	SPACE,
	PIPE_SIGN,
	REDIRECT
};

struct s_string {
	char	*str;
	int		len;
};

struct s_token {
	t_string	*buffer;
	t_type		token_type;
};

struct s_command {
	t_token		token;
	t_command	*next;
};

struct s_vars {
	t_command	*first;
};

//Parsing fonctions
int		isspecial(char c);
void	parsing(t_vars *vars, char *str);

#endif
