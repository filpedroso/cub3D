/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:06:11 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/30 15:09:18 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return (c + 32);
	}
	return (c);
}

/* int	main(void)
{
	char test_chars[] = {'A', 'Z', 'a', 'z', '0', '!', 'M', 'm'};
	size_t num_tests = sizeof(test_chars) / sizeof(test_chars[0]);

	printf("Testing ft_tolower:\n");
	for (size_t i = 0; i < num_tests; i++)
	{
		int original = test_chars[i];
		int converted = ft_tolower(original);
		printf("ft_tolower('%c') = '%c'\n", original, converted);
	}

	return (0);
} */