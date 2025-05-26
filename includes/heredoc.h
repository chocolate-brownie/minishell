/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgodawat <mgodawat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 13:43:40 by mgodawat          #+#    #+#             */
/*   Updated: 2025/05/26 22:38:31 by mgodawat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "error.h"
# include <readline/readline.h>
# include <stdio.h>
# include <unistd.h>

# define HEREDOC_FILE_PREFIX "/tmp/minishell_hd_"

/** @brief structure to track active heredoc files and update */
typedef struct s_hd_temp_file
{
	char						*path;
	struct s_hd_temp_file		*next;
}							t_hd_temp_file;

/** @brief struct of the main heredoc process
cat << *delimiter_str */
typedef struct s_heredoc_data
{
	char	*delimiter_str;
	char	*temp_filepath;
	int		fd;
}			t_heredoc_data;

#endif