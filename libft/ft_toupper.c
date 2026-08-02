/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:56:55 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/30 15:05:45 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
	{
		return (c - 32);
	}
	return (c);
}

/* int main(void)
{
	char	test_chars[];
	size_t	num_tests;
    size_t i;
	int		original;
	int		converted;

    test_chars[] = {'a', 'z', 'A', 'Z', '0', '!', 'm', 'M'};
    num_tests = sizeof(test_chars) / sizeof(test_chars[0]);
    printf("Testing ft_toupper:\n");
    for (i = 0; i < num_tests; i++)
    {
        original = test_chars[i];
        converted = ft_toupper(original);
        printf("ft_toupper('%c') = '%c'\n", original, converted);
    }
    return (0);
}
 */