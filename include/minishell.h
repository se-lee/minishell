#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/param.h>
# include <sys/wait.h>
# include <termios.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../libft/libft.h"

# define TRUE	1
# define FALSE	0	

# define IN		0
# define OUT	1

typedef struct s_vars		t_vars;
typedef struct s_redirect	t_redirect;
typedef struct s_token		t_token;
typedef struct s_string		t_string;
typedef struct s_envlist	t_envlist;
typedef struct s_redirect	t_redirect;
typedef struct s_command	t_command;
typedef enum e_type			t_type;
t_vars						*g_vars;

enum e_type
{
	WORD,
	QUOTE,
	SINGLE_QUOTE,
	PIPE_SIGN,
	REDIRECT_RIGHT,
	REDIRECT_LEFT,
	SPACE_SIGN
};

struct s_string {
	char	*str;
	int		len;
};

struct s_token {
	t_string	buffer;
	t_type		token_type;
	int			quote_nb;
	t_token		*next;
};

struct s_envlist {
	char		*name;
	char		*value;
	t_envlist	*next;
};

struct s_redirect {
	char		*filename;
	int			arrow_num;
	int			cmd_num;
	t_type		side;
	t_redirect	*next;
};

struct s_command {
	char		**command;
	int			pipe;
	int			quotes;
	int			cmd_index;
	int			redirect_right;
	int			redirect_left;
	int			fd[2];
	pid_t		pid;
	int			exit_status;
	t_command	*next;
};

struct s_vars {
	t_token			*first;
	t_envlist		*envp;
	t_command		*cmd;
	t_redirect		*inout;
	int				std_in;
	int				std_out;
	struct termios	saved_termios;
	long long int	return_value;
	int				error;
};

/* Parsing fonctions */
int			ft_isspecial(char c);
int			ft_isupper(char c);
int			find_space(char *str);
int			ft_piperedirect(int token_type);
void		add_char(t_token *token, char c);
void		update_token(t_token *token, char *var, char *value);
char		*replace(char *full, char *placeholder, char *real);
t_token		*replace_env(t_vars *vars, t_token *token);
void		tokenization(t_vars *vars, char *str);
void		parsing(t_vars *vars, char *str);
void		ft_comandadd_back(t_token **alst, t_token *new);
char		**array_realloc(char **array, char *new_line);
char		*remove_quotes(char *original, int token_type);
void		initialize_command(t_command *command);
void		add_piperedirect(t_token *current_token,
				t_command *current_command);
void		fill_redirect(t_vars *vars, int cmd_num);
void		fill_commands(t_vars *vars, t_token *token, int i);
t_token		*remove_token(t_vars *vars, t_token *token);
char		*find_variable(char *str);
void		malloc_token_next(t_token **current_token);
void		envlist_free(t_envlist *to_free);
void		envlist_print_all(t_envlist *envp);
int			check_error(t_token *token);
int			check_error2(t_vars *vars, t_token *token);

/* built-in utils */
int			format_is_valid(char *str);
int			ft_inenv(t_envlist *envp, char *str);
int			export_syntax(char *str, int quotes);
char		*search_home(t_envlist	*envp);
void		replace_main(t_vars *vars, t_envlist *current_envp,
				char *name, char *pwd);
void		replace_pwd(t_vars *vars, t_envlist *current_envp,
				char *name, char *pwd);
void		add_new_var_to_list(t_vars *vars, char *new_var);
void		free_env(t_envlist *current_env);
int			strisnum(char *str);
void		remove_space(char **str);
int			value_exceeds_llint(char *str, long long int return_value);
char		*find_old_pwd(t_vars *vars);
char		*find_pwd(t_vars *vars);
int		exit_simple(int return_value);

/* built-in commands */
int			builtin_cd(t_vars *vars, t_command *current_cmd);
int			builtin_echo(t_command *current_cmd);
int			builtin_env(t_vars *vars);
void		builtin_exit(t_vars *vars, t_command *current_cmd);
int			builtin_export(t_vars *vars, t_command *current_cmd);
int			builtin_unset(t_vars *vars, t_command *current_cmd, int i);
int			builtin_pwd(void);

/* envlist utils */
char		*get_env_value(t_envlist *envp, char *env_name, int return_value);
char		**envlist_to_char_array(t_envlist *envp);
int			envlist_count(t_envlist *envp);

/* execution */
void		print_commands(t_command *cmd); //erase this function
int			launch_commands(t_vars *vars, t_command *current_cmd,
				int fds[2], int to_close);
void		execute_command(t_vars *vars);
void		run_command(t_vars *vars, t_command *current_cmd);

/* execution utils */
void		run_command_builtin(t_vars *vars, t_command *current_cmd);
void		run_command_non_builtin(t_vars *vars, t_envlist *envlist,
				t_command *current_cmd);
int			envlist_count(t_envlist *envp);
char		**envlist_to_char_array(t_envlist *envp);
void		execute_pipe_commands(t_vars *vars);
void		launch_command(t_vars *vars, t_command *current_cmd,
				int input, int output);

/* loop_free */
void		free_token(t_token *token);
void		free_tokens(t_vars *vars);
void		free_commands(t_vars *vars);
void		free_inout(t_vars *vars);
void		loop_free(t_vars *vars);
void		run_command_no_pipe(t_vars *vars, t_command *current_cmd);
void		run_command_and_exit(t_vars *vars, t_command *current_cmd);
void		redirect_and_run_cmd(t_vars *vars, t_command *current_cmd,
				int builtin);
void		pipe_get_next_cmd(t_command *current_cmd);

/* pipe */
void		fd_dup_and_close(int input, int output);
void		fd_close(int input, int output);
void		pipe_and_launch_command(t_vars *vars, t_command *current_cmd,
				int input, int to_close);
void		wait_loop(t_vars *vars, pid_t child);

/* redirection */
int			redirect_input(t_redirect *current_inout);
int			redirect_output(t_redirect *current_inout);
void		put_to_heredoc(t_redirect *current_in);
void		redirection(t_vars *vars, t_command *current_cmd);
void		write_to_heredoc(t_redirect *current_in);
int			redirect_heredoc(void);
int			count_heredoc(t_vars *vars);
void		update_heredoc(t_vars *vars);
int			redirect_without_cmd(t_vars *vars);

/* command utils */
char		*get_command_path(t_envlist *envp, char *command);
int			count_command(t_command *cmd);
int			command_is_builtin(char **command);

/* envlist utils */
void		envlist_create(t_vars *vars, char **envp);
void		envlist_free(t_envlist *to_free);
char		*env_separation(char *str, int part);
void		envlist_print_all(t_envlist *envp);
t_envlist	*envlist_duplicate(t_envlist *envp);
t_envlist	*envlist_sort_ascii(t_vars *vars);

/* miscellaneous utils */
void		ft_append(char **a, char *str);
void		*protected_malloc(size_t count, size_t size);
void		free_array(char **array);
void		display_cmd_error(t_command *current_cmd, char *message, int arg);
void		display_syntax_error(t_vars *vars, t_token *current_token);

/* Signals */
void		sigchild(int sig);
void		sigmain(int sig);
void		set_termios(void);
void		control_c(int sig);

// void		run_command(t_vars *vars, t_command *current_cmd);

#endif
