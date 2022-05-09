/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daeidi-h <daeidi-h@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 00:50:46 by daeidi-h          #+#    #+#             */
/*   Updated: 2022/05/09 19:11:25 by daeidi-h         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> // waitpid
#include <pipex.h>

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
		cmd_path = get_path(&args[0], environ[i] + 5);
	}
	execve(cmd_path, args, environ);
}

static void	pipe_exec(char **argv, int i)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		error("pipe", 0);
	pid = fork();
	if (pid < 0)
		error("fork", 0);
	if (pid > 0)
	{
		close(fd[WRITE_END]);
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
	if (argc == 5)
	{
		fd_1[F_O] = open_ok(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC);
		while (++i < argc - 2)
			pipe_exec(argv, i);
		dup2(fd_1[F_O], STDOUT_FILENO);
		exec_cmd(argv[i]);
	}
	else
		error("Error: invalid arguments\n", 0);
	return (0);
}
