/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldurante <ldurante@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:58:27 by ldurante          #+#    #+#             */
/*   Updated: 2021/12/23 19:36:28 by ldurante         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_msg(t_input *in, char *MSG, int n)
{
	if (n >= 0)
	{
		write(2, SHELL, ft_strlen(SHELL));
		write(2, in->split_input[n], ft_strlen(in->split_input[n]));
		write(2, ": ", 2);
		write(2, MSG, ft_strlen(MSG));
		write(2, "\n", 1);
	}
	else
	{
		write(2, SHELL, ft_strlen(SHELL));
		write(2, MSG, ft_strlen(MSG));
		write(2, "\n", 1);
	}
	return (0);
}
