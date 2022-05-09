/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeidi-h <daeidi-h@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 12:48:08 by daeidi-h          #+#    #+#             */
/*   Updated: 2022/05/09 19:10:21 by daeidi-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pipex.h>

void	error(const char *s, int i)
{
	perror(s);
	if (i)
		exit(i);
	exit(EXIT_SUCCESS);
}

int	open_ok(char *file, int flag)
{
	int	fd;

	fd = open(file, flag, 0644);
	if (fd == -1)
		error("Could not open file", 1);
	return (fd);
}

char	*get_path(char **cmd, const char *path)
{
	char	*file;
	char	*dir;
	int		diff;

	while (*path)
	{
		diff = ft_strchr(path, ':') - path;
		if (diff < 0)
			diff = ft_strlen(path);
		dir = ft_substr(path, 0, diff);
		file = ft_strjoin(dir, "/");
		file = ft_strjoin(file, cmd[0]);
		free(dir);
		if (access(file, X_OK) == 0)
			return (file);
		free(file);
		if (ft_strlen(path) < (size_t)diff)
			break ;
		path += diff;
		if (*path)
			path++;
	}
	free(cmd);
	error("command not found\n", 127);
	return (NULL);
}

void	restore_spaces(char **exec_args)
{
	char	*str;

	while (*exec_args)
	{
		str = *exec_args;
		while (*str)
		{
			if (*str == -1)
				*str = ' ';
			str++;
		}
		exec_args++;
	}
	return ;
}

char	**trat_arg(char *cmd)
{
	char	*arg;
	char	**split;
	char	inside;
	int		i;

	arg = ft_calloc(ft_strlen(cmd) + 1, 1);
	inside = -2;
	i = 0;
	while (*cmd)
	{
		if ((*cmd == '\'' || *cmd == '\"') && inside == -2)
			inside = *cmd ;
		else if (inside != -2 && *cmd == inside)
			inside = -2;
		else if (inside != -2 && *cmd == ' ')
			arg[i++] = -1;
		else
			arg[i++] = *cmd;
		cmd++;
	}
	split = ft_split(arg, ' ');
	free(arg);
	restore_spaces(split);
	return (split);
}
