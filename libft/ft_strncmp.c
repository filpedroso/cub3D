/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:42:48 by fpedroso          #+#    #+#             */
/*   Updated: 2024/11/06 20:49:15 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n && *s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
	{
		return (0);
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

/* #include "libft.h"
#include <stdio.h>

int main(void)
{
    char *str1 = "Hello, World!";
    char *str2 = "Hello, World!";
    char *str3 = "Hello, 42!";
    char *str4 = "Hello, Universe!";
    size_t n;

    // Test 1: Compare identical strings
    n = 5;
    printf("Test 1: %d\n", ft_strncmp(str1, str2, n)); // Expected: 0

    // Test 2: Compare different strings within the first 7 characters
    n = 7;
    printf("Test 2: %d\n", ft_strncmp(str1, str3, n)); // Expected: 0


    n = 8;
    printf("Test 3: %d\n", ft_strncmp(str1, str3, n)); // Expected: non-zero

    // Test 4: Compare with n larger than string length
    n = 20;
    printf("Test 4: %d\n", ft_strncmp(str1, str2, n)); // Expected: 0

    // Test 5: Compare with n = 0
    n = 0;
    printf("Test 5: %d\n", ft_strncmp(str1, str3, n)); // Expected: 0

    // Test 6: Compare different strings with n = 1
    n = 1;
    printf("Test 6: %d\n", ft_strncmp(str1, str4, n)); // Expected: 0

    return (0);
} */