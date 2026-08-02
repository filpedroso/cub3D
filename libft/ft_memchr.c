/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:32:30 by fpedroso          #+#    #+#             */
/*   Updated: 2024/10/31 14:33:42 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	ch;
	unsigned char	*str;

	ch = (unsigned char)c;
	str = (unsigned char *)s;
	while (n--)
	{
		if (*str == ch)
		{
			return ((void *)str);
		}
		str++;
	}
	return (NULL);
}

/* int main(void)
{
    char str[] = "Hello, World!";
    char ch = 'W';
    size_t n;
    void *result;

    // Test 1: Character found within the first n bytes
    n = 13;
    result = ft_memchr(str, ch, n);
    if (result != NULL)
        printf("Test 1: Character '%c' found at position: %ld\n", ch,
		(char *)result - str);
    else
        printf("Test 1: Character '%c' not found within the first %zu bytes\n",
		ch, n);

    // Test 2: Character not found within the first n bytes
    n = 5;
    result = ft_memchr(str, ch, n);
    if (result != NULL)
        printf("Test 2: Character '%c' found at position: %ld\n", ch,
		(char *)result - str);
    else
        printf("Test 2: Character '%c' not found within the first %zu bytes\n",
		ch, n);

    // Test 3: Character found at the last position
    ch = '!';
    n = 13;
    result = ft_memchr(str, ch, n);
    if (result != NULL)
        printf("Test 3: Character '%c' found at position: %ld\n", ch,
		(char *)result - str);
    else
        printf("Test 3: Character '%c' not found within the first %zu bytes\n",
		ch, n);

    // Test 4: Character not found in the string
    ch = 'x';
    n = 13;
    result = ft_memchr(str, ch, n);
    if (result != NULL)
        printf("Test 4: Character '%c' found at position: %ld\n", ch,
		(char *)result - str);
    else
        printf("Test 4: Character '%c' not found within the first %zu bytes\n",
		ch, n);

    // Test 5: n is 0
    ch = 'H';
    n = 0;
    result = ft_memchr(str, ch, n);
    if (result != NULL)
        printf("Test 5: Character '%c' found at position: %ld\n", ch,
		(char *)result - str);
    else
        printf("Test 5: Character '%c' not found within the first %zu bytes\n",
		ch, n);

    return (0);
} */