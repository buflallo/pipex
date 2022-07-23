/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 04:30:42 by hlachkar          #+#    #+#             */
/*   Updated: 2022/07/22 00:47:22 by hlachkar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (ft_strchr(cmd, '.') == 0 || ft_strchr(cmd, '/') == 0)
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

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
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
	if (ft_strcmp(path,ac[0]) == 0 && access(path, X_OK) != 0)
		wrong_cmd(ac[0]);
	if (execve(path, ac, env) != 0)
		wrong_cmd(command);
}

int output_file(char *filename, int mode)
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

int input_file(char *filename)
{
	int fdin;

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
		return input_file(filename);
	else
		return output_file(filename, mode);
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