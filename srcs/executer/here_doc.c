/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 18:05:11 by ldurante          #+#    #+#             */
/*   Updated: 2022/01/18 15:43:42 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	check_hdoc(t_input *in)
{
	int	i;

	i = 0;
	in->is_hdoc = 0;
	while (in->split_in[i])
	{
		if (!(ft_strncmp(in->split_in[i], "<<", 3)) && in->q_state[i] == 0)
		{
			if (in->split_in[i + 1] != NULL)
				in->is_hdoc = 1;
			else
				error_msg(in, ERR_SYNTAX, -1, 0);
		}
		i++;
	}
}

void	remove_redir(t_input *in, int i)
{
	char	**aux;
	int		j;

	j = 0;
	aux = malloc(sizeof(char *) * (matrix_len(in->split_in) - 1));
	while (in->split_in[j] && j < i)
	{
		aux[j] = ft_strdup(in->split_in[j]);
		j++;
	}
	i += 2;
	while (in->split_in[i])
	{
		aux[j] = ft_strdup(in->split_in[i]);
		in->q_state[j] = in->q_state[i];
		j++;
		i++;
	}
	aux[j] = NULL;
	free_matrix(in->split_in);
	in->split_in = NULL;
	in->split_in = aux;
}

void	here_doc_aux(t_input *in, int fd)
{
	if (!in->split_in[0])
		return ;
	fd = open(".hd_tmp", O_RDONLY);
	if (!is_builtin(in))
		dup2(fd, STDIN_FILENO);
	close(fd);
}

void	here_doc(t_input *in, int i)
{
	char	*eof;
	int		fd;
	char	*here_doc;

	fd = open(".hd_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
		error_msg(in, ERR_FILE, -1, 0);
	eof = ft_strdup(in->split_in[i + 1]);
	free(in->prompt);
	in->prompt = ft_strdup("> ");
	while (1)
	{
		here_doc = readline(in->prompt);
		if (!here_doc || !(ft_strncmp(here_doc, eof, ft_strlen(eof))))
			break ;
		write(fd, here_doc, ft_strlen(here_doc));
		write(fd, "\n", 1);
		free(here_doc);
	}
	close(fd);
	free(here_doc);
	remove_redir(in, i);
	exec_hdoc(in);
	free(eof);
	here_doc_aux(in, fd);
}

void	exec_hdoc(t_input *in)
{
	int	i;

	i = 0;
	while (in->split_in[i])
	{
		if (!(ft_strncmp(in->split_in[i], "<<", 3)) && in->q_state[i] == 0)
		{
			if (in->split_in[i + 1] != NULL)
			{
				signal(SIGINT, handler4);
				signal(SIGQUIT, handler3);
				here_doc(in, i);
				i--;
			}
			else
				error_msg(in, ERR_SYNTAX, -1, 0);
		}
		i++;
	}
}
