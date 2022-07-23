/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 04:30:42 by hlachkar          #+#    #+#             */
/*   Updated: 2022/07/23 23:12:15 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
}

int	output_file(char *filename, int mode)
{
	int	fdout;

	if (!access(filename, F_OK) && access(filename, W_OK))
		wrong_cmd(filename);
	if (mode == O_APPEND)
		fdout = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fdout = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fdout == -1)
	{
		perror("pipex");
		exit(127);
	}
	else
		return (fdout);
}

int	input_file(char *filename)
{
	int	fdin;

	if (access(filename, F_OK) || access(filename, R_OK))
		wrong_cmd(filename);
	fdin = open(filename, O_RDONLY, 0644);
	if (fdin == -1)
	{
		perror("pipex");
		exit(127);
	}
	else
		return (fdin);
}

int	openfile(char *filename, int mode)
{
	if (mode == INFILE)
		return (input_file(filename));
	else
		return (output_file(filename, mode));
}

void	redirection_pipe_line(char *command, char **env)
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
