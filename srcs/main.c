/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 13:03:38 by ldurante          #+#    #+#             */
/*   Updated: 2021/12/25 17:04:12 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int exit_status;

void	leaks(void)
{
	system("leaks -q minishell");
}

void	init_structs(t_input *in, t_list **envp)
{
	in->env_list = envp;
	in->user_input = NULL;
	in->split_input = NULL;
	in->cmd_path = NULL;
	in->n_bytes = 0;
	in->fd_in = 0;
	in->fd_out = 0;
	exit_status = 0;
}

void	update_level(t_input *in)
{
	int		level;
	char	*number;
	char	*aux;	

	aux = ft_getenv("SHLVL", in);
	level = ft_atoi(aux);
	free(aux);
	level++;
	number = ft_itoa(level);
	aux = ft_strjoin("SHLVL=", number);
	in->split_input = malloc(sizeof(char *) * 3);
	in->split_input[0] = ft_strdup("export");
	in->split_input[1] = aux;
	in->split_input[2] = NULL;
	free(number);
	export(in);
	free_matrix(in->split_input);
}

void	handler(int	code)
{
	if (code == SIGINT)
	{
		exit_status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("",0);
		rl_redisplay();
	}
}

int	main(int argc, char **argv, char **environ)
{
	t_input	in;
	t_list	*envp;

	// atexit(leaks);
	envp = NULL;
	if (argc == 1)
	{
		//print_matrix(environ);
		init_env_list(&in, &envp, environ);
		init_structs(&in, &envp);
		update_level(&in);
		while (1)
		{
			signal(SIGINT, handler);
			signal(SIGQUIT, SIG_IGN);
			in.n_bytes = 0;
			// if (!isatty(STDIN_FILENO))
			// {
			// 	ioctl(STDIN_FILENO, FIONREAD, &in.n_bytes);
			// 	if (in.n_bytes < 1)
			// 		exit (0);
			// }
			read_input(&in);
		}
	}
	else
	{
		in.split_input = argv;
		error_msg(&in, ERR_ARG, -1);
	}
	return (0);
}
