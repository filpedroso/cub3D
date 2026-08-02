/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:01:05 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/27 18:47:27 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	return (ft_isdigit(c) || ft_isalpha(c));
}

/* #include <stdio.h>
int	main(void)
{
	for (int i = 0; i < 95; i++)
	{
		int	c = i + 32;
		ft_isalnum(c) ? printf("%c YES\n", c) : printf("%c NO\n", c);
	}
} */
