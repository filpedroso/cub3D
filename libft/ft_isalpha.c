/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:02:41 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/27 16:20:49 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

/* #include <stdio.h>
int	main(void)
{
	for (int i = 0; i < 95; i++)
	{
		int	c = i + 32;
		ft_isalpha(c) ? printf("%c YES\n", c) : printf("%c NO\n", c);
	}
} */