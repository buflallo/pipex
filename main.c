#include "pipex.h"

// O_CREAT | O_WRONLY | O_TRUNC,
// 				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH


int openfile(char* filename, int MODE){
    
    if(MODE == 0)
    {
        if(access(filename, F_OK) == 0)
            open(filename, O_RDONLY);
        else
        {
            write(STDERR, "No Such File Or Directory \n",28);
            exit(2);
        }
    }
    else
    {
        open(filename, O_CREAT | O_WRONLY | O_TRUNC,
 				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    }
}



char	*get_path (char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;
    char *test;

	i = 0;
	while (env[i] && str_ncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && str_ichr(path, ':') > -1)
	{
		dir = str_ndup(path, str_ichr(path, ':'));
		bin = path_join(dir, cmd);
        //printf("======%s\n",test);
		free(dir);
		if (access(bin, F_OK) == 0)
        {
            return (bin);
        }
		free(bin);
		path += str_ichr(path, ':') + 1;
	}
	return (cmd);
}



void execute(char* command, char **env)
{
    char** ac;
    char* path;

    ac = ft_split(command, ' ');
    // ac = "ls" "-la";
    // command = "ls -la"
    path = get_path(ac[0], env);
    // printf("%s",path);
    execve(path, ac, env);
    write(STDERR, "pipex: ", 7);
	write(STDERR, command, str_ichr(command, 0));
	write(STDERR, ": command not found\n", 20);
	exit(127);
}


void rediraction(char *command, char **env, int fdin){
    int fd[2];
    int pid;

    if(pipe(fd) == -1)
        exit(2);
    pid = fork();
    if (pid)
    {
        close(fd[1]);
        dup2(fd[0], STDIN);
        waitpid(pid, NULL, 0);
    }
    else
    {
        close(fd[0]);
        dup2(fd[1], STDOUT);
        if (fdin == STDIN)
            exit(2);
        execute(command, env);
    }

}

int	main (int ac, char **av, char **env)
{
	int	fdin;
	int	fdout;
    int i;
    i = 3;

	if (ac > 5)
	{
        fdin = openfile(av[1], INFILE);
        fdout = openfile(av[ac - 1], OUTFILE);
        dup2(fdin, STDIN);
        dup2(fdout, STDOUT);
        rediraction(av[2], env, fdin);
        while (i < ac - 2)
            rediraction(av[i++], env, fdin);
        execute(av[i++], env);
	}
	else
		write(STDERR, "Invalid number of arguments.\n", 29);
	return (1);
}