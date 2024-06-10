/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno <bruno@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:59:37 by brfernan          #+#    #+#             */
/*   Updated: 2024/05/31 19:42:48 by bruno            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

bool	execute(char *arg, char **envp)
{
	char	**com;
	char	*path;

	com = ft_split(arg, ' ');
	path = find_path(envp, com[0]);
	if (!path)
	{
		freecoms(com);
		return (false);
	}
	if (execve(path, com, envp) == -1)
		error("execution failed", 1);
	return (true);
}

void	child1_process(int *fd, char **av, char **envp)
{
	int	filein;

	filein = open(av[1], O_RDONLY, 0644);
	if (filein == -1)
		error(av[1], 0);
	close(fd[0]);
	dup2(fd[1], 1);// makes it so that fd[1] is the output that was going to 1
	dup2(filein, 0);// dups the filein into the input that will be executed
	close(fd[1]);
	close(filein);
	if (!execute(av[2], envp))
		error2(av[2], 1);
	exit(EXIT_SUCCESS);
}

void	child2_process(int *fd, char **av, char **envp)
{
	int	fileout;

	fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);//perms: writes, and then it either creates or truncates (delete what exists) the file
	if (fileout == -1)
		error(av[3], 0);
	close(fd[1]);
	dup2(fd[0], 0);// dups the fd[0] into the input that will be executed
	dup2(fileout, 1);// makes it so that fileout is the output that was going to 1
	close(fd[0]);
	close(fileout);
	if (!execute(av[3], envp))
		error2(av[3], 127);
	exit(EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	status = 0;
	if (ac != 5)
		return (ft_putendl_fd(WRONG, 2), 1);
	if (pipe(fd) == -1)//makes it so that fd[0] and fd[1] are connected (piped)
		error("pipe failed", 1);
	pid1 = fork();//from here on, program is forked aka runs twice
	if (pid1 < 0)
		error("pid1 error", 0);
	else if (pid1 == 0)
		child1_process(fd, av, envp);
	pid2 = fork();//forked again so main process isnt lost
	if (pid2 < 0)
		error("pid2 error", 0);
	else if (pid2 == 0)
		child2_process(fd, av, envp);
	close(fd[0]);//write
	close(fd[1]);//read
	waitpid(pid1, NULL, 0);//waits for process 1 to complete with no return
	waitpid(pid2, &status, 0);//waits for process 1 to complete with the return of status
	return (WEXITSTATUS(status));
}
