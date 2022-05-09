/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeidi-h <daeidi-h@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 12:53:38 by daeidi-h          #+#    #+#             */
/*   Updated: 2022/05/09 19:29:27 by daeidi-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/* file i/o */
# define F_I 0
# define F_O 1

/* pipe ends */
# define READ_END 0
# define WRITE_END 1

/* libraries */
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <libft.h>

void	error(const char *s, int i);
char	*get_path(char **cmd, const char *path);
int		open_ok(char *file, int flag);
void	restore_spaces(char **exec_args);
char	**trat_arg(char *cmd);
char	*ft_get_next_line_lim(int fd, char *limiter);
void	free_split(char **str);
void	free_ptr(char **str);
void	create_heredoc(char *argv_limiter);

#endif
