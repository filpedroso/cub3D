/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:41:19 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/27 16:20:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/* #include <stdio.h>
int	main(void)
{
	for (int i = 0; i < 50; i++)
	{
		int	c = i + 32;
		ft_isdigit(c) ? printf("%c YES\n", c) : printf("%c NO\n", c);
	}
} */
