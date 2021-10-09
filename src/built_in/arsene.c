void	exec(char **av, char **envp, int in, int out)
{
	int pid = fork;
	if (!pid)
	{
		if (in)
		{
			dup2(in, 0);
			close(in);
		}
		if (out > 1)
		{
			dup2(out, 1);
			close(out);
		}
		execve(av[0], &av[1], envp);
        write()
		exit(-1);
	}
}

int main(int ac, char **av, char **envp)
{
    int i = 0;
    int j = 0;
    int fd[2];
    int fdsave = 0;
    while (av[i])
    {
        if (av[i][0] == '|')
        {
            av[i] = NULL;
            pipe(fd);
            exec(av[j], envp, fdsave, fd[0]);
            fdsave = fd[1];
            i++;
            j = i;
        }
        else if (av[i][0] == ';')
        {
            av[i] = NULL;
            exec(av[j], envp);
            i++;
            j = i;
        }
        else
            i++;
    }
    if (i > 0 && av[i - 1] != NULL)
        exec(av[j], envp);
}