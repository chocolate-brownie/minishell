/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 23:18:01 by mgodawat          #+#    #+#             */
/*   Updated: 2025/04/19 01:03:38 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

typedef struct s_exec	t_exec;
typedef struct s_args	t_args;
typedef struct s_redirs	t_redirs;

typedef enum s_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

typedef struct s_redirs
{
	char				*path;
	t_redir_type		type;
	struct s_redirs		*next;
}						t_redirs;

typedef struct s_args
{
	char				*value;
	struct s_args		*next;
}						t_args;

/** s_exec struct in controls of the commands and its args

ex: echo hello
	[*cmd] [*args]

# NOTE: in this example there are two parts connected by the pipe so we will
represent this as a linked list of two t_command structures

ex: cat < input.txt | grep "hello" > output.txt

[t_command Node 1]  | [t_command Node 2]

Node 1: cat < input.txt

	t_exec structure:
		cmd: "cat"
		args: NULL (or pointer to an empty list)
		redirs: Pointer to a t_redirs list -> [ Redir Node A ] -> NULL
		next: Pointer to t_exec Node 2

	t_redirs List for Node 1 (Redirections):
		Redir Node A:
			path: "input.txt"
			type: REDIR_INPUT (Corresponds to <)
			next: NULL

Node 2: grep "hello" > output.txt

	t_exec structure:
		cmd: "grep"
		args: Pointer to a t_args list -> [ Arg Node X ] -> NULL
		redirs: Pointer to t_redirs list -> [ Redir Node B ] -> NULL
		next: NULL (This is the end of the pipeline)

	t_args List for Node 2 (Arguments):
		Arg Node X:
			value: "hello"
			next: NULL
	t_redirs List for Node 2 (Redirections):
		Redir Node B:
			path: "output.txt"
			type: OUTFILE (Corresponds to >)
			next: NULL */

typedef struct s_exec
{
	char				*cmd;
	t_args				*args;
	t_redirs			*redirs;
	struct s_exec		*next;

}						t_exec;

#endif