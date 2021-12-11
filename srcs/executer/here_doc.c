/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 18:05:11 by ldurante          #+#    #+#             */
/*   Updated: 2021/12/11 03:03:28 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**remove_heredoc(t_input *in, int i, char **full_doc)
{
	int j;

	j = i;
	i += 3;
	if (in->split_input[i] == NULL && in->split_input[0][0] == '<')
		in->split_input[0] = ft_strdup("");
	else
	{
		while(in->split_input[i])
		{
			in->split_input[j] = ft_strdup(in->split_input[i]);
			j++;
			i++;
		}
		in->split_input[j] = NULL;
	}
	if (!(ft_strncmp(in->split_input[0], "cat", 4))
		&& matrix_len(in->split_input) == 1)
	{
		print_matrix(full_doc);
		in->split_input[0] = ft_strdup("");
	}
	free_matrix(full_doc);
	return(in->split_input);
}

void	here_doc(t_input *in, int i)
{
	char	*here_doc;
	char	*delimiter;
	char	**full_doc;

	full_doc = malloc(sizeof(char *));
	full_doc[0] = NULL;
	delimiter = ft_strdup(in->split_input[i + 2]);
	in->prompt = ft_strdup("> ");
	while (1)
	{
		here_doc = readline(in->prompt);
		if (!(ft_strncmp(here_doc, delimiter, ft_strlen(delimiter))))
			break ;
		full_doc = matrix_add_back(full_doc, here_doc);
	}
	free(here_doc);
	remove_heredoc(in, i, full_doc);
}

