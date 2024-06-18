/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brfernan <brfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 14:59:37 by brfernan          #+#    #+#             */
/*   Updated: 2024/06/18 09:58:15 by brfernan         ###   ########.fr       */
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
		return (freecoms(com), false);
	execve(path, com, envp);
	freecoms(com);
	return (false);
}

void	child1_process(int *fd, char **av, char **envp)
{
	int	filein;

	filein = open(av[1], O_RDONLY, 0644);
	if (filein == -1)
		close_fds_exit(fd, av[1]);
//	we close fd[read] because we dont need to use it (we arent reading from the pipe, we are reading from filein)
	close(fd[READ]);
//	we then substitute stdin by filein, so now filein is acting like stdin. 
	dup2(filein, STDIN_FILENO);
	close(filein);
//	we now substitute the normal output location, stdout, to our fd[write], so the pipe on the other side can read
	dup2(fd[WRITE], STDOUT_FILENO);
	close(fd[WRITE]);
	if (!av[2][0] || av[2][0] == ' ' || !execute(av[2], envp))
		error2(av[2], 1, fd);
}

void	child2_process(int *fd, char **av, char **envp)
{
	int	fileout;

	fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fileout == -1)
		close_fds_exit(fd, av[4]);
//	we close fd[WRITE] because we dont need to use it (we arent writing to pipe, we are writing to fileout)
	close(fd[WRITE]);
//	we now substitute stdin by fd[read], so that execve reads from fd[read] instead of from stdin
	dup2(fd[READ], STDIN_FILENO);
	close(fd[READ]);
//	we now substitute the normal output stdout to our own output file, fileout
	dup2(fileout, STDOUT_FILENO);
	close(fileout);
	if (!av[3][0])
		error2(av[3], 126, fd);
	if (av[3][0] == ' ' || !execute(av[3], envp))
		error2(av[3], 127, fd);
}

int	main(int ac, char **av, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	status = 0;
	if (ac != 5)
		return (ft_putendl_fd(WRONG, 2), 2);
// pipes: connect 2 file descriptors:
// fd[read] can receive info from fd[write] after fd[write] closes (done being written to)
	if (pipe(fd) == -1)
		error("pipe failed", 1);
	pid1 = fork();
	if (pid1 < 0)
		error("pid1 error", 0);
	else if (pid1 == 0)
		child1_process(fd, av, envp);
	pid2 = fork();
	if (pid2 < 0)
		error("pid2 error", 0);
	else if (pid2 == 0)
		child2_process(fd, av, envp);
//	for our pipe to work, we have to close our fd[WR] for fd[RD] to finally be accessible. since the pid1 will be
//	using fd[write] until it's done, pid2 cant run. pid2 will only start running after pid1 is complete, so we can put
//	all the waitpids at the end since pid2 will have to wait for pid1
	close(fd[WRITE]);
	close(fd[READ]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	return (WEXITSTATUS(status));
}
