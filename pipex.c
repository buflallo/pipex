/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 02:47:47 by hlachkar          #+#    #+#             */
/*   Updated: 2022/07/22 00:56:23 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		fdout = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fdout == -1)
		{
			perror("pipex");
			exit(127);
		}
		else
			return (fdout);
	}
}

void	redirection(char *command, char **env, char *in)
{
	int	fd[2];
	int	pid;
	int	filein;

	if (pipe(fd) == -1)
		exit(2);
	pid = fork();
	if (pid)
	{
		waitpid(pid, NULL, 0);
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

void	final_execute(char *command, char **env)
{
	int	pid;
	int *status;

	status = NULL;
	pid = fork();
	if (!pid)
	{
		execute(command, env);
	}
}

int	main(int ac, char **av, char **env)
{
	int	fileout;
	int	status;

	status = 0;
	if (ac == 5)
	{
		fileout = openfile(av[ac - 1], OUTFILE);
		dup2(fileout, STDOUT);
		redirection(av[2], env, av[1]);
		final_execute(av[3], env);
		while ((wait(&status)) > 0)
			;
		close(fileout);
		exit(0);
	}
	else
		write(STDERR, "Invalid number of arguments.\n", 29);
	return (1);
}
