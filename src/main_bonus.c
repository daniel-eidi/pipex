/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeidi-h <daeidi-h@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 00:50:46 by daeidi-h          #+#    #+#             */
/*   Updated: 2022/05/03 17:18:07 by daeidi-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> // waitpid
#include <pipex.h>

static void	reading_stdin(int fd, char *argv_limiter)
{
	char	*line;
	char	*limiter;

	limiter = ft_strjoin(argv_limiter, "\n");
	line = ft_get_next_line(STDIN_FILENO);
	while (line)
	{
		if (!ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			close(fd);
			free(limiter);
			free(line);
			exit(0);
		}
		write(fd, line, ft_strlen(line));
		free(line);
		line = ft_get_next_line(STDIN_FILENO);
	}
	close(fd);
	free(line);
	free(limiter);
}

static void	create_heredoc(char *argv_limiter)
{
	int		fd_2[2];
	pid_t	pid;
	int		status;

	if (pipe(fd_2) < 0)
		error("pipe");
	pid = fork();
	if (pid < 0)
		error("fork");
	if (pid > 0)
	{
		close(fd_2[WRITE_END]);
		waitpid(pid, &status, 0);
		dup2(fd_2[READ_END], STDIN_FILENO);
		close(fd_2[READ_END]);
	}
	else
	{
		close(fd_2[READ_END]);
		reading_stdin(fd_2[WRITE_END], argv_limiter);
	}
}

static void	exec_cmd(char *cmd)
{
	extern char	**environ;
	char		*cmd_path;
	char		**args;
	int			i;

	args = trat_arg(cmd);
	if (*args[0] == '/' || *args[0] == '.' || *args[0] == '~')
		cmd_path = args[0];
	else
	{
		i = 0;
		while (environ[i] && ft_strncmp(environ[i], "PATH=", 5))
			i++;
		if (!environ[i])
		{
			ft_putstr_fd("Error: path not found\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		cmd_path = get_path(args[0], environ[i] + 5);
	}
	execve(cmd_path, args, environ);
	error(cmd_path);
}

static void	pipe_exec(char **argv, int i)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		error("pipe");
	pid = fork();
	if (pid < 0)
		error("fork");
	if (pid > 0)
	{
		close(fd[WRITE_END]);
		waitpid(pid, &status, 0);
		dup2(fd[READ_END], STDIN_FILENO);
	}
	else
	{
		if (i == 2)
		{
			fd[READ_END] = open_ok(argv[1], O_RDONLY);
			dup2(fd[READ_END], STDIN_FILENO);
		}
		dup2(fd[WRITE_END], STDOUT_FILENO);
		exec_cmd(argv[i]);
	}
}

int	main(int argc, char *argv[])
{
	int	i;
	int	fd_1[2];

	i = 1;
	if ((argc >= 5 && ft_strncmp(argv[i], "here_doc", 9)) || argc >= 6)
	{
		fd_1[F_O] = open_ok(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND);
		if (!ft_strncmp(argv[i], "here_doc", 9))
			create_heredoc(argv[++i]);
		else
		{
			close(fd_1[F_O]);
			fd_1[F_O] = open_ok(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
		}
		while (++i < argc - 2)
			pipe_exec(argv, i);
		dup2(fd_1[F_O], STDOUT_FILENO);
		exec_cmd(argv[i]);
	}
	else
		error("Error: invalid arguments\n");
	return (0);
}
