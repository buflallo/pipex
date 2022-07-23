/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlachkar <hlachkar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 04:30:42 by hlachkar          #+#    #+#             */
/*   Updated: 2022/07/23 23:12:20 by hlachkar         ###   ########.fr       */
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
	if (ft_strcmp(path, ac[0]) == 0 && access(path, X_OK) != 0)
		wrong_cmd(ac[0]);
	if (execve(path, ac, env) != 0)
		wrong_cmd(command);
}
