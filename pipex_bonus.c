/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 08:07:00 by hlachkar          #+#    #+#             */
/*   Updated: 2022/06/27 00:56:21 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "get_next_line.h"

int	openfile(char *filename, int mode)
{
	int	fdout;

	if (mode == INFILE)
	{
		if (access(filename, F_OK))
		{
			write(STDERR, "pipex: ", 7);
			write(STDERR, filename, ft_strchr(filename, '\0'));
			write(STDERR, ": No such file or directory\n", 28);
			exit(127);
		}
		return (open(filename, O_RDONLY, 0644));
	}
	else
	{
		fdout = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fdout == -1)
		{
			perror("pipex");
			exit(127);
		}
		else
			return (fdout);
	}
}

void	redirection(char *command, char **env)
{
	int	fd[2];
	int	pid;

	if (pipe(fd) == -1)
		exit(2);
	pid = fork();
	if (pid)
	{
		close(fd[1]);
		dup2(fd[0], STDIN);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT);
		execute(command, env);
	}
}

void	final_execute(char *command, char **env)
{
	int	pid;

	pid = fork();
	if (!pid)
		execute(command, env);
}

void	first_execute(char *command, char **env, char *in)
{
	int	fd[2];
	int	pid;
	int	filein;

	if (pipe(fd) == -1)
		exit(2);
	pid = fork();
	if (pid)
	{
		close(fd[1]);
		dup2(fd[0], STDIN);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT);
		filein = openfile(in, INFILE);
		dup2(filein, STDIN);
		execute(command, env);
	}
}

int	main(int ac, char **av, char **env)
{
	int	filein;
	int	fileout;
	int	i;

	i = 2;
	fileout = openfile(av[ac - 1], OUTFILE);
	if (ft_strncmp(av[1], "here_doc", 8) == 0 && ac == 6)
	{
		dup2(fileout, STDOUT);
		here_doc(av[2]);
		redirection(av[3], env);
		execute(av[4], env);
	}
	else if (ac >= 5)
	{
		filein = openfile(av[1], INFILE);
		dup2(filein, STDIN);
		dup2(fileout, STDOUT);
		while (i < ac - 2)
			redirection(av[i++], env);
		execute(av[ac - 2], env);
	}
	else
		write(STDERR, "Invalid number of arguments.\n", 29);
	return (1);
}
