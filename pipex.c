/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 02:47:47 by hlachkar          #+#    #+#             */
/*   Updated: 2022/06/27 00:00:40 by hlachkar         ###   ########.fr       */
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

char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && ft_strchr(path, ':') > -1)
	{
		dir = ft_strndup(path, ft_strchr(path, ':'));
		bin = str_join(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_strchr(path, ':') + 1;
	}
	return (cmd);
}

void	execute(char *command, char **env)
{
	char	**ac;
	char	*path;

	if (command[0] != '\0')
		ac = ft_split(command, ' ');
	else
	{
		wrong_cmd(command);
		exit(127);
	}
	path = get_path(ac[0], env);
	if (execve(path, ac, env) != 0)
	{
		wrong_cmd(command);
	}
}

void	redirection(char *command, char **env, char *in)
{
	int	fd[2];
	int	pid;
	int filein;

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

void final_execute(char *command, char **env)
{
	int	pid;

	pid = fork();
	if (!pid)
		execute(command, env);
}

int	main(int ac, char **av, char **env)
{
	int	fileout;
	int status = 0;

	if (ac == 5)
	{
		
		fileout = openfile(av[ac - 1], OUTFILE);
		dup2(fileout, STDOUT);
		redirection(av[2], env, av[1]);
		final_execute(av[3], env);
		while ((wait(&status)) > 0);
		close(fileout);
		exit(0);
	}
	else
		write(STDERR, "Invalid number of arguments.\n", 29);
	return (1);
}
