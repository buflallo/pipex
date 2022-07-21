/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 04:30:42 by hlachkar          #+#    #+#             */
/*   Updated: 2022/06/23 23:45:29 by hlachkar         ###   ########.fr       */
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
	if (execve(path, ac, env) != 0)
	{
		wrong_cmd(command);
	}
}
