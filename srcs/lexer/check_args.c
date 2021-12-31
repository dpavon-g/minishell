/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 19:23:22 by ldurante          #+#    #+#             */
/*   Updated: 2021/12/22 17:32:24 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_args(t_input *in)
{
	int		input_size;

	if (!in->user_input)
		return (0);
	ft_bzero(&in->flags, sizeof(in->flags));
	input_size = count_tokens(in->user_input, in, 0);
	in->split_input = malloc(sizeof(char *) * (input_size + 1));
	in->quote_state = malloc(sizeof(int) * input_size);
	if (!in->split_input)
		return (0);
	ft_bzero(&in->flags, sizeof(in->flags));
	count_tokens(in->user_input, in, 1);
	in->split_input[input_size] = NULL;
	expand_vars(in);
	return (input_size);
}
