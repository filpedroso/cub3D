/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 19:20:29 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/05 20:06:46 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/* void	odd_toupper(unsigned int i, char *c)
{
	if (ft_isalpha(*c) && i % 2 == 1)
		*c = *c - ' ';
}

int	main(void)
{
	char s[] = "jacarezinho de asa delta";
	ft_striteri(s, odd_toupper);
	printf("%s", s);
}*/