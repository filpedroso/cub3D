/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 19:57:18 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/06 20:54:57 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_digits(long num);
static void			itoa_help(char *ascii, long num, unsigned int count);

char	*ft_itoa(int n)
{
	unsigned int	count;
	char			*ascii;
	long			num;
	int				minus_flag;

	num = (long)n;
	minus_flag = (num < 0);
	if (minus_flag)
		num = -num;
	count = count_digits(num) + minus_flag;
	ascii = ft_calloc(1, (count + 1));
	if (!ascii)
		return (NULL);
	if (minus_flag)
		ascii[0] = '-';
	itoa_help(ascii + minus_flag, num, count - minus_flag);
	return (ascii);
}

static unsigned int	count_digits(long num)
{
	unsigned int	count;

	count = 1;
	while (num >= 10)
	{
		num /= 10;
		count++;
	}
	return (count);
}

static void	itoa_help(char *ascii, long num, unsigned int count)
{
	ascii[count] = '\0';
	while (count > 0)
	{
		ascii[--count] = (num % 10) + '0';
		num /= 10;
	}
}

/* int	main(void)
{
	int		num;
	char	*ascii;

	num = INT_MAX;
	ascii = ft_itoa(num);
	if (ascii)
	{
		printf("%s\n", ascii);
		free(ascii);
	}
	return (0);
} */
