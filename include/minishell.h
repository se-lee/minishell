#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include "../libft/libft.h"

# define TRUE	1
# define FALSE	0

typedef struct s_vars		t_vars;
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
	t_string	buffer;
	t_type		token_type;
	t_token		*next;
};

struct s_vars {
	t_token	*first;
	char	**envp;
};

//Parsing fonctions
int		isspecial(char c);
int		ft_isupper(char c);
char	*replace(char *full, char *placeholder, char *real);
void	parsing(t_vars *vars, char *str);
void	ft_comandadd_back(t_token **alst, t_token *new);

#endif
