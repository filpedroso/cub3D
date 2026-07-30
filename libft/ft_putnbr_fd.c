/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:20:34 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/05 20:43:24 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	deli_rec(long num, int fd);

void	ft_putnbr_fd(int n, int fd)
{
	long	num;

	num = n;
	if (num < 0)
	{
		num = -num;
		ft_putchar_fd('-', fd);
	}
	deli_rec(num, fd);
}

void	deli_rec(long num, int fd)
{
	if (num >= 10)
		deli_rec(num / 10, fd);
	ft_putchar_fd(num % 10 + '0', fd);
}

/* int main()
{
	ft_putnbr_fd(-123456, 1);
} */
